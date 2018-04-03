// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#ifndef GRAPH_H_
#define GRAPH_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <tuple>

using std::string;
using std::vector;
using std::tuple;

// Structure for a location in the graph.
struct Location {
  size_t id;
  size_t prize;
  double arrival;
  double leave;
  double latitude;
  double longitude;
  string name;
};

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
  const vector<vector<double>>* getDistances() const;
  const vector<tuple<double, double>>* getLocations() const;
  const vector<size_t>* getDurations() const;

  // To read the graph from a text file.
  void buildFromFile(string fileName, bool unitPrizes = true);
  FRIEND_TEST(GraphTest, buildFromFile);

  // Destructor
  ~Graph();

 private:
  size_t _numNodes;
  vector<size_t> _releases;
  vector<size_t> _deadlines;
  vector<size_t> _durations;
  vector<size_t> _prizes;
  vector<vector<double>> _distances;
  vector<string> _nodeNames;
  vector<tuple<double, double>> _geoLocations;
};

#endif  // GRAPH_H_

