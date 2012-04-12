#include <ostream>

class Size{
 public:
 Size(int rows, int cols) :
  rows(rows),
    cols(cols)
    {}

  Size operator=(const Size& rhs){
    if (this != &rhs){
      this->rows = rhs.rows;
      this->cols = rhs.cols;
    }
    return *this;
  }

  int rows;
  int cols;
  friend std::ostream& operator<< (std::ostream& stream, const Size& size);
};

class Matrix{
 public:
  Matrix();
  Matrix(const Size& size);  
  ~Matrix();
  
  Matrix operator=(const Matrix& rhs);
  Matrix operator*(const Matrix& rhs);

  Size size;
  int** data;
  friend std::ostream& operator<< (std::ostream& stream, const Matrix& matrix);
};
