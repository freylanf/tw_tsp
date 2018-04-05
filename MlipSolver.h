// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#ifndef MLIPSOLVER_H_
#define MLIPSOLVER_H_

#include <gtest/gtest.h>
#include "Graph.h"
#include <gurobi_c++.h>
#include <string>
#include <tuple>
#include <vector>

using std::vector;
using std::set;
using std::string;
using std::tuple;

// Class to solve PC_TW_TSP instance with a mixed linear
// integer program (MLIP).

class MlipSolver {
 public:
  // Constructor.
  explicit MlipSolver(Graph graph): _model(_env) {
    _graph = graph;
  }

  // Algorithm computing the optimal tour.
  // Returns a tuple containing the value of the otimal tour.
  // and a vector of the locations on the tour.
  size_t solve(double timeOut = 600.0);

  // To calculate the optimal tour.
  vector<Location> getTour();

  // Destructor
  ~MlipSolver();

 private:
  // To setup the variables, constraints and
  // objective function of the MLIP.
  void setupModel();

  Graph _graph;  // The graph to solve.

  GRBEnv _env;  // The model environment.

  GRBModel _model;  // The model.

  // Model Variables and Constraints.
  GRBVar* _nodes;
  GRBVar** _edges;
  GRBVar* _arrivals;
  GRBVar* _leaves;
  GRBConstr* _nodeOutConstr;
  GRBConstr* _nodeInConstr;
  GRBConstr* _durConstr;
  GRBConstr** _distConstr;
};

#endif  // MLIPSOLVER_H_
