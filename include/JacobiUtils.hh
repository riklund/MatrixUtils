#ifndef JacobiUtils_hh
#define JacobiUtils_hh 1

#include <complex>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "Globals.hpp"
#include "RLException.hh"
#include "JacobiUtils.hh"
#include "EigenInformation.hh"

#define MAX_N_ITER 20
#define MYEPS (1E-12)


void ExpandM(vector<vector<ComplexDouble> > & M
			); ///Adds an extra row and an extra column to the matrix M, and fills them with zeros.

void Orthogonalize(const vector<vector<ComplexDouble> > & searchSpace, 
				   vector<ComplexDouble> & davidsonCorrection
				   ); ///Orthogonalizes davidsonCorrection against searchSpace.

void GetDavidsonCorrection(const vector<ComplexDouble> & myDiagonal, 
						   const ComplexDouble & eigenvalue, 
						   const vector<ComplexDouble> & res, 
						   vector<ComplexDouble> & output
						   ); ///Computes the Davidson correction.

void Normalize(vector<ComplexDouble> & toNormalize
			   );

ComplexDouble VNorm(const vector<ComplexDouble> & v
			 ); ///Norm of  the input vector.

size_t MaxIndex(const EigenInformation * myInformation
				); ///The index of the largest eigenvalue.


ComplexDouble VectorProduct(const vector<ComplexDouble> & v1, 
							const vector<ComplexDouble> & v2
							); ///Vector product between two vectors.


void MatrixVectorProduct(const vector<vector<ComplexDouble> > & matrix, 
						 const vector<ComplexDouble> & vect, 
						 vector<ComplexDouble> & output
						 );


void ConstructM(const vector<vector<ComplexDouble> > & searchSpace, 
				const vector<vector<ComplexDouble> > & W, 
				vector<vector<ComplexDouble> > & M
				); ///Fills in the last row and column in M by doing a partial matrix-matric multiplication of searchSpace and W.


#endif
