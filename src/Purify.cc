#include "Purify.hh"

int main(int argc, char ** argv)
{
  if(argc != 4 && argc != 5)
	{
	  cerr << "Usage: " << argv[0] << " matrix_g=0 matrix_g=1 output [adj_factor]" << endl;
	  cerr << "adj_factor = g_1 - g_0 if not equal to one" << endl;
	  return 1;
	}
  double adj_factor = 1;
  if(argc == 5)
	adj_factor = atof(argv[4]);


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

  size_t dim = dim0;


  if(fwrite(&dim, sizeof(dim), 1, output) != 1)
	{
	  cerr << "Could not write matrix dimension to output matrix." << endl;
	  return 4;
	}

  valarray<ComplexDouble> arr0(dim);
  valarray<ComplexDouble> arr1(dim);
  valarray<ComplexDouble> coupling(dim);
  vector<ComplexDouble> diag(dim, 0.0);

  fpos_t diagonalPos;
  if(fgetpos(output, &diagonalPos))
	{
	  cerr << "Failed to lock on to diagonal position in output file." << endl;
	  return 44;
	}

  if(fwrite(&diag[0], sizeof(ComplexDouble), dim, output) != dim)
	{
	  cerr << "Failed to preallocate diagonal." << endl;
	}


  for(size_t n = 0; n<dim; ++n)
	{
	  if(n%100==0)
		printf("Processing main part, loop %ld / %ld.\n", n, dim);
	  if(fread(&arr0[0], sizeof(ComplexDouble), dim, matrix0) != dim)
		{
		  cerr << "Failed read from matrix0." << endl;
		  return 4;
		}
	  if(fread(&arr1[0], sizeof(ComplexDouble), dim, matrix1) != dim)
		{
		  cerr << "Failed read from matrix1." << endl;
		  return 5;
		}
	  coupling = (arr1 -arr0) / adj_factor;
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
	  
	  if(fwrite(&coupling[0], sizeof(ComplexDouble), dim, output) != dim)
		{
		  cerr << "Failed write to output file." << endl;
		  return 7;
		}
	}
  
  //skip uninteresting info.
  fseek(matrix0, 2*sizeof(int), SEEK_CUR);
  fseek(matrix1, 2*sizeof(int), SEEK_CUR);

  if(!ValidateMatrixPurify(matrix0) || !ValidateMatrixPurify(matrix1))
	{
	  return 42;
	}

  size_t fileValNo = FILE_VALIDATION_NUMBER_RL2;
  if(fwrite(&fileValNo, sizeof(fileValNo), 1, output) != 1)
	{
	  cerr << "Could not write new file validation number to file." << endl;
	  return 9;
	}

  if(fsetpos(output, &diagonalPos))
	{
	  cerr << "Failed to rewind file to write diagonal." << endl;
	  return 10;
	}
  if(fwrite(&diag[0], sizeof(ComplexDouble), dim, output) != dim)
	{
	  cerr << "Failed write of diag to output file." << endl;
	  return 8;
	}

  fclose(matrix0);
  fclose(matrix1);
  fclose(output);


  return 0;
}


bool ValidateMatrixPurify(FILE * file)
{
  unsigned long oldValidateNo;
  if(fread(&oldValidateNo, sizeof(oldValidateNo), 1, file) != 1)
	{
	  cerr << "Could not read old file validation number." << endl;
	  return false;
	}
  if(oldValidateNo != FILE_VALIDATION_NUMBER_OLD)
	{	  
	  cerr << "Old file consistency validateion failed." << endl;
	  return false;
	}
  return true;
}
