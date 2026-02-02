#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Here's a free test for you! Model yours after this one.
// Test functions have no interface and thus no RMEs, but
// add a comment like the one here to say what it is testing.
// -----
// Fills a 3x5 Matrix with a value and checks
// that Matrix_at returns that value for each element.
TEST(test_fill_basic) {
  Matrix mat;
  const int width = 3;
  const int height = 5;
  const int value = 42;
  Matrix_init(&mat, 3, 5);
  Matrix_fill(&mat, value);

  for(int r = 0; r < height; ++r){
    for(int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value);
    }
  }
}
TEST(test_fill_square) {
  Matrix mat; 
  const int width = 4; 
  const int height = 4;
  const int value = 7;
  Matrix_init(&mat, width, height); // initialize 4x4 matrix
  Matrix_fill(&mat, value);

  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value); 
    }
  }
}

TEST(test_fill_negative) {
  Matrix mat;
  const int width = 4;
  const int height = 6;
  const int value = -4;
  Matrix_init (&mat, width, height);
  Matrix_fill(&mat, value);

  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value); 
    }
  }
}

TEST(test_fill_zero){
  Matrix mat;
  const int width = 4; 
  const int height = 5;
  const int value = 0; 
  Matrix_init (&mat, width, height);
  Matrix_fill (&mat, value); 

  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      ASSERT_EQUAL(*Matrix_at(&mat, r, c), value); 
    }
  }
}

bool equal_matrices(const Matrix *mat1, const Matrix *mat2){
  if (Matrix_width(mat1) != Matrix_width(mat2)) 
    return false; 
  if (Matrix_height(mat1) != Matrix_height(mat2))
    return false; 
  for (int r = 0; r < Matrix_height(mat1); ++r){
    for(int c = 0; c < Matrix_width(mat1); ++c){
      if (*Matrix_at(mat1, r,c )!= *Matrix_at(mat2, r,c))
        return false;
    }
  }
  return true;
}

TEST(test_matrix_print){
  Matrix mat;
  Matrix_init(&mat, 4,4);
  *Matrix_at(&mat, 0,0) = 1; 
  *Matrix_at(&mat, 0,1) = 2; 
  *Matrix_at(&mat, 0,2) = 3; 
  *Matrix_at(&mat, 0,3) = 4;
  *Matrix_at(&mat, 0,4) = 5;

  ostringstream expected;
  expected << "4 4\n"
           << "1 2 3 4 \n"
           << "5 0 0 0 \n"
           << "0 0 0 0 \n"
           << "0 0 0 0 \n";
  ostringstream actual; 
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}


TEST_MAIN()