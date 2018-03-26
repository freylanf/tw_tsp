//
// Created by Felix Freyland on 18.03.18.
// E-mail: felixfreyland@gmx.de
//

#ifndef TW_TSP_GRAPH_H_
#define TW_TSP_GRAPH_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <tuple>

using std::string;
using std::vector;
using std::tuple;

// A class for a graph TW_TSP instance.
class Graph {
 public:
  // Default Constructor.
  Graph();

  // Getters.
  size_t getNodesNum() const;
  const vector<size_t>* getDeadlines() const;
  const vector<size_t>* getReleases() const;
  const vector<size_t>* getPrizes() const;
  const vector<string>* getNodeNames() const;
  const vector<vector<float>>* getDistances() const;
  const vector<tuple<float, float>>* getLocations() const;
  const vector<size_t>* getDurations() const;

  // To read the graph from a text file.
  void buildFromFile(string fileName, bool unitPrizes=true);
  FRIEND_TEST(GraphTest, buildFromFile);

  // Destructor
  ~Graph();
 private:
  size_t _numNodes;
  vector<size_t> _releases;
  vector<size_t> _deadlines;
  vector<size_t> _durations;
  vector<size_t> _prizes;
  vector<vector<float>> _distances;
  vector<string> _nodeNames;
  vector<tuple<float, float>> _geoLocations;
};

#endif // TW_TSP_GRAPH_H_

