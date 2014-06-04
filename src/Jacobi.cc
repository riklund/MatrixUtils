#include "Jacobi.hh"

int main(int argc, char ** argv)
{
  if(argc != 4)
	{
	  cerr << "Usage: " << argv[0] << " input_matrix guess output_vector" << endl;
	  return 1;
	}

  VerbosePrinter myPrinter(10);

  myPrinter.Print(2, "Setting up input matrix...");
  FileMatrix myMatrix(argv[1]);
  myMatrix.RegisterListener(&myPrinter);
  myPrinter.Print(2, "done.\n");
  vector<ComplexDouble> myDiagonal;
  myMatrix.GetDiagonal(myDiagonal);

  size_t dim = myMatrix.GetDim();
  size_t bIndex = atoi(argv[2]);
  
  vector<ComplexDouble> v1(dim, 0.0);
  v1.at(bIndex) = 1;

  ///Note: if v1 is not normalized, it should be normalized here.


  /// Matrix slicing note:
  /// The inner vectors are the _columns_ of the matrix.
  vector<vector<ComplexDouble> > V; //V-matrix.
  V.push_back(v1);


  vector<vector<ComplexDouble> > W;

  vector<ComplexDouble> w1(dim, 0.0);
  myMatrix.Multiply(&v1[0], &w1[0]);

  W.push_back(w1);




  vector<ComplexDouble> u(dim, 0.0);
  ComplexDouble theta = VectorProduct(v1, w1);


  vector<vector<ComplexDouble> > M;
  M.push_back(vector<ComplexDouble>(1, theta));




  for(size_t k = 0; k<MAX_ITERATIONS; ++k)
	{
	  myPrinter.Print(2, "Starting iteration %d (max %d), eigenvalue is %+13.10e %+13.10ei \n", k, MAX_ITERATIONS, real(theta), imag(theta));
	  ///Find eigenvalues and eigenvectors of M.
	  CMatrix * Mtemp = new CMatrix(M.size(), M.size());
	  for(size_t i = 0; i<M.size(); ++i)
		{
		  for(size_t j = 0; j<M.at(i).size(); ++j)
			{
			  Mtemp->Element(i, j) = M[i][j];
			}
		}

	  EigenInformation * myInformation = LapackeEigenvalueSolver::Solve(Mtemp);
	  
	  size_t idx = MaxIndex(myInformation);
	  myPrinter.Print(3, "Maximum index in eigenvector : %d.\n", idx);

	  delete Mtemp;
	  theta = myInformation->EigenPairs[idx].Eigenvalue;
	  vector<ComplexDouble> s = myInformation->EigenPairs[idx].Eigenvector;
	  delete myInformation;
	  
	  MatrixVectorProduct(V, s, u);
	  Normalize(u);

	  
	  vector<ComplexDouble> res; 
	  MatrixVectorProduct(W, s, res); 
	  
	  for(size_t i = 0; i < res.size(); ++i)
		{
		  res.at(i) -= theta * u.at(i);
		}

	  

	  double resNorm = abs(VNorm(res));

	  myPrinter.Print(3, "Res-norm: %+13.10e\n", resNorm);
	  if( resNorm < TOLERANCE)
		{
		  break;
		}

	  vector<ComplexDouble> t;
	  myPrinter.Print(4, "Computing Davidson correction...");
	  GetDavidsonCorrection(myDiagonal, theta, res, t );
	  myPrinter.Print(4, "done.\n");




	  myPrinter.Print(4, "Orthogonalizing...");
	  Orthogonalize(V, t);
	  myPrinter.Print(4, "done.\n");



	  V.push_back(t);


	  /*	  cout.precision(10);
	  cout << fixed;

	  for(size_t b = 0; b<V.at(0).size(); ++b)
		{
		  
		  for(size_t a = 0; a<V.size(); ++a)
			{
			  cout << real(V[a][b]) << "+"<< imag(V[a][b]) << "i    ";
			}
		  cout << endl;
		}
	  */




	  vector<ComplexDouble> nextW(dim, 0.0);
	  myMatrix.Multiply(&t[0], &nextW[0]);
	  W.push_back(nextW);

	  //Construct M.
	  myPrinter.Print(4, "Constructing M...");
	  ExpandM(M);
	  ConstructM(V, W, M);
	  myPrinter.Print(4, "done.\n");
	}

  myPrinter.Print(1, "Eigenvalue: %+13.10e %+13.10ei.\n", real(theta), imag(theta));

  FILE * fout = fopen(argv[3], "w");
  fprintf(fout, "%+13.10e %+13.10e ", real(theta), imag(theta));
  for(vector<ComplexDouble>::const_iterator it = u.begin(); it != u.end(); ++it)
	{
	  fprintf(fout, "%+13.10e %+13.10e ", real(*it), imag(*it));
	}
  fprintf(fout, "\n");

  
  fclose(fout);
  return 0;

}
