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

void runMaster(mpi::communicator, int size);
void runSlave(mpi::communicator);
MatrixCrossSection getCrossSection(const Matrix& mat, int row_begin, int col_begin, int num_cols_and_rows);

int main(int argc, char* argv[])
{
  assert(argc == 2);
  int size = atoi(argv[1]);

  mpi::environment env(argc, argv);
  mpi::communicator world;
  const int my_rank = world.rank();


  // Main Loop
  if (my_rank == 0) runMaster(world, size);
  else              runSlave(world);

  return 0;
}

void runMaster(mpi::communicator world, int size)
{
  // Start timer and go.
  boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();

  mpi::request reqs[1];

  // Send
  Matrix A(Size(size, size));
  for(int row = 0; row < A.size.rows; ++row){
    for(int col = 0; col < A.size.cols; ++col){
      A.data[row][col] = row +1 * col;
    }
  }
  cout << A << endl;
  cout << "\nProduct:\n" << A*A << endl;

  MatrixCrossSection cs = getCrossSection( A, 0, 0, 2);
  reqs[0] = world.isend(1, 0, cs);

  // Recieve
  // std::vector<int> returned;
  // world.recv(1, 0, returned);

  // cout << "Recived: ";
  // std::copy(returned.begin(), returned.end(), std::ostream_iterator<int>(cout, ","));
  // cout << endl;

  //  mpi::wait_all(reqs, reqs + 1);
  // Done

  boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
  cout << "took " << sec.count() << " seconds\n";
}

void runSlave(mpi::communicator world)
{
  // Recieve
  MatrixCrossSection cs;
  world.recv(0, 0, cs);

  for(int i = 0; i < cs.row_data.size(); ++i){
    cout << "\n\nRow " << cs.row_id + i << ":\n";
    std::copy(cs.row_data[i].begin(), cs.row_data[i].end(), std::ostream_iterator<int>(cout, ","));

    cout << "\nCol " << cs.col_id + i << ":\n";
    std::copy(cs.col_data[i].begin(), cs.col_data[i].end(), std::ostream_iterator<int>(cout, ","));
  }

  Matrix subMatrix(Size(cs.row_data.size(), cs.row_data.size()));
  cs.calculateVectorProduct(subMatrix);

  cout << "\nSubMatrix:\n" << subMatrix;
  cout << endl;
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

