// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <gtest/gtest.h>
#include <iostream>
#include <tuple>
#include <set>
#include "./MlipSolver.h"

// _____________________________________________________________________________
TEST(FptSolverTest, solve) {
  Graph g;
  g.buildFromFile("test_data/example_graph2.graph", true);
  MlipSolver solver = MlipSolver(g);
  auto solution = solver.solve();
  auto path = solver.getTour();
  ASSERT_EQ(solution, 1);
  ASSERT_EQ(path.size(), 1);
  ASSERT_EQ(path[0].id, 1);
  ASSERT_EQ(path[0].arrival, 4);
  ASSERT_EQ(path[0].prize, 1);
  ASSERT_EQ(path[0].leave, 6);
  ASSERT_EQ(path[0].name, "node1");

  Graph g1;
  g1.buildFromFile("test_data/example_graph4.graph", false);
  MlipSolver solver1 = MlipSolver(g1);
  auto solution1 = solver1.solve();
  auto path1 = solver1.getTour();
  ASSERT_EQ(path1.size(), 3);
  ASSERT_EQ(path1[0].id, 2);
  ASSERT_EQ(path1[1].id, 3);
  ASSERT_EQ(path1[2].id, 4);

  ASSERT_EQ(path1[0].arrival, 0);
  ASSERT_EQ(path1[1].arrival, 6);
  ASSERT_EQ(path1[2].arrival, 13);

  ASSERT_EQ(path1[0].prize, 3);
  ASSERT_EQ(path1[1].prize, 6);
  ASSERT_EQ(path1[2].prize, 3);

  ASSERT_EQ(path1[0].leave, 1);
  ASSERT_EQ(path1[1].leave, 7);
  ASSERT_EQ(path1[2].leave, 14);

  ASSERT_EQ(path1[0].name, "node2");
  ASSERT_EQ(path1[1].name, "node3");
  ASSERT_EQ(path1[2].name, "node4");
}
