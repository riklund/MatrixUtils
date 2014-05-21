#include "ConstructMatrix.hh"

int main(int argc, char ** argv)
{
  if(argc != 4)
	{
	  cerr << "Usage: " << argv[0] << " pObject outputMatrix couplingCoefficient" << endl;
	  return 1;
	}
  double g = atof(argv[3]);


  FILE * input = fopen(argv[1], "rb");
  if( input == NULL)
	{
	  cerr << "Input file could not be opened" << endl;
	  return 1;
	}
  FILE * output = fopen(argv[2], "wb"); 
  if( output == NULL)
	{
	  cerr << "Output file could not be opened" << endl;
	  return 2;
	}
  size_t dim;

  if(fread(&dim, sizeof(dim), 1, input) != 1)
	{
	  cerr << "Could not read dimension from input." << endl;
	  return 3;
	}

  if(fwrite(&dim, sizeof(dim), 1, output) != 1)
	{
	  cerr << "Could not write matrix dimension to output." << endl;
	  return 4;
	}
  vector<ComplexDouble> diagonal(dim);
  vector<ComplexDouble> row(dim);

  if(fread(&diagonal[0], sizeof(ComplexDouble), dim, input) != dim)
	{
	  cerr << "Failed to load diagonal from file." << endl;
	  return 5;
	}

  for(size_t n = 0; n<dim; ++n)
	{
	  if(fread(&row[0], sizeof(ComplexDouble), dim, input) != dim)
		{
		  cerr << "Failed to load row " << n << " from input file." << endl;
		  return 5;
		}
	  for(size_t j = 0; j<dim; ++j)
		{
		  row[j] *= g;
		}
	  row[n] += diagonal[n];
	  if(fwrite(&row[0], sizeof(ComplexDouble), dim, output) != dim)
		{
		  cerr << "Failed write to output file." << endl;
		  return 7;
		}
	  if(n%100==0)
		printf("Processing main part, loop %ld / %ld.\n", n, dim);
	}
  if(!ValidateMatrix(input))
	{
	  return 8;
	}

  size_t fileValNo = FILE_VALIDATION_NUMBER_RL2;
  if(fwrite(&fileValNo, sizeof(fileValNo), 1, output) != 1)
	{
	  cerr << "Could not write file validation number to file." << endl;
	  return 9;
	}

  
  fclose(input);
  fclose(output);
  return 0;
}



bool ValidateMatrix(FILE * file)
{
  size_t oldValidateNo;
  if(fread(&oldValidateNo, sizeof(oldValidateNo), 1, file) != 1)
	{
	  cerr << "Could not read old file validation number." << endl;
	  return false;
	}
  if(oldValidateNo != FILE_VALIDATION_NUMBER_RL2)
	{	  
	  cerr << "Old file consistency validateion failed." << endl;
	  return false;
	}
  return true;
}
