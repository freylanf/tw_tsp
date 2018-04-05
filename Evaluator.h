// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#ifndef EVALUATOR_H_
#define EVALUATOR_H_

#include <string>
#include <vector>
#include "./Graph.h"
using std::string;

// Class that reads graphs from a folder, solves the graphs
// with FPT and MLIP solvers and writes runtime results and
// paths to a file.

class Evaluator {
 public:
  // Function to evaluate all graphs with MLIP and FPT solver.
  void evaluate(const string& inPath, bool unitPrizes = false);

  // Function that writes the solutions paths and runtimes to
  // files.
  void writeResults(const string& outPath, bool unitPrizes = false);

 private:
  size_t _instSize;
  vector<Graph> _graphs;
  vector<double> _fptRuntimes;
  vector<double> _mlipRuntimes;
  vector<size_t> _fptPrizes;
  vector<size_t> _mlipPrizes;
  vector<vector<Location>> _fptPaths;
  vector<vector<Location>> _mlipPaths;
};

#endif  // EVALUATOR_H_
