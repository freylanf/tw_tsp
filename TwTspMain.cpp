// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "FptSolver.h"
#include "MlipSolver.h"

using std::string;
using std::get;
using std::setw;
using std::endl;
using std::ofstream;
using std::cout;

// Takes a .graph data file as input and calculates the
// optimal tour.
int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage 1: ./TwTspMain <filename> <SolverType> [-up]\n");
    exit(1);
  }
  bool unitPrizes = false;
  if (argc == 4 && string(argv[3]) == "-up") {
    unitPrizes = true;
  }
  // Build graph and solver.
  Graph graph;
  graph.buildFromFile(string(argv[1]), unitPrizes);
  vector<Location> path;
  size_t maxPrize;
  if (string(argv[2]) == "MLIP") {
    MlipSolver solver = MlipSolver(graph);
    auto solution = solver.solve();
    std::cout << "MaxValue: " <<  get<0>(solution) << endl;
    maxPrize = get<0>(solution);
    path = get<1>(solution);
  } else if (string(argv[2]) == "FPT") {
      FptSolver solver = FptSolver(graph);
      auto solution = solver.solve();
      maxPrize = get<0>(solution);
      auto tourEnd = get<1>(solution);
      path = solver.getTour(tourEnd);
  } else {
    cout << "Provide MLIP or FPT as command line Argument for solver type";
    exit(1);
  }

  string outFile = string(argv[2]) + "_result" + string(argv[1]);
  ofstream tourFile(outFile);
  if (tourFile.is_open()) {
    tourFile << "Optimal Tour: \n";
    tourFile << setw(6) << "LocId " << "|" << setw(6) << "Prize " << "|"
             << setw(10) << "Arrival " << "|" << setw(10) << "Leave " << "|"
             << setw(10) << "Lat. " << "|" << setw(10)
             << "Longitude " << "|"
             << setw(10) << "Address " << endl << endl;
    for (auto loc : path) {
      tourFile << setw(6) << loc.id << "|" << setw(6) << loc.prize << "|"
               << setw(10) << loc.arrival << "|" << setw(10)
               << loc.leave << "|"
               << setw(10) << loc.latitude << "|"
               << setw(10) << loc.longitude << "|"
               << setw(10) << loc.name << endl;
    }

    tourFile << endl << "Total Prize: " << maxPrize << "\t"
             << "Total Length: " << path.size() << endl;
    tourFile.close();
  } else {
    cout << "Unable to open file";
    }
  return 0;
}
