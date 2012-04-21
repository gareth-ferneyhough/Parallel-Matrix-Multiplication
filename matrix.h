#include <boost/serialization/serialization.hpp>
#include <vector>
#include <ostream>

class Index{
 public:
 Index(int row, int col) :
    row(row),
    col(col)
    {}

 Index operator=(const Index& rhs){
    if (this != &rhs){
      this->row = rhs.row;
      this->col = rhs.col;
    }
    return *this;
  }

  int row;
  int col;
};

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
  std::vector<int> getRow(int row_id) const;
  std::vector<int> getCol(int col_id) const;

  Size size;
  int** data;
  friend std::ostream& operator << (std::ostream& stream, const Matrix& matrix);
  
  // this is a hack to specify sub-matricies
  Index top_left;
  Index lower_right;
};

class MatrixCrossSection{
 public:
  MatrixCrossSection();
  ~MatrixCrossSection();
  MatrixCrossSection operator=(const MatrixCrossSection& rhs);
  void calculateVectorProduct(Matrix& mat);
  
  int row_id;
  int col_id;
  std::vector<std::vector<int> >row_data;
  std::vector<std::vector<int> >col_data;

  friend class boost::serialization::access;

  template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
      ar & row_id;
      ar & col_id;
      ar & row_data;
      ar & col_data;
    }
};
