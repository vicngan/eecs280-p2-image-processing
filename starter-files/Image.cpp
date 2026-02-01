#include <cassert>
#include "Image.hpp"

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height) {
  assert(img != nullptr);
  assert(width > 0 && height > 0);

  img -> width = width;
  img -> height = height; 

  //initializing the three color channels
  Matrix_init (&img->red_channel, width, height);
  Matrix_init (&img->green_channel, width, height);
  Matrix_init (&img->blue_channel, width, height);
}

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is) {
  assert (img != nullptr);

  std::string ppm_type;

  int width, height, max_color_value;
  if (!(is >> ppm_type >> width >> height >> max_color_value)) {
    return ; //Invald PPM format
  }
  assert(ppm_type == "P3");
  assert (width > 0 && height > 0); 
  assert (max_color_value == 255); 

  Image_init (img, width, height); 
  for (int r = 0; r < height; ++r){
    for (int c = 0; c < width; ++c){
      int red, green, blue;
      is >> red >> green >> blue; //reading pixel values
      *Matrix_at(&img -> red_channel, r, c) = red; 
      *Matrix_at(&img -> green_channel, r, c) = green; 
      *Matrix_at(&img -> blue_channel, r, c) = blue;

      Pixel p = {red, green, blue};
      Image_set_pixel(img, r, c, p); //setting pixel values
    }
  }
}



// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os) {
  assert(img != nullptr);
   //Header
  
  os << "P3\n";
  os << Image_width(img) << " " << Image_height(img) << "\n";
  os << "255\n";

  for (int r = 0; r < Image_height(img); r++){
    for (int c = 0; c < Image_width(img); c++){
      Pixel p = Image_get_pixel(img, r, c);
        os << p.r << " " << p.g << " " << p.b << " ";
    }
    os << "\n";
  }
}   
  

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img) {
  assert (img != nullptr); 
  return img -> width;
  
}


// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img) {
  assert (img != nullptr);
  return img -> height;

}

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column) {
  assert(img != nullptr);
  assert(0 <= row && row < Image_height(img));
  assert( 0 <= column && column < Image_width(img));
   
  Pixel p;
  p.r = *Matrix_at(&img->red_channel, row, column);
  p.g = *Matrix_at(&img->green_channel, row, column);
  p.b = *Matrix_at(&img->blue_channel, row, column);

  return p;
}




// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color) {
  assert(img != nullptr); 
  assert(0 <= row && row < Image_height(img));
  assert(0 <= column && column < Image_width(img));
  
  *Matrix_at(&img->red_channel, row, column) = color.r;
  *Matrix_at(&img->green_channel, row, column) = color.g;
  *Matrix_at(&img->blue_channel, row, column) = color.b;
}



// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color) {
  assert(img != nullptr);
  for (int r = 0; r < Image_height(img); r++){
    for (int c = 0; c < Image_width(img); c++){
      Image_set_pixel(img, r, c, color);
    }
  }
}
