//
// Created by Felix Freyland on 25.03.18.
// E-mail: felixfreyland@gmx.de
//

#include <fstream>
#include <iostream>
#include <iomanip>
#include "FptSolver.h"

using namespace std;
// Takes a .graph data file as input and calculates the
// optimal tour.

int main(int argc, char *argv[]) {

  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage 1: ./TwTspMain <filename>\n");
    fprintf(stderr,"Usage 2: -/TwTspMain <filename> -up");
    exit(1);
  }
  bool unitPrizes = false;
  if (string(argv[2]) == "-up") {
    unitPrizes = true;
  }
  // Build graph and solver.
  Graph graph;
  graph.buildFromFile(string(argv[1]), unitPrizes);
  FptSolver solver = FptSolver(graph);
  auto solution = solver.solve();
  auto maxPrize = get<0>(solution);
  auto tourEnd = get<1>(solution);
  auto path = solver.getTour(tourEnd);

  string outFile = "result" + string(argv[1]);
  ofstream tourFile (outFile);
  if (tourFile.is_open())
  {
    tourFile << "Optimal Tour: \n";
    tourFile << setw(6) << "LocId " << "|" << setw(6) << "Prize " << "|"
             << setw(10) << "Arrival " << "|" << setw(10) << "Leave " << "|"
             << setw(10) << "Lat. " << "|" << setw(10) << "Longitude " << "|"
             << setw(10) << "Address " << endl << endl;
    for (auto loc : path) {
      tourFile << setw(6) << loc.id << "|" << setw(6) << loc.prize << "|"
               << setw(10) << loc.arrival << "|" << setw(10) << loc.leave << "|"
               << setw(10) << loc.latitude << "|" << setw(10) << loc.longitude << "|"
               << setw(10) << loc.name << endl;
    }
    tourFile <<endl << "Total Prize: " << maxPrize << "\t"
             << "Total Length: " << path.size() << endl;
    tourFile.close();
  }
  else cout << "Unable to open file";
  return 0;
}