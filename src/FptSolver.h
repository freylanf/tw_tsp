//
// Created by Felix Freyland on 22.03.18.
// E-mail: felixfreyland@gmx.de
//

#ifndef TW_TSP_FPTSOLVER_H
#define TW_TSP_FPTSOLVER_H

#include <gtest/gtest.h>
#include "Graph.h"
#include <set>
#include <algorithm>


using std::vector;
using std::set;
using std::string;
using std::tuple;

//Structure for a single constraint of a partial tour.
struct Constraint {
  float time;
  size_t revenue;
  tuple<size_t, size_t> predecessor;
  set<size_t> prohibJobs;

  // The > operator checks if a constraint is obsolete. It is
  // not better than the new constraint starts earlier, has
  // a larger revenue and its prohib set is contained in the
  // constraints prohib set.
  bool operator>(const Constraint &newConstr);
};

// Structure for a location in the calculated tour.
struct Location {
  size_t id;
  size_t prize;
  float arrival;
  float leave;
  float latitude;
  float longitude;
  string name;

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

  // Destructor
  ~FptSolver();

 private:
  Graph _graph; // the graph to solve.
  vector<vector<vector<Constraint>>> _constraints;

  // Initialize a 2D field for the constraints.
  void initConstraints();
  FRIEND_TEST(FptSolverTest, initConstraints);

  // Method to check whether node2 has to be added to prohibited
  // jobs of a partial tour ending in node1.
  bool checkProhibited (size_t node1, size_t node2, float time) const;
  FRIEND_TEST(FptSolverTest, checkProhibited);

  // Updates the set of constraints for a partial tour.
  // The constraints at row, col are updated.
  // All constraints (t', P', revenue') in the set of constraints
  // at idx (row, node) are removed if t <= t', P subset P' and
  // revenue >= revenue'. Where (t, P, revenue) is the new Constraint).
  void updateConstraints(Constraint &newConstr, size_t row, size_t col);
  FRIEND_TEST(FptSolverTest, updateConstraints);
  // Method to compute the optimal tour for a solved instance
  // by going backwards through constraints.
  vector<Location> const getTour(tuple<size_t, size_t, size_t > tourEnd) const;
  FRIEND_TEST(FptSolverTest, getTour);
};

#endif //TW_TSP_FPTSOLVER_H
