#ifndef LapackeEigenvalueSolver_hh
#define LapackeEigenvalueSolver_hh 1

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "Matrix.hpp"
#include "RLException.hh"
#include "CommandLineArgument.hh"
#include "CommandLineInterpreter.hh"
#include "EigenInformation.hh"
#include "MKL_options.hpp"

typedef Matrix<ComplexDouble> CMatrix;


using namespace std;

/**Eigenvalue solver using LAPACKe
 */
class LapackeEigenvalueSolver
{
public:
  static EigenInformation * Solve(CMatrix * toSolve,
								  bool ComputeEigenvectors = true
								  );

protected:
  static void RescaleEigenvectors(EigenInformation * eigenData
								  );


private:
  LapackeEigenvalueSolver() {}
};


#endif
