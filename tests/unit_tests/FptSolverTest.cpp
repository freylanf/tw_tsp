//
// Created by Felix Freyland on 18.03.18.
// E-mail: felixfreyland@gmx.de
//

#include <gtest/gtest.h>
#include "FptSolver.h"
#include <iostream>


// _____________________________________________________________________________
TEST(FptSolverTest, initConstraints) {
  Graph g;
  g.buildFromFile("test_data/example_graph2.graph");
  FptSolver solver = FptSolver(g);
  solver.initConstraints();
  ASSERT_EQ(solver._constraints.size(), 3);
  for (int i = 0; i < 3; i++) {
    ASSERT_EQ(solver._constraints[i].size(), 3);
  }
  ASSERT_EQ(solver._constraints[1][1][0].time, 4);
  ASSERT_EQ(solver._constraints[1][1][0].revenue, 1);
  tuple<int, int> t1 {0, 0};
  ASSERT_EQ(solver._constraints[1][1][0].predecessor, t1);
  set<size_t> s1 = {1};
  ASSERT_EQ(solver._constraints[1][1][0].prohibJobs, s1);
  ASSERT_EQ(solver._constraints[1][2][0].time, 8);
  ASSERT_EQ(solver._constraints[1][2][0].revenue, 1);
  tuple<size_t, size_t> t {0, 0};
  ASSERT_EQ(solver._constraints[1][2][0].predecessor, t);
  set<size_t> s = {2};
  ASSERT_EQ(solver._constraints[1][2][0].prohibJobs, s);
}

// _____________________________________________________________________________
TEST(FptSolverTest, checkProhibited) {
  Graph g;
  g.buildFromFile("test_data/example_graph2.graph");
  FptSolver solver = FptSolver(g);
  solver.initConstraints();
  ASSERT_FALSE(solver.checkProhibited(1, 2, 5));

}

// _____________________________________________________________________________
TEST(FptSolverTest, solve) {
  Graph g1;
  g1.buildFromFile("test_data/example_graph2.graph");
  FptSolver solver1 = FptSolver(g1);
  auto result = solver1.solve();
  ASSERT_EQ(std::get<0>(result), 1);
  tuple<size_t, size_t, size_t> end {1, 1, 0};
  ASSERT_EQ(std::get<1>(result), end);

  Graph g2;
  g2.buildFromFile("test_data/example_graph3.graph");
  FptSolver solver2 = FptSolver(g2);
  auto result2 = solver2.solve();
  ASSERT_EQ(std::get<0>(result2), 3);
  tuple<size_t, size_t, size_t> end2 {3, 1, 0};
  ASSERT_EQ(std::get<1>(result2), end2);

  Graph g3;
  g3.buildFromFile("test_data/example_graph4.graph", false);
  FptSolver solver3 = FptSolver(g3);
  auto result3 = solver3.solve();
  ASSERT_EQ(std::get<0>(result3), 12);
  tuple<size_t, size_t, size_t> end3 {3, 4, 1};
  ASSERT_EQ(std::get<1>(result3), end3);

  Graph g4;
  g4.buildFromFile("test_data/example_graph4.graph");
  FptSolver solver4 = FptSolver(g4);
  auto result4 = solver4.solve();
  ASSERT_EQ(std::get<0>(result4), 3);
  tuple<size_t, size_t, size_t> end4 {3, 4, 0};
  ASSERT_EQ(std::get<1>(result4), end4);
}

// _____________________________________________________________________________
TEST(FptSolverTest, updateConstraints) {
  Graph g;
  g.buildFromFile("test_data/example_graph4.graph");
  FptSolver s = FptSolver(g);
  s.initConstraints();
  tuple<size_t, size_t> pred {0, 0};

  set<size_t> prohibJNew = {2};
  set<size_t> prohibJ1 = {2, 3};
  set<size_t> prohibJ2 = {1, 3};

  Constraint newCon = {4, 5, pred, prohibJNew};
  Constraint constr1 = {5, 3, pred, prohibJ1};
  Constraint constr2 = {6, 3, pred, prohibJ2};

  s._constraints[2][2].push_back(constr1);
  s._constraints[2][2].push_back(constr2);
  s.updateConstraints(newCon, 2, 2);

  // constraint #2 will be deleted and the new constraint added.
  ASSERT_EQ(s._constraints[2][2].size(), 2);
  ASSERT_EQ(s._constraints[2][2][0].time, 6);
  ASSERT_EQ(s._constraints[2][2][0].revenue, 3);
  ASSERT_EQ(s._constraints[2][2][1].time, 4);
  ASSERT_EQ(s._constraints[2][2][1].revenue, 5);

  set<size_t> prohibJNew1 = {2};
  set<size_t> prohibJ1_1 = {2, 3};
  set<size_t> prohibJ2_1 = {2};

  Constraint newCon1 = {4, 5, pred, prohibJNew1};
  Constraint constr11 = {5, 3, pred, prohibJ1_1};
  Constraint constr21 = {3, 6, pred, prohibJ2_1};

  s._constraints[2][3].push_back(constr11);
  s._constraints[2][3].push_back(constr21);
  s.updateConstraints(newCon, 2, 3);

  // new constraint not added and #2 deleted.
  ASSERT_EQ(s._constraints[2][3].size(), 1);
  ASSERT_EQ(s._constraints[2][3][0].time, 3);
  ASSERT_EQ(s._constraints[2][3][0].revenue, 6);
}

// _____________________________________________________________________________
TEST(FptSolverTest, getTour) {
  Graph g;
  g.buildFromFile("test_data/example_graph4.graph", false);
  FptSolver s = FptSolver(g);
  auto tourEnd = std::get<1>(s.solve());
  auto path = s.getTour(tourEnd);
  ASSERT_EQ(path.size(), 3);
  ASSERT_EQ(path[0].id, 2);
  ASSERT_EQ(path[1].id, 3);
  ASSERT_EQ(path[2].id, 4);

  ASSERT_EQ(path[0].arrival, 0);
  ASSERT_EQ(path[1].arrival, 6);
  ASSERT_EQ(path[2].arrival, 13);

  ASSERT_EQ(path[0].prize, 3);
  ASSERT_EQ(path[1].prize, 6);
  ASSERT_EQ(path[2].prize, 3);

  ASSERT_EQ(path[0].leave, 1);
  ASSERT_EQ(path[1].leave, 7);
  ASSERT_EQ(path[2].leave, 14);

  ASSERT_EQ(path[0].name, "node2");
  ASSERT_EQ(path[1].name, "node3");
  ASSERT_EQ(path[2].name, "node4");
 }