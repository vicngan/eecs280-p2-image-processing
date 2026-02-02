#include <cassert>
#include "Matrix.hpp"

// REQUIRES: mat points to a Matrix
//           0 < width && 0 < height
// MODIFIES: *mat
// EFFECTS:  Initializes *mat as a Matrix with the given width and height,
//           with all elements initialized to 0.
void Matrix_init(Matrix* mat, int width, int height) {
  
  assert (mat != nullptr);
  assert (width > 0);
  assert(height > 0);

  (*mat).width = width;
  (*mat).height = height;
  (*mat).data.assign(width * height, 0); 

}
  
// REQUIRES: mat points to a valid Matrix
// MODIFIES: os
// EFFECTS:  First, prints the width and height for the Matrix to os:
//             WIDTH [space] HEIGHT [newline]
//           Then prints the rows of the Matrix to os with one row per line.
//           Each element is followed by a space and each row is followed
//           by a newline. This means there will be an "extra" space at
//           the end of each line.
void Matrix_print(const Matrix* mat, std::ostream& os) {
  assert(mat != nullptr);

  os << mat->width << " " << mat->height << "\n";

  for (int r = 0; r < mat->height; ++r) {
    for (int c = 0; c < mat->width; ++c) {
      os << mat->data[r * mat->width + c] << " ";
    }
    os << "\n";
  }
}


// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the width of the Matrix.
int Matrix_width(const Matrix* mat) {
  return (*mat).width;
}

// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the height of the Matrix.
int Matrix_height(const Matrix* mat) {
  return (*mat).height;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// MODIFIES: (The returned pointer may be used to modify an
//            element in the Matrix.)
// EFFECTS:  Returns a pointer to the element in the Matrix
//           at the given row and column.
int* Matrix_at(Matrix* mat, int row, int column) {
  assert(row >= 0 && row < Matrix_height(mat)); 
  assert(column >= 0 && column < Matrix_width(mat));
  
  return &(*mat).data[row * Matrix_width(mat) + column];
  
  
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column && column < Matrix_width(mat)
//
// EFFECTS:  Returns a pointer-to-const to the element in
//           the Matrix at the given row and column.
const int* Matrix_at(const Matrix* mat, int row, int column) {
  assert (row >= 0 && row < Matrix_height(mat)); 
  assert (column >= 0 && column < Matrix_width(mat));

  return &(*mat).data[row * Matrix_width(mat) + column]; 
  

}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element of the Matrix to the given value.
void Matrix_fill(Matrix* mat, int value) {
  for (size_t i = 0; i < (*mat).data.size(); i++) {
    (*mat).data[i] = value; 
  } 
}

// REQUIRES: mat points to a valid Matrix
// MODIFIES: *mat
// EFFECTS:  Sets each element on the border of the Matrix to
//           the given value. These are all elements in the first/last
//           row or the first/last column.
void Matrix_fill_border(Matrix* mat, int value) {
  int h = Matrix_height(mat);
  int w = Matrix_width(mat);
  for (int i = 0; i < w; i++) {
    *Matrix_at(mat, 0, i) = value;
    *Matrix_at(mat, h-1, i) = value; 
  for (int i = 0; i < h; i++) {
    *Matrix_at(mat, 0, i) = value;
    *Matrix_at(mat, w-1, i) = value;
  }}
}


// REQUIRES: mat points to a valid Matrix
// EFFECTS:  Returns the value of the maximum element in the Matrix
int Matrix_max(const Matrix* mat) {
  int max_val = *Matrix_at(mat, 0, 0);  //first pixel value as initial start 
  for (int r = 0; r < Matrix_height(mat); r++) {
    for (int c = 0; c < Matrix_width(mat); c++) {
      if (*Matrix_at(mat, r, c) > max_val) {
        max_val = *Matrix_at(mat, r, c); 
        return max_val;
      }
    }
  } return 0;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the column of the element with the minimal value
//           in a particular region. The region is defined as elements
//           in the given row and between column_start (inclusive) and
//           column_end (exclusive).
//           If multiple elements are minimal, returns the column of
//           the leftmost one.
int Matrix_column_of_min_value_in_row(const Matrix* mat, int row,
                                        int column_start, int column_end) {
  assert (row >= 0 && row < Matrix_height(mat)); 
  assert (column_start >= 0 && column_end <= Matrix_width(mat)); 
  assert (column_start < column_end);

  int min_val = *Matrix_at(mat, row, column_start);
  int min_col = column_start;
  for (int c = column_start; c <column_end; c++) {
    if(*Matrix_at(mat, row, c) < min_val) {
      min_val = *Matrix_at(mat, row, c);
      min_col = c; 
      return min_col; 
    }
  }
  return 0;
}

// REQUIRES: mat points to a valid Matrix
//           0 <= row && row < Matrix_height(mat)
//           0 <= column_start && column_end <= Matrix_width(mat)
//           column_start < column_end
// EFFECTS:  Returns the minimal value in a particular region. The region
//           is defined as elements in the given row and between
//           column_start (inclusive) and column_end (exclusive).
int Matrix_min_value_in_row(const Matrix* mat, int row,
                              int column_start, int column_end) {
  assert (row >= 0 && row < Matrix_height(mat)); 
  assert (column_start >= 0 && column_end <= Matrix_width(mat)); 
  assert (column_start < column_end);
                              
  int min_val = *Matrix_at(mat, row, column_start); 
  for (int c = column_start; c < column_end; c++) {
    if (*Matrix_at(mat, row, c) < min_val) {
        min_val = *Matrix_at(mat, row, c); 
        return min_val;
    }
  } 
  return 0;
} 

