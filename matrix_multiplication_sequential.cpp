#define BOOST_CHRONO_HEADER_ONLY

#include <assert.h>
#include <iostream>
#include <boost/chrono.hpp>
#include "matrix.h"

using std::cout;
using std::endl;

int main(int argc, char** argv){
  assert(argc == 2);
  int size = atoi(argv[1]);

  Matrix A(Size(size, size));
  Matrix B(Size(size, size));

  for(int row = 0; row < A.size.rows; ++row){
    for(int col = 0; col < A.size.cols; ++col){
      A.data[row][col] = row +1 * col;
    }
  }

  for(int row = 0; row < B.size.rows; ++row){
    for(int col = 0; col < B.size.cols; ++col){
      B.data[row][col] = row +1 * col;
    }
  }

  //cout << A << endl;
  //cout << B << endl;

  // Start timer and go
  boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

  Matrix C = A*B;

  // Done
  boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  
  //cout << C << endl;
  cout << sec << endl;

  return 0;
}


  
