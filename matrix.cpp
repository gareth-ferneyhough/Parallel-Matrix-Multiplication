#include <iostream>
#include <assert.h>
#include "matrix.h"

Matrix::Matrix() :
  size(Size(0,0))
{
}

Matrix::Matrix(const Size& size) :
  size(size)
{
  data = new int*[size.rows];
  for (int i = 0; i < size.rows; ++i)
    data[i] = new int[size.cols];
}

Matrix::~Matrix()
{
  for(int i = 0; i < size.rows; ++i){
    delete[] data[i];
  }

  delete[] data;
}

Matrix Matrix::operator=(const Matrix& rhs)
{
  if (this == &rhs)      // Same object?
    return *this;        // Yes, so skip assignment, and just return *this.

  // Deallocate data
  if(this->size.rows < 0){

    for(int i = 0; i < this->size.rows; ++i){
      delete[] this->data[i];
    }

    delete[] this->data;
  }


  // Reallocate and copy
  this->size = rhs.size;
  std::cout << "asdf" << this->size;

  this->data = new int*[this->size.rows];
  for (int i = 0; i < this->size.rows; ++i)
    this->data[i] = new int[this->size.cols];

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
   assert(rhs.size.rows == rhs.size.cols);// == this->size.rows == this->size.cols);
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