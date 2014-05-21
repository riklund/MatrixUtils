#ifndef FileMatrix_hh
#define FileMatrix_hh 1

#include <iostream>
#include <vector>
#include "RLException.hh"
#include "Globals.hpp"
#include "RLMacros.hpp"
#include "VerbosePrinter.hh"
#include "VerbosePrinterEventEnabled.hh"

using namespace std;
#define FILE_VALIDATION_NUMBER_RL2 963258741


class FileMatrix 
  : public VerbosePrinterEventEnabled
{
public:
  FileMatrix(const char * fileName
			 );
  ~FileMatrix();
  void Multiply(const ComplexDouble * input, 
				ComplexDouble * output
				);
  size_t GetDim() const;

  void GetDiagonal(vector<ComplexDouble> & output
				   );
  
private:
  void ValidateFile();

  FILE * file;
  size_t  dim;
  ComplexDouble * work;


};

#endif
