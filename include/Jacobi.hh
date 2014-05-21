#ifndef Jacobi_hh
#define Jacobi_hh 1

#include <complex>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include "Globals.hpp"
#include "RLException.hh"
#include "JacobiUtils.hh"
#include "FileMatrix.hh"
#include "LapackeEigenvalueSolver.hh"
#include "EigenPair.hh"
#include "EigenInformation.hh"
using namespace std;


#define FILE_VALIDATION_NUMBER_RL2 963258741

#define MAX_ITERATIONS 300
#define TOLERANCE (1.0E-10)

int main(int argc, 
		 char ** argv
		 );


#endif
