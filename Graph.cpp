// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include "Graph.h"

// ____________________________________________________________________________
Graph::Graph() {
  _numNodes = 0;
  _releases = {};
  _deadlines = {};
  _durations = {};
  _prizes = {};
  _distances = {};
  _nodeNames = {};
  _geoLocations = {};
}

// ____________________________________________________________________________
vector<size_t> const * Graph::getDeadlines() const {
  return &_deadlines;
}

size_t Graph::getNodesNum() const {
  return _numNodes;
}

vector<size_t> const * Graph::getReleases() const {
  return &_releases;
}

vector<size_t> const * Graph::getPrizes() const {
  return &_prizes;
}

vector<string> const * Graph::getNodeNames() const {
  return &_nodeNames;
}

vector<vector<double>> const * Graph::getDistances() const {
  return &_distances;
}

vector<tuple<double, double>> const * Graph::getLocations() const {
  return &_geoLocations;
}

vector<size_t> const * Graph::getDurations() const {
  return &_durations;
}

// ____________________________________________________________________________
void Graph::buildFromFile(const string fileName, const bool unitPrizes) {
  std::ifstream file(fileName.c_str());
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << fileName << std::endl;
    exit(1);
  }

  std::string line;
  while (true) {
    std::getline(file, line);
    // if (file.eof()) break;

    // Skip the header in graph file.
    if (line.at(0) == '#') continue;

    // Get the number of nodes in Graph.
    if (isdigit(line[0])) {
      _numNodes = atoi(line.c_str());
      break;
    }
  }

  int currentLine = 0;
  while (currentLine < _numNodes) {
    // read the line and write to members of graph.
    std::getline(file, line);
    vector<string> tokens;
    while (true) {
      int pos = line.find("\t");
      if (pos == string::npos) {
        tokens.push_back(line);
        break;
      }
      tokens.push_back(line.substr(0, pos));
      line = line.substr(pos + 1);
    }

    // write tokens to Graph member variables.
    _nodeNames.push_back(tokens[1]);
    string geoLoc = tokens[2].substr(1, tokens[2].size() - 2);
    int sep = geoLoc.find(",");
    double lat = std::stof(geoLoc.substr(0, sep));
    double lon = std::stof(geoLoc.substr(sep + 1, string::npos));
    const tuple<double, double> geoL(lat, lon);
    _geoLocations.push_back(geoL);
    string release = tokens[3];
    _releases.push_back(atoi(release.c_str()));
    string deadline = tokens[4];
    _deadlines.push_back(atoi(deadline.c_str()));
    string duration = tokens[5];
    _durations.push_back(atoi(duration.c_str()));
    // either set unit prizes or read prizes from file.
    if (unitPrizes) {
      if (currentLine == 0) {
        _prizes.push_back(0);
      } else {
        _prizes.push_back(1);
        }
    } else {
      string prize = tokens[6];
      _prizes.push_back(atoi(prize.c_str()));
      }

    currentLine++;
  }

  // Read the distance matrix.
  while (true) {
    std::getline(file, line);
    if ((file.eof()) || line.at(0) == '#') break;
    vector<double> row;
    while (true) {
      int pos = line.find(" ");
      if (pos == string::npos) {
        row.push_back(atof(line.substr(0, pos).c_str()));
        break;
      }
      row.push_back(atof(line.substr(0, pos).c_str()));
      line = line.substr(pos + 1);
    }
    _distances.push_back(row);
  }
}

// ____________________________________________________________________________
Graph::~Graph() {
}
