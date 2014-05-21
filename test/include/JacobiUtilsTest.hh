#ifndef JacobiUtilsTest_hh
#define JacobiUtilsTest_hh 1


///General inclusion statements.
#include <iostream>
#include <assert.h>
#include <map>
#include <vector>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <algorithm>
#include <complex>
#include "Globals.hpp"
#include "RLMacros.hpp"
#include "JacobiUtils.hh"

///This should always be included
#include "GenericUnitTest.hh"


using namespace std;


/**
   Test class.
 */
//It is imperative to have this on a single line before the inheritance line.
class JacobiUtilsTest 
  : public GenericUnitTest ///Always extend GenericUnitTest.
{
 public:
  int RunTests() const; ///Main function.
  string ToString() const; ///Should return a descriptive name of the function.
 protected:
  bool TestExpandM() const;
  bool TestOrthogonalize() const;
  bool TestVNorm() const;
  bool TestMaxIndex() const;
  bool TestVectorProduct() const;
  bool TestMatrixVectorProduct1() const;
  bool TestMatrixVectorProduct2() const;
  bool TestConstructM() const;
  bool TestDavidsonCorrection() const;
};
#endif
