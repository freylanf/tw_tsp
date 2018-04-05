// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include "./Evaluator.h"
#include <time.h>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include "./MlipSolver.h"
#include "./FptSolver.h"

using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using boost::filesystem::initial_path;
using::boost::filesystem::create_directory;
using std::string;
using std::copy;
using std::back_inserter;
using std::sort;
using std::setw;
using std::ofstream;

// ____________________________________________________________________________
void Evaluator::evaluate(const string& inPath, bool unitPrize) {
  // reading all graphs in the folder and adding it to graph list.
  path graphPath(initial_path() / inPath);
  if (!exists(graphPath)) {
    std::cout << "\nNot found: " << graphPath.string() << std::endl;
    exit(1);
  }
  typedef vector<path> vec;
  vec v;
  if (is_directory(graphPath)) {
    copy(directory_iterator(graphPath), directory_iterator(), back_inserter(v));
    std::sort(v.begin(), v.end());
    for (vec::const_iterator it(v.begin()); it != v.end(); ++it) {
      Graph g;
      g.buildFromFile((*it).string(), unitPrize);
      _graphs.push_back(g);
    }
  }
  _instSize = _graphs[0].getNodesNum() - 1;

  // Now solving all the graphs with both solver types and store
  // runtimes and found paths.
  for (auto graph : _graphs) {
    MlipSolver m = MlipSolver(graph);
    FptSolver f = FptSolver(graph);
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
    auto resultFpt = f.solve();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec) * 1000;
    elapsed += (finish.tv_nsec - start.tv_nsec) / (1000 * 1000);
    _fptPaths.push_back(f.getTour(std::get<1>(resultFpt)));
    _fptRuntimes.push_back(elapsed);
    _fptPrizes.push_back(std::get<0>(resultFpt));

    clock_gettime(CLOCK_MONOTONIC, &start);
    auto resultMlip = m.solve();
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec) * 1000;
    elapsed += (finish.tv_nsec - start.tv_nsec) / (1000 * 1000);
    _mlipPaths.push_back(m.getTour());
    _mlipRuntimes.push_back(elapsed);
    _mlipPrizes.push_back(resultMlip);
  }
}

// ____________________________________________________________________________
void Evaluator::writeResults(const string& outPath, bool unitPrizes) {
  path resultPath(initial_path() / outPath);
  if (!exists(resultPath)) {
    create_directory(resultPath);
  }
  string uP = "_";
  if (unitPrizes) {
    uP = "_UP_";
  }
  string instSize = std::to_string(_instSize);

  // Writing the runtimes file.
  string outFile = resultPath.string() + "/" + instSize + uP + "runtimes.txt";
  ofstream runTimesFile(outFile);
  if (runTimesFile.is_open()) {
    runTimesFile << "Runtimes and Prizes for Instances of size " << instSize
                 <<std::endl
                 << setw(4) << "Id " << "|"
                 << setw(11) << "FPT_Prize " << "|"
                 << setw(12) << "MLIP_Prize " << "|"
                 << setw(11) << "FPT msec. " << "|"
                 << setw(12) << "MLIP msec. " << "|"
                 << std::endl;
    size_t idx = 0;
    while (true) {
      if (idx == _fptRuntimes.size()) { break; }
      runTimesFile << setw(2) << idx  << setw(3) << "|"
                   << setw(5) << _fptPrizes[idx] << setw(7) << "|"
                   << setw(6) << _mlipPrizes[idx] << setw(7) << "|"
                   << setw(5) << _fptRuntimes[idx] << setw(7) << "|"
                   << setw(6) << _mlipRuntimes[idx] << setw(7) << "|"
                   << std::endl;
      idx++;
    }
  }
  // Writing paths to file.
  string toursOut = resultPath.string() + "/" + instSize + "_FPT"
                    + uP + "paths.txt";
  ofstream fptTours(toursOut);
  if (fptTours.is_open()) {
    fptTours << "Optimal Tours for FPT solved instances of size: " << instSize
             << std::endl << std::endl;
    size_t idx = 0;
    for (auto path : _fptPaths) {
      fptTours << "Path for instance " << idx << std::endl;
      fptTours << setw(7) << "LocId " << "|"
               << setw(7) << "Prize " << "|"
               << setw(11) << "Arrival  " << "|"
               << setw(11) << "Leave   " << "|"
               << "     Location" << std::endl;
      for (auto loc : path) {
        fptTours << setw(4) << loc.id << setw(4) << "|"
                 << setw(4) << loc.prize << setw(4) << "|"
                 << setw(6) << loc.arrival << setw(6) << "|"
                 << setw(6) << loc.leave <<  setw(6) << "|"
                 << "     " << loc.name << std::endl;
      }
      fptTours << std::endl;
      idx++;
    }
    fptTours.close();
  }

  toursOut = resultPath.string() + "/" + instSize + "_MLIP"
             + uP + "paths.txt";
  ofstream mlipTours(toursOut);
  if (mlipTours.is_open()) {
    mlipTours << "Optimal Tours for MLIP solved instances of size: " << instSize
              << std::endl << std::endl;
    size_t idx = 0;
    for (auto path : _mlipPaths) {
      mlipTours << "Path for instance " << idx << std::endl;
      mlipTours << setw(7) << "LocId " << "|"
                << setw(7) << "Prize " << "|"
                << setw(11) << "Arrival  " << "|"
                << setw(11) << "Leave   " << "|"
                << "     Location" << std::endl;
      for (auto loc : path) {
        mlipTours << setw(4) << loc.id << setw(4) << "|"
                  << setw(4) << loc.prize << setw(4) << "|"
                  << setw(6) << loc.arrival << setw(6) << "|"
                  << setw(6) << loc.leave <<  setw(6) << "|"
                  << "     " << loc.name << std::endl;
      }
      idx++;
      mlipTours << std::endl;
    }
    mlipTours.close();
  }
}
