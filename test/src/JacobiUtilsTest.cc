#include "JacobiUtilsTest.hh"

bool JacobiUtilsTest::TestExpandM() const
{
  vector<vector<ComplexDouble> > myMatrix;
  for(size_t i = 0; i<10; ++i)
	{
	  ExpandM(myMatrix);
	  if(myMatrix.size() != i+1)
		{
		  return false;
		}
	  for(size_t j = 0; j<myMatrix.at(i).size(); ++j)
		{
		  if(myMatrix.at(j).size() != i+1)
			return false;
		}
	
	}
  return true;
}

bool JacobiUtilsTest::TestOrthogonalize() const
{
  vector<vector<ComplexDouble> > searchSpace(2, vector<ComplexDouble>(3, 0.0));
  searchSpace[0][0] = 1.0;
  searchSpace[1][2] = 1.0;

  vector<ComplexDouble > toTest(3, 0.0);
  toTest[1] = ComplexDouble(0, 1);
  toTest[2] = ComplexDouble(1, 1);
  Orthogonalize(searchSpace, toTest);
  if(toTest.size() != 3)
	return false;


  if(!DBL_EQUAL(toTest[0], 0.0))
	return false;
  if(!DBL_EQUAL(toTest[1], 1.0))
	return false;
  if(!DBL_EQUAL(toTest[2], 0.0))
	return false;
  
  return true;
}

bool JacobiUtilsTest::TestVNorm() const
{
  vector<ComplexDouble> normalizable;
  ComplexDouble sqSum = 0.0;
  for(size_t i = 0; i<10; ++i)
	{
	  sqSum += ComplexDouble(i,i)*ComplexDouble(i, i);
	  normalizable.push_back(ComplexDouble(i, i));
	}
  if(!DBL_EQUAL(abs(VNorm(normalizable)), abs(sqrt(sqSum))))
	return false;
  return true;
}


bool JacobiUtilsTest::TestMaxIndex() const
{
  EigenInformation myInformation;
  myInformation.EigenPairs.resize(8);
  for(size_t i = 0; i<8; ++i)
	{
	  myInformation.EigenPairs[i].Eigenvalue = ComplexDouble(i, i);
	  myInformation.EigenPairs[i].Eigenvector.push_back(ComplexDouble(i, i));
	}
  if(MaxIndex(&myInformation) != 7)
	return false;
  return true;
}

bool JacobiUtilsTest::TestVectorProduct() const
{
  vector<ComplexDouble> v1(10, 1.0);
  vector<ComplexDouble> v2(10, 2.0);
  v1[5] = ComplexDouble(1,50);
  if(!DBL_EQUAL(VectorProduct(v1, v2), ComplexDouble(20, 100)))
	return false;
  return true;
}

bool JacobiUtilsTest::TestMatrixVectorProduct1() const
{
  vector<vector<ComplexDouble> > IdMatrix(10, vector<ComplexDouble>(10, 0.0));
  vector<ComplexDouble> vect(10, 0.0);;
  for(size_t i = 0; i<10; ++i)
	{
	  vect[i] = ComplexDouble(i, 2*i);
	  for(size_t j = 0; j<10; ++j)
		{
		  IdMatrix[i][j] = 2*(i==j);
		}
	}
  vector<ComplexDouble> output;
  MatrixVectorProduct(IdMatrix, vect, output);
  if(output.size() != 10)
	return false;
  for(size_t i = 0; i<10; ++i)
	{
	  if(!DBL_EQUAL(output[i], 2.0*vect[i]))
		return false;
	}
  return true;
}

bool JacobiUtilsTest::TestMatrixVectorProduct2() const
{
  vector<vector<ComplexDouble> > Matrix(1, vector<ComplexDouble>(10, 0.0));
  Matrix.at(0).at(5) = 5.0;
  vector<ComplexDouble> vect(1, 2.0);
  vector<ComplexDouble> output;

  MatrixVectorProduct(Matrix, vect, output);
  if(output.size() != 10)
	return false;
  for(size_t i = 0; i<10; ++i)
	{
	  if(!DBL_EQUAL(output[i], ((i==5)?10.0:0.0)))
		return false;
	}
  return true;
}


bool JacobiUtilsTest::TestConstructM() const
{
  vector<vector<ComplexDouble> > M(10, vector<ComplexDouble>(10, 0.0));
  vector<vector<ComplexDouble> > W(10, vector<ComplexDouble>(10, 1.0));
  vector<vector<ComplexDouble> > searchSpace(10, vector<ComplexDouble>(10, 1.0));
  ConstructM(searchSpace, W, M);
  if(M.size() != 10)
	return false;
  for(size_t i = 0; i<10; ++i)
	{
	  if(!DBL_EQUAL(M[i][9], 10.0))
		return false;
	  if(!DBL_EQUAL(M[9][i], 10.0))
		return false;
	}
  return true;
}

bool JacobiUtilsTest::TestDavidsonCorrection() const
{
  vector<ComplexDouble> diagonal;
  diagonal.push_back(0.814723686393179); diagonal.push_back(0.097540404999410);
  diagonal.push_back(0.157613081677548); diagonal.push_back(0.141886338627215);

  ComplexDouble eigenVal = -0.034553097766873;

  vector<ComplexDouble> res;
  res.push_back(0.1); res.push_back(0.2); res.push_back(0.3); res.push_back(0.4);

  vector<ComplexDouble> output;
  GetDavidsonCorrection(diagonal, eigenVal, res, output);
  vector<ComplexDouble> expected;




  expected.push_back(0.117747243142766);
  expected.push_back(1.514079010788793);
  expected.push_back(1.561148797709049);
  expected.push_back(2.267066865406297);
  for(size_t i = 0; i<4; ++i)
	{
	  if(!DBL_EQUAL(output.at(i),expected.at(i)))
		return false;
	}
  return true;
}

int JacobiUtilsTest::RunTests() const
{
  if(!TestExpandM())
	return 1;
  if(!TestOrthogonalize())
	return 2;
  if(!TestVNorm())
	return 3;
  if(!TestMaxIndex())
	return 4;
  if(!TestVectorProduct())
	return 5;
  if(!TestMatrixVectorProduct1())
	return 6;
  if(!TestMatrixVectorProduct2())
	return 8;
  if(!TestConstructM())
	return 7;
  if(!TestDavidsonCorrection())
	return 9;


  return 0; ///Return 0 for correct behavior.
}


string JacobiUtilsTest::ToString() const
{
  return "JacobiUtils";
}
