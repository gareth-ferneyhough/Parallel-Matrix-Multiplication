/* matrix_multiplication.cpp        *
 * Parallel matrix multiplication   *
 * By: Gareth Ferneyhough           *
 * University of Nevada, Reno       */

#define BOOST_CHRONO_HEADER_ONLY

#include <algorithm>
#include <string>
#include <fstream>
#include <boost/chrono.hpp>
#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/serialization.hpp>
#include "matrix.h"

namespace mpi = boost::mpi;
using std::string;
using std::cout;
using std::endl;

void runMaster(mpi::communicator, int size, int grid_dimension);
void runSlave(mpi::communicator);
MatrixCrossSection getCrossSection(const Matrix& mat, int row_begin, int col_begin, int num_cols_and_rows);

int main(int argc, char* argv[])
{
  assert(argc == 2);
  int size = atoi(argv[1]);

  mpi::environment env(argc, argv);
  mpi::communicator world;
  const int my_rank = world.rank();

  int grid_dimension = sqrt(world.size() -1);
  assert( size % grid_dimension == 0);

  // Main Loop
  if (my_rank == 0) runMaster(world, size, grid_dimension);
  else              runSlave(world);

  return 0;
}

void runMaster(mpi::communicator world, int size, int grid_dimension)
{
  // Start timer and go.
  boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

  // Send
  Matrix      A(Size(size, size));
  Matrix result(Size(size, size));

  for(int row = 0; row < A.size.rows; ++row){
    for(int col = 0; col < A.size.cols; ++col){
      A.data[row][col] = row +1 * col;
    }
  }
  //cout << A << endl;
  //cout << "\nProduct:\n" << A*A << endl;

  // Split matrix up and send to slaves
  int slave_id = 1;
  int sub_matrix_sizes = size / grid_dimension;

  for(int i = 0; i < size; i += sub_matrix_sizes){
    for(int j = 0; j < size; j += sub_matrix_sizes){
      MatrixCrossSection cs = getCrossSection( A, i, j, sub_matrix_sizes);
      world.isend(slave_id, 0, cs);
      slave_id ++;
    }
  }

  // Recieve
  std::vector<Matrix> saved;
  int num_slaves = world.size() -1;
  
  for(int i = 1; i <= num_slaves; ++i){
    Matrix r;
    world.recv(i, 0, r);
    result.insertSubMatrix(r);
  }

  // Done
  boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  cout << "took " << sec.count() << " seconds\n";

  // Print Result
  //cout << "\nResult:\n" << result << endl;

  assert ( result == A*A);
}

void runSlave(mpi::communicator world)
{
  // Recieve
  MatrixCrossSection cs;
  world.recv(0, 0, cs);

  Matrix subMatrix(Size(cs.row_data.size(), cs.row_data.size()));
  cs.calculateVectorProduct(subMatrix);
  
  world.send(0, 0, subMatrix);
}

MatrixCrossSection getCrossSection(const Matrix& mat, int row_begin, int col_begin, int num_cols_and_rows)
{
  MatrixCrossSection cs;
  cs.row_id = row_begin;
  cs.col_id = col_begin;

  for(int i = 0; i < num_cols_and_rows; ++i){
    cs.row_data.push_back( mat.getRow(row_begin + i) );
    cs.col_data.push_back( mat.getCol(col_begin + i) );
  }
  return cs;
}

