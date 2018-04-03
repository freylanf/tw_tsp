//
// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#ifndef FPTSOLVER_H_
#define FPTSOLVER_H_

#include <gtest/gtest.h>
#include "Graph.h"
#include <set>
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>

using std::vector;
using std::set;
using std::string;
using std::tuple;

// Structure for a single constraint of a partial tour.
struct Constraint {
  double time;
  size_t revenue;
  tuple<size_t, size_t> predecessor;
  set<size_t> prohibJobs;

  // The > operator checks if a constraint is obsolete. It is
  // not better than the new constraint starts earlier, has
  // a larger revenue and its prohib set is contained in the
  // constraints prohib set.
  bool operator>(const Constraint &newConstr);
};

// Class to solve PC_TW_TSP instance with a dynamic programming
// approach as suggested by [Nebel, Renz].

class FptSolver {
 public:
  // Constructor taking a graph instance.
  explicit FptSolver(Graph graph);

  // Algorithm computing the optimal tour.
  tuple<size_t, tuple<size_t, size_t, size_t>> solve();
  FRIEND_TEST(FptSolverTest, solve);

  // Method to compute the optimal tour for a solved instance
  // by going backwards through constraints.
  vector<Location> const getTour(tuple<size_t, size_t, size_t > tourEnd) const;
  FRIEND_TEST(FptSolverTest, getTour);

  // Destructor
  ~FptSolver();

 private:
  Graph _graph;  // the graph to solve.
  vector<vector<vector<Constraint>>> _constraints;

  // Initialize a 2D field for the constraints.
  void initConstraints();
  FRIEND_TEST(FptSolverTest, initConstraints);

  // Method to check whether node2 has to be added to prohibited
  // jobs of a partial tour ending in node1.
  bool checkProhibited(size_t node1, size_t node2, double time) const;
  FRIEND_TEST(FptSolverTest, checkProhibited);

  // Updates the set of constraints for a partial tour.
  // The constraints at row, col are updated.
  // All constraints (t', P', revenue') in the set of constraints
  // at idx (row, node) are removed if t <= t', P subset P' and
  // revenue >= revenue'. Where (t, P, revenue) is the new Constraint).
  void updateConstraints(Constraint newConstr, size_t row, size_t col);
  FRIEND_TEST(FptSolverTest, updateConstraints);
};

#endif  // FPTSOLVER_H_
