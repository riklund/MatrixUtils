#include "Purify.hh"

int main(int argc, char ** argv)
{
  if(argc != 4)
	{
	  cerr << "Usage: " << argv[0] << " matrix_g=0 matrix_g=1 output" << endl;
	  return 1;
	}
  FILE * matrix0 = fopen(argv[1], "rb");
  FILE * matrix1 = fopen(argv[2], "rb");
  FILE * output = fopen(argv[3], "wb");
  if(matrix0 == NULL)
	{
	  cerr << "Could not open matrix_g=0 file." << endl;
	  return 2;
	}
  if(matrix1 == NULL)
	{
	  cerr << "Could not open matrix_g=1 file." << endl;
	  return 2;
	}
  if(output == NULL)
	{
	  cerr << "Could not open output file." << endl;
	  return 2;
	}
  uint dim0, dim1;
  if(fread(&dim0, sizeof(dim0), 1, matrix0) != 1)
	{
	  cerr << "Could not read dimension from matrix0." << endl;
	  return 3;
	}
  if(fread(&dim1, sizeof(dim1), 1, matrix1) != 1)
	{
	  cerr << "Could not read dimension from matrix1." << endl;
	  return 3;
	}
  if(dim0 != dim1)
	{
	  cerr << "Dimension mismatch between input matrices: " << dim0 << " " << dim1 << endl;
	  return 3;
	}

  if(fwrite(&dim0, sizeof(dim0), 1, output) != 1)
	{
	  cerr << "Could not write matrix dimension to output matrix." << endl;
	  return 4;
	}

  valarray<ComplexDouble> arr0(dim0);
  valarray<ComplexDouble> arr1(dim0);
  valarray<ComplexDouble> coupling(dim0);
  vector<ComplexDouble> diag(dim0);


  for(size_t n = 0; n<dim0; ++n)
	{
	  if(fread(&arr0[0], sizeof(ComplexDouble), dim0, matrix0) != dim0)
		{
		  cerr << "Failed read from matrix0." << endl;
		  return 4;
		}
	  if(fread(&arr1[0], sizeof(ComplexDouble), dim1, matrix1) != dim0)
		{
		  cerr << "Failed read from matrix1." << endl;
		  return 5;
		}
	  coupling = arr1 -arr0;
	  diag.at(n) = arr0[n];
	  for(size_t i = 0; i<n; ++i)
		{
		  if(n==i)
			continue;
		  if(abs(arr0[i]) > EPS)
			{
			  cerr << "Large off-diagonal element detected in matrix0: " << n << " " << i << arr0[i] << endl;
			  return 6;
			}
		}
	  
	  if(fwrite(&coupling[0], sizeof(ComplexDouble), dim0, output) != dim0)
		{
		  cerr << "Failed write to output file." << endl;
		  return 7;
		}
	}
  if(fwrite(&diag[0], sizeof(ComplexDouble), dim0, output) != dim0)
	{
	  cerr << "Failed write of diag to output file." << endl;
	  return 8;
	}
  size_t fileValNo = FILE_VALIDATION_NUMBER_RL2;
  if(fwrite(&fileValNo, sizeof(fileValNo), 1, output) != 1)
	{
	  cerr << "Could not write new file validation number to file." << endl;
	  return 9;
	}

  return 0;
}
