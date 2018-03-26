//
// Created by Felix Freyland on 18.03.18.
// E-mail: felixfreyland@gmx.de
//

#include <gtest/gtest.h>
#include "Graph.h"


// _____________________________________________________________________________
TEST(GraphTest, buildFromFile) {
  Graph g;
  g.buildFromFile("test_data/example_graph2.graph");
  // testing names of nodes.
  ASSERT_EQ(g._numNodes, 3);
  ASSERT_EQ(g._nodeNames.size(), 3);
  ASSERT_EQ(g._nodeNames[0], "starting-point");
  ASSERT_EQ(g._nodeNames[1], "node1");
  ASSERT_EQ(g._nodeNames[2], "node2");

  // testing geoLocations.
  ASSERT_EQ(g._geoLocations.size(), 3);
  ASSERT_EQ(std::get<0>(g._geoLocations[0]), 0.0);
  ASSERT_EQ(std::get<1>(g._geoLocations[0]), 0.0);
  ASSERT_EQ(std::get<0>(g._geoLocations[1]), 0.0);
  ASSERT_EQ(std::get<1>(g._geoLocations[1]), 0.0);
  ASSERT_EQ(std::get<0>(g._geoLocations[2]), 0.0);
  ASSERT_EQ(std::get<1>(g._geoLocations[2]), 0.0);

  // testing releases and deadlines.
  ASSERT_EQ(g._releases.size(), 3);
  ASSERT_EQ(g._releases[0], 0);
  ASSERT_EQ(g._releases[1], 4);
  ASSERT_EQ(g._releases[2], 8);

  ASSERT_EQ(g._deadlines.size(), 3);
  ASSERT_EQ(g._deadlines[0], 0);
  ASSERT_EQ(g._deadlines[1], 6);
  ASSERT_EQ(g._deadlines[2], 12);

  // testing prizes.
  ASSERT_EQ(g._prizes.size(), 3);
  ASSERT_EQ(g._prizes[0], 0);
  ASSERT_EQ(g._prizes[1], 1);
  ASSERT_EQ(g._prizes[2], 1);

  // testing durations.
  ASSERT_EQ(g._durations.size(), 3);
  ASSERT_EQ(g._durations[0], 0);
  ASSERT_EQ(g._durations[1], 2);
  ASSERT_EQ(g._durations[2], 2);

  // testing distance matrix.
  ASSERT_EQ(g._distances.size(), 3);

  for (int i = 0; i < 3; i++) {
    ASSERT_EQ(g._distances[i].size(), 3);
  }
  ASSERT_EQ(g._distances[0][0], 0.0);
  ASSERT_EQ(g._distances[0][1], 5.0);
  ASSERT_EQ(g._distances[0][2], 10.0);
  ASSERT_EQ(g._distances[1][0], 5.0);
  ASSERT_EQ(g._distances[1][1], 0.0);
  ASSERT_EQ(g._distances[1][2], 8.0);
  ASSERT_EQ(g._distances[2][0], 10.0);
  ASSERT_EQ(g._distances[2][1], 8.0);
  ASSERT_EQ(g._distances[2][2], 0.0);

  Graph g1;
  g1.buildFromFile("test_data/example_graph3.graph", false);
  // testing names of nodes.
  ASSERT_EQ(g1._numNodes, 4);
  ASSERT_EQ(g1._nodeNames.size(), 4);
  ASSERT_EQ(g1._nodeNames[0], "starting-point");
  ASSERT_EQ(g1._nodeNames[1], "40 Greg Urwin Circuit, Casey");
  ASSERT_EQ(g1._nodeNames[2], "11 Helmrich Street, Moncrieff");
  ASSERT_EQ(g1._nodeNames[3], "129 Slim Dusty Circuit, Moncrieff, ACT");

  // testing geoLocations.
  ASSERT_EQ(g1._geoLocations.size(), 4);
  ASSERT_FLOAT_EQ(std::get<0>(g1._geoLocations[0]), 0.0);
  ASSERT_FLOAT_EQ(std::get<1>(g1._geoLocations[0]), 0.0);
  ASSERT_FLOAT_EQ(std::get<0>(g1._geoLocations[1]), -35.1552247);
  ASSERT_FLOAT_EQ(std::get<1>(g1._geoLocations[1]), 149.1015429);
  ASSERT_FLOAT_EQ(std::get<0>(g1._geoLocations[2]), -35.1559098);
  ASSERT_FLOAT_EQ(std::get<1>(g1._geoLocations[2]), 149.1071681);
  ASSERT_FLOAT_EQ(std::get<0>(g1._geoLocations[3]), -35.1560337);
  ASSERT_FLOAT_EQ(std::get<1>(g1._geoLocations[3]), 149.1097125);

  // testing releases and deadlines.
  ASSERT_EQ(g1._releases.size(), 4);
  ASSERT_EQ(g1._releases[0], 0);
  ASSERT_EQ(g1._releases[1], 735);
  ASSERT_EQ(g1._releases[2], 630);
  ASSERT_EQ(g1._releases[3], 630);

  ASSERT_EQ(g1._deadlines.size(), 4);
  ASSERT_EQ(g1._deadlines[0], 0);
  ASSERT_EQ(g1._deadlines[1], 780);
  ASSERT_EQ(g1._deadlines[2], 840);
  ASSERT_EQ(g1._deadlines[3], 840);

  // testing prizes.
  ASSERT_EQ(g1._prizes.size(), 4);
  ASSERT_EQ(g1._prizes[0], 0);
  ASSERT_EQ(g1._prizes[1], 3);
  ASSERT_EQ(g1._prizes[2], 2);
  ASSERT_EQ(g1._prizes[3], 1);

  // testing durations.
  ASSERT_EQ(g1._durations.size(), 4);
  ASSERT_EQ(g1._durations[0], 0);
  ASSERT_EQ(g1._durations[1], 15);
  ASSERT_EQ(g1._durations[2], 15);
  ASSERT_EQ(g1._durations[3], 15);

  // testing distance matrix.
  ASSERT_EQ(g1._distances.size(), 4);

  for (int i = 0; i < 4; i++) {
    ASSERT_EQ(g1._distances[i].size(), 4);
  }
  ASSERT_FLOAT_EQ(g1._distances[0][0], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[0][1], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[0][2], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[0][3], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[1][0], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[1][1], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[1][2], 3.98);
  ASSERT_FLOAT_EQ(g1._distances[1][3], 3.02);
  ASSERT_FLOAT_EQ(g1._distances[2][0], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[2][1], 4.09);
  ASSERT_FLOAT_EQ(g1._distances[2][2], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[2][3], 1.01);
  ASSERT_FLOAT_EQ(g1._distances[3][0], 0.0);
  ASSERT_FLOAT_EQ(g1._distances[3][1], 3.09);
  ASSERT_FLOAT_EQ(g1._distances[3][2], 0.96);
  ASSERT_FLOAT_EQ(g1._distances[3][3], 0.0);

}
