#include "JacobiUtils.hh"


void ExpandM(vector<vector<ComplexDouble> > & M)
{
  for(vector<vector<ComplexDouble> >::iterator it = M.begin(); it!=M.end(); ++it)
	{
	  it->push_back(0.0);
	}
  M.push_back(vector<ComplexDouble>((M.size() > 0)?(M.back().size()):(1), 0.0));
}

void Orthogonalize(const vector<vector<ComplexDouble> > & V, vector<ComplexDouble> & t)
{
  for(size_t i = 0; i<V.size(); ++i)
	{
	  double ratio = 0.0;
	  while(ratio < 0.25)
		{
		  if(t.size() != V.at(i).size())
			throw RLException("d-corr s-Space mismatch at %d : %d vs %d.", i, t.size(), V.size());
		  
		  ComplexDouble coeff = VectorProduct(V.at(i), t);
		  for(size_t k = 0; k<t.size(); ++k)
			{
			  t.at(k) = t.at(k) - coeff * V.at(i).at(k);
			}
		  ratio = abs(VectorProduct(V.at(i), t)/coeff);
		}
	}

  Normalize(t);
}


void GetDavidsonCorrection(const vector<ComplexDouble> & myDiagonal, const ComplexDouble & eigenvalue, const vector<ComplexDouble> & res, vector<ComplexDouble> & output)
{
  output.resize(myDiagonal.size(), 0.0);
  for(size_t i = 0; i<myDiagonal.size(); ++i)
	{
	  ComplexDouble denom = myDiagonal[i] - eigenvalue;
	  if(abs(denom) < MYEPS)
		{
		  denom = -MYEPS;
		}
	  output[i] = res[i] / denom ;
	}
}

ComplexDouble VNorm(const vector<ComplexDouble> & v)
{
  return sqrt(VectorProduct(v, v));
}

size_t MaxIndex(const EigenInformation * myInformation)
{
  size_t maxIndex = 0;
  double maxValue = 0;
  for(size_t i = 0; i<myInformation->EigenPairs.size(); ++i)
	{
	  if(abs(myInformation->EigenPairs[i].Eigenvector.at(0)) > maxValue)
		{
		  maxValue = abs(myInformation->EigenPairs[i].Eigenvector.at(0));
		  maxIndex = i;
		}
	}
  return maxIndex;
}




ComplexDouble VectorProduct(const vector<ComplexDouble> & v1, const vector<ComplexDouble> & v2)
{
  ComplexDouble toReturn = 0.0;
  if(v1.size() != v2.size())
	{
	  throw RLException("Incompatible vector sizes for vector product: %d and %d", v1.size(), v2.size());
	}
  for(size_t i = 0; i<v1.size(); ++i)
	{
	  toReturn += v1[i] * v2[i];
	}
  return toReturn;
}

void MatrixVectorProduct(const vector<vector<ComplexDouble> > & matrix, const vector<ComplexDouble> & vect, vector<ComplexDouble> & output)
{
  if(matrix.empty())
	{
	  throw RLException("Empty matrix not allowed.");
	}
  if(output.size() != matrix.at(0).size())
	output.resize(matrix.at(0).size());
  if(vect.size() != matrix.size())
	throw RLException("Matrix-vector dimension mismatch in product function.");

  fill(output.begin(), output.end(), 0.0);
  for(size_t i = 0; i<matrix.size(); ++i)
	{
	  if(matrix.at(i).size() != matrix.at(0).size())
		throw RLException("Internal matrix inconsistency in matrix-vector product function.");

	  for(size_t j = 0; j<matrix.at(i).size(); ++j)
		{
		  output[j] += matrix[i][j] * vect[i];
		}
	}
}


void ConstructM(const vector<vector<ComplexDouble> > & searchSpace, const vector<vector<ComplexDouble> > & W, vector<vector<ComplexDouble> > & M)
{
  if(W.size() != searchSpace.size())
	{
	  throw RLException("Unexpected W-searchSpace size mismatch.");
	}
  size_t k = M.size()-1;
  for(size_t m = 0; m<=k; ++m)
	{
	  M[m][k] = VectorProduct(searchSpace[m], W[k]);
	  M[k][m] = VectorProduct(searchSpace[k], W[m]);
#ifndef NO_STRICT_ASYMMETRY_CHECK
	  if(!DBL_EQUAL(M[m][k], M[k][m]))
		throw RLException("Unexpected  asymmetry: (%+13.10e %+13.10ei) != (%+13.10e %+13.10ei)", real(M[m][k]), imag(M[m][k]), real(M[k][m]), imag(M[k][m]));
#endif
	}
}


void Normalize(vector<ComplexDouble> & toNormalize)
{
  ComplexDouble norm = VNorm(toNormalize);
  for(vector<ComplexDouble>::iterator it = toNormalize.begin(); it!=toNormalize.end(); ++it)
	{
	  *it/=norm;
	}
}
