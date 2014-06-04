
#Matrix utility software

Consists of three different subprograms:

##Purify
Takes two binary square matrix objects of the form g*F + d*I where g and d are scalars, F is a full matrix and I is a diagonal matrix, and creates a binary object which can be used to construct other matrix of the form g*F + d*I for any g. 

###Format of input matrix:
In order, the content of an input binary file should be the following:
1. Two unsigned integers, n and m, describing the dimensions of the matrix. Requirement: n = m since the matrix should be square.
2. n*n complex<double>, the content of the matrix, row by row.
3. An unsigned long long with the value 38198464.


##ConstructMatrix
Operates on the output from Purify to construct a matrix on the form g*F + d*I.

##Jacobi
Operates on the output from ConstructMatrix, and finds the eigenvalue and eigenvector with the largest overlap of a vector with a component in a given direction.


#Note

At no point in any of the programs the full matrices are loaded into memory. If the matrices are of dimension n x n, the memory usage is O(n) with a small constant factor.