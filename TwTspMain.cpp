// Copyright 2018
// Author: Felix Freyland <felix.freyland@gmx.de>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "FptSolver.h"
#include "MlipSolver.h"
#include "Evaluator.h"

using std::string;
using std::get;
using std::setw;
using std::endl;
using std::ofstream;
using std::cout;

// Takes a path to a folder with .graph files and an outpath for results,
// calculates the optimal tours for all data with FPT and MLIP solver
// and writes runtimes and paths textfiles.
int main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "Usage1: ./TwTspMain <read_path> <write_path>\n");
    fprintf(stderr, "Usage2: ./TwTspMain <read_path> <write_path> --UP\n");
    fprintf(stderr, "Usage2 calculates tours with unit prizes for locations\n");
    exit(1);
  } else if (argc == 3) {
    Evaluator ev;
    string inPath = argv[1];
    string outPath = argv[2];
    ev.evaluate(inPath);
    ev.writeResults(outPath);
  } else {
    string unitPrize = argv[3];
    if (unitPrize == "--UP") {
    Evaluator ev;
    string inPath = argv[1];
    string outPath = argv[2];
    ev.evaluate(inPath, true);
    ev.writeResults(outPath, true);
    } else {
      fprintf(stderr, "%s is not a valid cammand line argument\n", argv[3]);
      fprintf(stderr, "Use --UP for non unit prizes\n");
      exit(1);
    }
  }
  return 0;
}
