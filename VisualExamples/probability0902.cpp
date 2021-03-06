#include "stdafx.h"

#include <iostream>
#include "Eigen/Dense"

using namespace Eigen;
using namespace std;

int main(int argc, char* argv[])
{	
	//
	// Creating matrices
	//
	// Here we use dynamically sized matrices with entries of type double
	// MatrixXd is predefined to do exactly that.

	// A 4x4 matrix is initialised with zeroes
	MatrixXd M1(4, 4);
	M1.setZero();
	std::cout << M1 << std::endl << std::endl;
	
	// A 2x2 matrix is initialised with zeros
	MatrixXd M2 = MatrixXd::Zero(2, 2);
	std::cout << M2 << std::endl << std::endl;

	// A 2x3 matrix with random entries
	MatrixXd M3 = MatrixXd::Random(2, 3);
	std::cout << M3 << std::endl << std::endl;

	// A 3x3 matrix all entries are 1.2
	MatrixXd M4 = MatrixXd::Constant(3, 3, 1.2);
	std::cout << M4 << std::endl << std::endl;

	//
	// We experiment with putting entries into the matrices
	//
	// We put 4 new entries into the matrix M2
	// We are filling up row by row
	M2 << 1, 2, 3, 4;
	std::cout << M2 << std::endl << std::endl;

	// M1 is a 4x4 matrix which can be thought of as
	// a 2x2 matrix where entries ae 2x2 matrices.
	M1 << M2, M2*M2, 10 * M2, 2 * M2;
	std::cout << M1 << std::endl << std::endl;

	// We can fill in individual rows and columns
	M2.row(1) << 0, 0;
	M2.col(0) << 10, 10;
	std::cout << M2 << std::endl << std::endl;

	// Individual entries can be accessed
	M3(1, 1) = 12.0;
	std::cout << M3 << std::endl << std::endl;

	// We can pick out top-left or bottom-right corners and manipulate those

	// We pick out the 2x2 submatrix with 
	// indices (0,0) -- (2,2) and set it to be zero
	M1.topLeftCorner(2, 2).setZero();
	std::cout << M1 << std::endl << std::endl;

	// We pick out the 2x2 submatrix with indices 
	// (2,2) -- (4,4) and set it to the identity matrix
	M1.bottomRightCorner(2, 2).setIdentity();
	std::cout << M1 << std::endl << std::endl;
	
	// We pick out a 2x2 block with top-left corner at (1,1)
	M1.block<2, 2>(1, 1) << 10, 11, 12, 13;
	std::cout << M1 << std::endl << std::endl;

	// We pick out a block with top-left corner (1,1) and bottom right corner (2,2)
	M1.block(1, 1, 2, 2) << 20, 21, 22, 23;
	std::cout << M1 << std::endl << std::endl;

	// We fill in the first 1 rows
	M1.topRows(1) << 1, 2, 3, 4;  
	std::cout << M1 << std::endl << std::endl;

	// We fill in two leftmost columns. We need 8 numbers
	// for two columns in M1
	M1.leftCols(2) << 1, 2, 3, 4, 5, 6, 7, 8;  
	std::cout << M1 << std::endl << std::endl;

	//
	// Matrix operations
	//
	M1 = MatrixXd::Random(2, 2);
	M2 = MatrixXd::Random(2, 3);
	M3 = MatrixXd::Random(2, 2);
	std::cout << M1 * M2 << std::endl << std::endl;
	std::cout << M1 + M3 << std::endl << std::endl;
	std::cout << M1 * 2 + 2 * M3 << std::endl << std::endl;

	// cout << M2 * M1; // FAILS - do you see why?

	// self-explanatory
	std::cout << M1.inverse() << std::endl << std::endl << M1.transpose();

}






