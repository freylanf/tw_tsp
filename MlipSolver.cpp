// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <tuple>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include "MlipSolver.h"

// _____________________________________________________________________________
size_t MlipSolver::solve(double timeOut) {
  _model.set(GRB_IntParam_LogToConsole, 0);
  _model.set(GRB_IntParam_OutputFlag, 0);
  _model.set(GRB_StringParam_LogFile, "gurobi.log");
  setupModel();
  _model.optimize();
  size_t optimum = static_cast<size_t>(_model.get(GRB_DoubleAttr_ObjVal));
  return optimum;
}

// ____________________________________________________________________________
void MlipSolver::setupModel() {
  const size_t totalNodes = _graph.getNodesNum();
  const size_t startId = 0;
  const size_t endId = totalNodes;

  // adding a virtual end Node to the graph instance.
  vector<size_t> releases = *(_graph.getReleases());
  releases.push_back(0);
  vector<size_t> deadlines = *(_graph.getDeadlines());
  deadlines.push_back(1440);
  vector<size_t> durations = *(_graph.getDurations());
  durations.push_back(0);
  vector<size_t> prizes = *(_graph.getPrizes());
  prizes.push_back(0);
  vector<vector<double>> distances = *(_graph.getDistances());

  // Distance from startpoint to all locations set to zero.
  // A tour can immediately start at any location.
  for (size_t i = 0; i < totalNodes; i++) {
    distances[0][i] = 0;
  }
  // Adding zero distances to virtual end location.
  for (size_t i = 0; i < totalNodes; i ++) {
    distances[i].push_back(0);
  }
  vector<double> lastRow(totalNodes + 1, 0.0);
  distances.push_back(lastRow);

  // setup model Variables.
  _nodes = new GRBVar[totalNodes + 1];
  _edges = new GRBVar*[totalNodes + 1];
  for (size_t i = 0; i < totalNodes + 1; i++) {
    _edges[i] = new GRBVar[totalNodes + 1];
  }
  _arrivals = new GRBVar[totalNodes + 1];
  _leaves = new GRBVar[totalNodes + 1];

  // Nodes.
  for (size_t id = 1; id < endId; id++) {
    std::ostringstream nodeName;
    nodeName << "node" << id;
    _nodes[id] = _model.addVar(0.0, 1.0, 0.0, GRB_BINARY, nodeName.str());
  }

  // Edges.
  for (size_t src = 0; src <= endId; src++) {
    for (size_t targ = 0; targ <= endId; targ++) {
      std::ostringstream edgeName;
      if (src != targ) {
        edgeName << "edge" << src << targ;
        _edges[src][targ] = _model.addVar(0.0, 1.0, 0.0,
                           GRB_BINARY, edgeName.str());
      }
    }
  }

  // Arrivals at all nodes excluding start.
  for (size_t i = 1; i <= endId; i++) {
    std::ostringstream name;
    name << "arrive" << i;
    double earliestArr = releases[i];
    double latestArr = deadlines[i] - durations[i];
    _arrivals[i] = _model.addVar(earliestArr, latestArr, 0.0, GRB_CONTINUOUS,
                                name.str());
  }

  for (size_t i = 0; i < endId; i++) {
    std::ostringstream name;
    name << "leave" << i;
    double earliestLeave = releases[i] + durations[i];
    double latestLeave = deadlines[i];
    _leaves[i] = _model.addVar(earliestLeave, latestLeave, 0.0, GRB_CONTINUOUS,
                                name.str());
  }

  // setup model Objective.
  GRBLinExpr objFunction = 0;
  for (size_t i = 1; i < endId; i++) {
    objFunction += _nodes[i] * prizes[i];
  }
  _model.setObjective(objFunction, GRB_MAXIMIZE);

  // setup model constraints.

  // Exactly one edge out of start node.
  GRBLinExpr startOut = 0;
  for (size_t i = 1; i <= endId; i++) {
    startOut += _edges[0][i];
  }
  _model.addConstr(startOut == 1.0, "startOut");

  // Exactly one edge into end node.
  GRBLinExpr endIn = 0;
  for (size_t i = 0; i < endId; i++) {
    endIn += _edges[i][endId];
  }
  _model.addConstr(endIn == 1.0, "endIn");

  // All used nodes have one outgoing and one incoming edge.
  _nodeOutConstr = new GRBConstr[totalNodes + 1];
  _nodeInConstr = new GRBConstr[totalNodes + 1];
  for (size_t i = 1; i < endId; i++) {
    GRBLinExpr sumOut = 0;
    GRBLinExpr sumIn = 0;
    for (size_t j = 1; j <= endId; j++) {
      if (i != j) {
        sumOut += _edges[i][j];
      }
    }
    for (size_t j = 0; j < endId; j++) {
      if (i != j) {
        sumIn += _edges[j][i];
      }
    }
    std::ostringstream cOut;
    std::ostringstream cIn;
    cOut << i << "_out_";
    cIn << i << "_inn_";
    _nodeOutConstr[i] = _model.addConstr(sumOut == _nodes[i], cOut.str());
    _nodeOutConstr[i] = _model.addConstr(sumIn == _nodes[i], cIn.str());
  }

  _durConstr = new GRBConstr[totalNodes + 1];
  _distConstr = new GRBConstr*[totalNodes + 1];
  for (size_t i = 0; i < totalNodes + 1; i++) {
    _distConstr[i] = new GRBConstr[totalNodes + 1];
  }

  // Constraints for visiting times.
  for (size_t i = 1; i < endId; i++) {
    _durConstr[i] = _model.addConstr(_arrivals[i] + durations[i] == _leaves[i]);
  }

  // Constraints for reachability of nodes.
  for (size_t src = 0; src < endId; src++) {
    for (size_t targ = 1; targ <= endId; targ++) {
      if (src != targ) {
        GRBLinExpr factor = 1.0 - _edges[src][targ];
        GRBLinExpr compValue = 0;
        if (deadlines[src] + distances[src][targ] > releases[targ]) {
          compValue = (deadlines[src] + distances[src][targ] - releases[targ])
                       * factor;
        }
        try {
          _distConstr[src][targ] = _model.addConstr(_leaves[src]
                                  + distances[src][targ]
                                  - _arrivals[targ] <= compValue);
        } catch(GRBException e) {
          std::cout << e.getErrorCode() << std::endl;
          exit(1);
        }
      }
    }
  }
}

// ____________________________________________________________________________
vector<Location> MlipSolver::getTour() {
  size_t maxNodes = _graph.getNodesNum();
  vector<tuple<size_t, size_t>> edgesTaken;
  for (size_t row = 0; row < maxNodes; row++) {
    for (size_t col = 1; col < maxNodes + 1; col++) {
      if (row != col) {
        double edgeValue = _edges[row][col].get(GRB_DoubleAttr_X);
        if (edgeValue == 1.00) {
          tuple<size_t, size_t> pathEdge {row, col};
          edgesTaken.push_back(pathEdge);
        }
      }
    }
  }
  vector<Location> path;
  size_t nextLoc = std::get<1>(edgesTaken[0]);
  while (true) {
    if (nextLoc == maxNodes) {
      break;
    }
    for (auto edge : edgesTaken) {
      if (std::get<0>(edge) == nextLoc) {
        size_t idx = std::get<0>(edge);
        double leave = _leaves[idx].get(GRB_DoubleAttr_X);
        double arrival = _arrivals[idx].get(GRB_DoubleAttr_X);
        size_t prize = _graph.getPrizes()->at(idx);
        double lat = std::get<0>(_graph.getLocations()->at(idx));
        double longit = std::get<1>(_graph.getLocations()->at(idx));
        string name = _graph.getNodeNames()->at(idx);
        Location loc = {idx, prize, arrival, leave, lat, longit, name};
        path.push_back(loc);
        nextLoc = std::get<1>(edge);
        break;
      }
    }
  }
  return path;
}

// ____________________________________________________________________________
MlipSolver::~MlipSolver() {
  delete[] _nodes;
  for (size_t i = 0; i < _graph.getNodesNum() + 1; i++) {
    delete[] _edges[i];
  }
  delete[] _edges;
  delete[] _arrivals;
  delete[] _leaves;
  delete[] _nodeOutConstr;
  delete[] _nodeInConstr;
  delete[] _durConstr;
  for (size_t i = 0; i < _graph.getNodesNum() + 1; i++) {
    delete[] _distConstr[i];
  }
  delete[] _distConstr;
}
