#include <cassert>
#include <vector>
#include "processing.hpp"

using namespace std;

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_left is provided for you.
// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the left (counterclockwise).
void rotate_left(Image* img) {

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, width - 1 - c, r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------

// v DO NOT CHANGE v ------------------------------------------------
// The implementation of rotate_right is provided for you.
// REQUIRES: img points to a valid Image.
// MODIFIES: *img
// EFFECTS:  The image is rotated 90 degrees to the right (clockwise).
void rotate_right(Image* img){

  // for convenience
  int width = Image_width(img);
  int height = Image_height(img);

  // auxiliary image to temporarily store rotated image
  Image aux;
  Image_init(&aux, height, width); // width and height switched

  // iterate through pixels and place each where it goes in temp
  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      Image_set_pixel(&aux, c, height - 1 - r, Image_get_pixel(img, r, c));
    }
  }

  // Copy data back into original
  *img = aux;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// v DO NOT CHANGE v ------------------------------------------------
// The implementation of diff2 is provided for you.
static int squared_difference(Pixel p1, Pixel p2) {
  int dr = p2.r - p1.r;
  int dg = p2.g - p1.g;
  int db = p2.b - p1.b;
  // Divide by 100 is to avoid possible overflows
  // later on in the algorithm.
  return (dr*dr + dg*dg + db*db) / 100;
}
// ^ DO NOT CHANGE ^ ------------------------------------------------


// ------------------------------------------------------------------
// You may change code below this line!



// REQUIRES: img points to a valid Image.
//           energy points to a Matrix.
// MODIFIES: *energy
// EFFECTS:  energy serves as an "output parameter".
//           The Matrix pointed to by energy is initialized to be the same
//           size as the given Image, and then the energy matrix for that
//           image is computed and written into it.
//           See the project spec for details on computing the energy matrix.
void compute_energy_matrix(const Image* img, Matrix* energy) {
  Matrix_init(energy, Image_height(img), Image_width(img)); 
  Matrix_fill(energy, 0);
  int max_energy = 0;

  for (int r = 0; r < Matrix_height(energy); ++r) {
    for (int c = 0; c < Matrix_width(energy); ++c) {
      int h_gradient = squared_difference(
        Image_get_pixel(img, r, c-1), Image_get_pixel(img, r, c+1));
      int v_gradient = squared_difference(
        Image_get_pixel(img, r-1, c), Image_get_pixel(img, r+1,c));

      int total_energy = h_gradient + v_gradient;
      *Matrix_at(energy, r, c) = total_energy;

      if (total_energy > max_energy){
        max_energy = total_energy;
      }
    }
  }
  
}


// REQUIRES: energy points to a valid Matrix.
//           cost points to a Matrix.
//           energy and cost aren't pointing to the same Matrix
// MODIFIES: *cost
// EFFECTS:  cost serves as an "output parameter".
//           The Matrix pointed to by cost is initialized to be the same
//           size as the given energy Matrix, and then the cost matrix is
//           computed and written into it.
//           See the project spec for details on computing the cost matrix.
void compute_vertical_cost_matrix(const Matrix* energy, Matrix *cost) {
  assert(energy != cost);
  int height = Matrix_height(energy);
  int width = Matrix_width(energy);

  Matrix_init(cost, height, width);
  for (int c = 0; c < width; ++c) {
    *Matrix_at(cost, 0, c) = *Matrix_at(energy, 0, c);
  }

  for (int r = 1; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      int min_cost = *Matrix_at(cost, r-1, c);
      if (c > 0) {
        min_cost = std::min(min_cost, *Matrix_at(cost, r-1, c-1));
      }
      if (c == 0) {
        min_cost = std::min(min_cost, *Matrix_at(cost, r-1, c));
      } 
      if (c < width - 1) {
        min_cost = std::min(min_cost, *Matrix_at(cost, r-1, c+1));
      }

      *Matrix_at(cost, r, c) = *Matrix_at(energy, r, c) + min_cost;
    }
  }
    }
  

// REQUIRES: cost points to a valid Matrix
// EFFECTS:  Returns the vertical seam with the minimal cost according to the given
//           cost matrix, represented as a vector filled with the column numbers for
//           each pixel along the seam, with index 0 representing the lowest numbered
//           row (top of image). The length of the returned vector is equal to
//           Matrix_height(cost).
//           While determining the seam, if any pixels tie for lowest cost, the
//           leftmost one (i.e. with the lowest column number) is used.
//           See the project spec for details on computing the minimal seam.
//           Note: When implementing the algorithm, compute the seam starting at the
//           bottom row and work your way up.
vector<int> find_minimal_vertical_seam(const Matrix* cost) {

  int height = Matrix_height(cost);
  int width = Matrix_width(cost);

  vector<int>seam(height, 0); 
  int min_col = 0; 
  int min_cost = *Matrix_at(cost, height - 1, 0); 

  for (int c= 1; c < width; ++c) {
    int current_cost = *Matrix_at(cost, height-1, c); 
    if (current_cost < min_cost){
      min_cost = current_cost; 
      min_col = c; 
    }
  }
    seam[height-1] = min_col; 
    for (int r = height - 2; r >= 0; --r){
      int prev_col = seam[r+1];
      int min_col = prev_col; 
      int min_cost = *Matrix_at(cost, r, prev_col);
      if (prev_col > 0) {
        int left_cost = *Matrix_at(cost, r, prev_col -1); 
        if (left_cost < min_cost) {
          min_cost = left_cost;
          min_col = prev_col - 1;
        }
      }
      if (prev_col == 0){
        min_cost = *Matrix_at(cost, r, prev_col);
        min_col = prev_col;
      }
      if (prev_col < width - 1) {
        int right_cost = *Matrix_at(cost, r, prev_col + 1); 
        if (right_cost < min_cost) {
          min_cost = right_cost;
          min_col = prev_col + 1;
        }
      }
      seam[r] = min_col;
    }
  


// REQUIRES: img points to a valid Image with width >= 2
//           seam.size() == Image_height(img)
//           each element x in seam satisfies 0 <= x < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Removes the given vertical seam from the Image. That is, one
//           pixel will be removed from every row in the image. The pixel
//           removed from row r will be the one with column equal to seam[r].
//           The width of the image will be one less than before.
//           See the project spec for details on removing a vertical seam.
// NOTE:     Declare a new variable to hold the smaller Image, and
//           then do an assignment at the end to copy it back into the
//           original image.
void remove_vertical_seam(Image *img, const vector<int> &seam) {
  assert(image_width(>= 2)); 
  int old_width = Image_width(img);
  int new_width = old_width -1; 
  int height = Image_height(img);

  Image new_img; 
  Image_init (&new_img, new_width, height); 

  for (int r = 0; r < height; ++r){
    int seam_col = seam[r]; 
    for (int c = 0; c < seam_col; ++c ){
      pixel p = Image_get_pixel(img, r, c); 
      Image_set_pixel(&new_img, r, c, p); 
    }
  }
}


// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width of the given Image to be newWidth by using
//           the seam carving algorithm. See the spec for details.
// NOTE:     Use a vector to hold the seam, and make sure that it has
//           the right size. You can use .data() on a vector to get
//           the underlying array.
void seam_carve_width(Image *img, int newWidth) {
   assert(img);
   asser(0 <newWidth && newWidth <= Image_width(img));
   
   while(Image_width(img) > newWidth) {
    Matrix energy;
    Matris cost;

    compute_energy_matrix(img, &energy);
    compute_vertical_cost_matrix(&energy , &cost);

    vector<int> seam = find_minimal_vertical_seam(&cost);
    vector<int> seam = find_minimal(&energy, &cost);

    vector<int> seam = find_minimal_verical_seam(&cost);
    remove_vertical_seam(img, seam);
   }
  }

// REQUIRES: img points to a valid Image
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the height of the given Image to be newHeight.
// NOTE:     This is equivalent to first rotating the Image 90 degrees left,
//           then applying seam_carve_width(img, newHeight), then rotating
//           90 degrees right.
void seam_carve_height(Image *img, int newHeight) {
  assert(false); // TODO Replace with your implementation!
}

// REQUIRES: img points to a valid Image
//           0 < newWidth && newWidth <= Image_width(img)
//           0 < newHeight && newHeight <= Image_height(img)
// MODIFIES: *img
// EFFECTS:  Reduces the width and height of the given Image to be newWidth
//           and newHeight, respectively.
// NOTE:     This is equivalent to applying seam_carve_width(img, newWidth)
//           and then applying seam_carve_height(img, newHeight).
void seam_carve(Image *img, int newWidth, int newHeight) {
  assert(false); // TODO Replace with your implementation!
}
