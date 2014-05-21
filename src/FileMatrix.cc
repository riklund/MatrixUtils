#include "FileMatrix.hh"
FileMatrix::FileMatrix(const char * fileName)
{
  file = fopen(fileName, "rb");
  if(fread(&dim, sizeof(dim), 1, file) != 1)
	{
	  throw RLException("FileMatrix: Could not read dimension from file '%s'.", fileName);
	}
  work = new ComplexDouble[dim];
}

FileMatrix::~FileMatrix()
{
  fclose(file);
  file = NULL;
  delete work;
}

void FileMatrix::Multiply(const ComplexDouble * input, ComplexDouble * output)
{
  vPrint(3, "Performing matrix multiplication...");
  fseek(file, sizeof(size_t), SEEK_SET);
  for(size_t i = 0; i<dim; ++i)
	{
	  if(fread(work, sizeof(ComplexDouble), dim, file) != dim)
		{
		  throw RLException("Could not rad row %d from file.", i);
		}
	  output[i] = 0.0;
	  for(size_t j = 0; j<dim; ++j)
		{
		  output[i] += input[j] * work[j];
		}
	}
  ValidateFile();
  vPrint(3, "done.\n");
}

void FileMatrix::ValidateFile()
{
  size_t fileValNo;
  if(fread(&fileValNo, sizeof(fileValNo), 1, file) != 1)
	{
	  throw RLException("Could not read file validation number during multiply algorithm.");
	}
  if(fileValNo != FILE_VALIDATION_NUMBER_RL2)
	{
	  throw RLException("File validation number incorrect during matrix multiply.");
	}
}

void FileMatrix::GetDiagonal(vector<ComplexDouble> & output)
{
  vPrint(3, "Obtaining diagonal...");
  output.resize(dim, 0.0);
  fseek(file, sizeof(size_t), SEEK_SET);
  for(size_t i = 0; i<dim; ++i)
	{
	  if(fread(&output[i], sizeof(ComplexDouble), 1, file) != 1)
		{
		  throw RLException("Could not read row %d from file.", i);
		}
	  if(i + 1 < dim)
		fseek(file, dim*sizeof(ComplexDouble), SEEK_CUR);
	}
  ValidateFile();
  vPrint(3, "done.\n");
}


size_t FileMatrix::GetDim() const
{ 
  return dim; 
}
