#include <iostream>
#include <assert.h>
#include "matrix.h"

Matrix::Matrix() :
  size(Size(0,0)),
  top_left(0,0),
  lower_right(0,0)
{
}

Matrix::Matrix(const Size& size) :
  size(size),
  top_left(top_left),
  lower_right(lower_right)
{
  assert(size.rows == size.cols);

  data.resize(size.rows);
  for (int i = 0; i < data.size(); ++i)
    data[i].resize(size.cols);
}

Matrix::~Matrix()
{
}

Matrix Matrix::operator=(const Matrix& rhs)
{
  if (this == &rhs)      // Same object?
    return *this;        // Yes, so skip assignment, and just return *this.

  // Reallocate and copy
  size        = rhs.size;
  top_left    = rhs.top_left;
  lower_right = rhs.lower_right;

  data.clear();
  data.resize(size.rows);
  for (int i = 0; i < size.rows; ++i){
    data[i].clear();
    data[i].resize(size.cols);
  }

  for (int row = 0; row < size.rows; ++row){
    for (int col = 0; col < size.cols; ++col){
      data[row][col] = rhs.data[row][col];
    }
  }

  return *this;
}

// Square for now
Matrix Matrix::operator*(const Matrix& rhs)
{
  // square
  assert(rhs.size.rows == rhs.size.cols);
  assert(this->size.rows == rhs.size.rows);
  assert(this->size.cols == rhs.size.cols);

  Matrix result(this->size);

  for(int row = 0; row < this->size.rows; ++row){
    for(int col = 0; col < this->size.cols; ++col){
      int sum = 0;
      for(int k = 0; k < this->size.rows; ++k){
        sum += this->data[row][k] * rhs.data[k][col];
      }
      result.data[row][col] = sum;
    }
  }
  return result;
}

bool Matrix::operator==(const Matrix& rhs)
{
  return data == rhs.data;
}

std::vector<int> Matrix::getRow(int row) const
{
  return data[row];
}

std::vector<int> Matrix::getCol(int col) const
{
  std::vector<int> col_data;
  for(int row = 0; row < size.rows; row++)
    col_data.push_back(data[row][col]);

  return col_data;
}

void Matrix::insertSubMatrix(const Matrix& sub_matrix)
{
  for(int i = 0; i < sub_matrix.size.rows; ++i){
    for(int j = 0; j < sub_matrix.size.cols; ++j){
      data[i + sub_matrix.top_left.row][j + sub_matrix.top_left.col] = sub_matrix.data[i][j];
    }
  }
}

std::ostream& operator <<(std::ostream& stream, const Matrix& matrix)
{
  for(int row = 0; row < matrix.size.rows; ++row){
    for(int col = 0; col < matrix.size.cols; ++col){
      stream << matrix.data[row][col] << " ";
    }
    stream << std::endl;
  }
  return stream;
}

std::ostream& operator <<(std::ostream& stream, const Size& size)
{
  stream << size.rows << "," << size.cols;
  return stream;
}

MatrixCrossSection::MatrixCrossSection()
{
  row_id = col_id = -1;
}

MatrixCrossSection::~MatrixCrossSection()
{
}

MatrixCrossSection MatrixCrossSection::operator=(const MatrixCrossSection& rhs)
{
  if (this == &rhs)      // Same object?
    return *this;        // Yes, so skip assignment, and just return *this.

  row_id   = rhs.row_id;
  col_id   = rhs.col_id;

  assert( rhs.row_data.size() == rhs.col_data.size() );

  row_data.resize(rhs.row_data.size());
  col_data.resize(rhs.col_data.size());

  for(int i = 0; i < rhs.row_data.size(); ++i){
    std::copy( rhs.row_data[i].begin(), rhs.row_data[i].end(), std::back_inserter( row_data[i] ) );
    std::copy( rhs.col_data[i].begin(), rhs.col_data[i].end(), std::back_inserter( col_data[i] ) );
  }
  return *this;
}

void MatrixCrossSection::calculateVectorProduct(Matrix& mat)
{
  assert( row_data.size() == col_data.size() );
  int size = row_data.size();

  mat.top_left    = Index(row_id, col_id);
  mat.lower_right = Index(row_id + size, col_id + size);

  for(int row = 0; row < size; ++row){
    for(int col = 0; col < size; ++col){

      int sum = 0;
      for(int i = 0; i < row_data[0].size(); ++i){
        sum += row_data[row][i] * col_data[col][i];
      }
      mat.data[row][col] = sum;
    }
  }
}
