// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <tuple>
#include <algorithm>
#include <vector>
#include <set>
#include "FptSolver.h"

// _____________________________________________________________________________
bool Constraint::operator>(const Constraint &newConstr) {
  if (newConstr.time > this->time || newConstr.revenue < this->revenue) {
    return false;
  } else {
    std::set<size_t> intersection;
    std::set_intersection(newConstr.prohibJobs.begin(),
                    newConstr.prohibJobs.end(),
                    this->prohibJobs.begin(), this->prohibJobs.end(),
                    std::inserter(intersection, intersection.begin()));
    return intersection == newConstr.prohibJobs;
  }
}


FptSolver::~FptSolver() = default;

// _____________________________________________________________________________
FptSolver::FptSolver(Graph graph) {
  _graph = graph;
  _constraints = {};
}

// _____________________________________________________________________________
void FptSolver::initConstraints() {
  size_t dimension = _graph.getNodesNum();
  // Initialize empty field for constraints.
  for (size_t i = 0; i < dimension; i++) {
    vector<vector<Constraint>> level = {};
    for (size_t j = 0; j < dimension; j++) {
      vector<Constraint> constraint = {};
      level.push_back(constraint);
    }
    _constraints.push_back(level);
  }

  // initialise constraints at first level.
  for (size_t node = 1; node < dimension; node++) {
    auto time = static_cast<double>(_graph.getReleases()->at(node));
    size_t revenue = _graph.getPrizes()->at(node);
    tuple<size_t, size_t> pred {0, 0};
    set<size_t> prohibJ = {node};
    Constraint constr = {time, revenue, pred, prohibJ};
    _constraints[1][node].push_back(constr);
  }
}

// _____________________________________________________________________________
tuple<size_t, tuple<size_t, size_t, size_t>> FptSolver::solve() {
  initConstraints();
  size_t max_prize = 0;
  // to remember the (level, node_id, constraint_id) of the so far best tour.
  tuple<size_t, size_t, size_t>  bestTourEnd(1, 0, 0);
  bool done = false;  // to check if there is any continuation.
  size_t nodesNum = _graph.getNodesNum();

  // different levels.
  for (size_t level = 1; level < nodesNum; level++) {
    if (done) {break;}
    done = true;

    // for all all jobs at current level.
    for (size_t job = 1; job < nodesNum; job++) {
      // check constraints for continuation of a tour.
      size_t constrId = 0;
      for (auto constr : _constraints[level][job]) {
        size_t prize = constr.revenue;
        if (prize > max_prize) {
          max_prize = prize;
          std::get<0>(bestTourEnd) = level;
          std::get<1>(bestTourEnd) = job;
          std::get<2>(bestTourEnd) = constrId;
        }
        // all jobs at next level.
        for (size_t successor = 1; successor < nodesNum; successor++) {
          if (constr.prohibJobs.count(successor) != 0) {continue;}
          double timeAtNext = constr.time
              + static_cast<double>(_graph.getDurations()->at(job))
              + (_graph.getDistances()->at(job))[successor]
              + static_cast<double>(_graph.getDurations()->at(successor));

          set<size_t> newProhib {successor};
          size_t deadline = _graph.getDeadlines()->at(successor);
          if (timeAtNext > static_cast<double>(deadline)) {
            continue;
          }
          // check if new job has to be added to prohibited Jobs.
          for (auto tourJob : constr.prohibJobs) {
            if (checkProhibited(successor, tourJob, timeAtNext)) {
              newProhib.insert(tourJob);
            }
          }
          done = false;  // a continuation is possible;

          size_t newPrize = constr.revenue + _graph.getPrizes()->at(successor);
          auto sucRelease = static_cast<double>(_graph.getReleases()
                                               ->at(successor));
          auto duration = static_cast<double>(_graph.getDurations()->at(job));
          double travelTime = _graph.getDistances()->at(job)[successor];
          double newTime = std::max(sucRelease, constr.time
                                   + duration + travelTime);

          tuple<size_t , size_t > predecessor {job, constrId};
          Constraint newConstr = {newTime, newPrize, predecessor, newProhib};
          updateConstraints(newConstr, level + 1, successor);
        }
        constrId++;
      }
    }
  }
  return std::make_tuple(max_prize, bestTourEnd);
}

// _____________________________________________________________________________
bool FptSolver::checkProhibited(const size_t node1, const size_t node2,
                                const double time) const {
  auto release = static_cast<double>(_graph.getReleases()->at(node1));
  auto duration1 = static_cast<double>(_graph.getDurations()->at(node1));
  auto duration2 = static_cast<double>(_graph.getDurations()->at(node2));
  double earliestStart = std::max(time, release + duration1);
  double travelTime = _graph.getDistances()->at(node1)[node2];
  double deadline = _graph.getDeadlines()->at(node2);
  return earliestStart + travelTime + duration2 <= deadline;
}

// _____________________________________________________________________________
void FptSolver::updateConstraints(Constraint newConstr, const size_t row,
                                  const size_t col) {
  bool newisGood = true;
  vector<Constraint> updatedCons;
  for (auto constr : _constraints[row][col]) {
    if (!(constr > newConstr)) {
      updatedCons.push_back(constr);
    }
  }
  for (const auto &constr : updatedCons) {
    if (newConstr > constr) {
      newisGood = false;
      break;
    }
  }
  if (newisGood) {
    updatedCons.push_back(newConstr);
  }
  _constraints[row][col].swap(updatedCons);
}

// _____________________________________________________________________________
vector<Location> const FptSolver::getTour(tuple<size_t,
                                                size_t, size_t> tourEnd) const {
  vector<Location> reversePath;
  size_t level = std::get<0>(tourEnd);
  size_t job = std::get<1>(tourEnd);
  size_t constrId = std::get<2>(tourEnd);

  while (level > 0) {
    Constraint actualConstr = _constraints[level][job][constrId];
    auto arrival = actualConstr.time;
    auto leave = arrival + _graph.getDurations()->at(job);
    auto prize = _graph.getPrizes()->at(job);
    auto geoLoc = _graph.getLocations()->at(job);
    auto lat = std::get<0>(geoLoc);
    auto longit = std::get<1>(geoLoc);
    auto  name = _graph.getNodeNames()->at(job);
    Location node = {job, prize, arrival, leave, lat, longit, name};
    reversePath.push_back(node);

    // get predecessor on the tour.
    job = std::get<0>(actualConstr.predecessor);
    constrId = std::get<1>(actualConstr.predecessor);
    level--;
  }

  vector<Location> path;
  while (!reversePath.empty()) {
    auto loc = reversePath.back();
        path.push_back(loc);
    reversePath.pop_back();
  }
  return path;
}
