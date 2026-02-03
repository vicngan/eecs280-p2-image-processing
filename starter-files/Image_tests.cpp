#include "Matrix.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>

using namespace std;

// Sets various pixels in a 2x2 Image and checks that Image_print
// produces the correct output.
TEST(test_print_basic) {
  Image img;
  const Pixel red = {255, 0, 0};
  const Pixel green = {0, 255, 0};
  const Pixel blue = {0, 0, 255};
  const Pixel white = {255, 255, 255};

  Image_init(&img, 2, 2);
  Image_set_pixel(&img, 0, 0, red);
  Image_set_pixel(&img, 0, 1, green);
  Image_set_pixel(&img, 1, 0, blue);
  Image_set_pixel(&img, 1, 1, white);

  ostringstream s;
  Image_print(&img, s);

  ostringstream correct;
  correct << "P3\n2 2\n255\n";
  correct << "255 0 255 0 0 0 \n";
  correct << "255 0 0 0 255 0 \n";
  correct << "0 0 255 255 255 255 \n";
  ASSERT_EQUAL(s.str(), correct.str());
}

// Checks that Image_init sets width/height correctly.
TEST(test_init_dimensions) {
  Image img;
  Image_init(&img, 3, 4);
  ASSERT_EQUAL(Image_width(&img), 3);
  ASSERT_EQUAL(Image_height(&img), 4);
}

// Checks that all pixels are initialized to (0,0,0).
TEST(test_init_pixels_zero) {
  Image img;
  Image_init(&img, 2, 3);

  Pixel p00 = Image_get_pixel(&img, 0, 0);
  Pixel p12 = Image_get_pixel(&img, 1, 2);

  ASSERT_EQUAL(p00.r, 0);
  ASSERT_EQUAL(p00.g, 0);
  ASSERT_EQUAL(p00.b, 0);

  ASSERT_EQUAL(p12.r, 0);
  ASSERT_EQUAL(p12.g, 0);
  ASSERT_EQUAL(p12.b, 0);
}

// Checks set/get for one pixel and that other pixels remain unchanged.
TEST(test_set_get_single_pixel) {
  Image img;
  Image_init(&img, 2, 2);

  const Pixel p = {12, 34, 56};
  Image_set_pixel(&img, 1, 0, p);

  Pixel got = Image_get_pixel(&img, 1, 0);
  ASSERT_EQUAL(got.r, 12);
  ASSERT_EQUAL(got.g, 34);
  ASSERT_EQUAL(got.b, 56);

  Pixel other = Image_get_pixel(&img, 0, 0);
  ASSERT_EQUAL(other.r, 0);
  ASSERT_EQUAL(other.g, 0);
  ASSERT_EQUAL(other.b, 0);
}

// Checks overwriting the same pixel works.
TEST(test_set_overwrite_pixel) {
  Image img;
  Image_init(&img, 1, 1);

  const Pixel a = {1, 2, 3};
  const Pixel b = {200, 100, 50};

  Image_set_pixel(&img, 0, 0, a);
  Image_set_pixel(&img, 0, 0, b);

  Pixel got = Image_get_pixel(&img, 0, 0);
  ASSERT_EQUAL(got.r, 200);
  ASSERT_EQUAL(got.g, 100);
  ASSERT_EQUAL(got.b, 50);
}

// ✅ NEW: checks bottom-right corner pixel (width-1, height-1)
TEST(test_set_get_corner_pixel) {
  Image img;
  Image_init(&img, 4, 3); // width=4, height=3 => corner is (3,2)

  const Pixel corner = {9, 8, 7};
  Image_set_pixel(&img, 3, 2, corner);

  Pixel got = Image_get_pixel(&img, 3, 2);
  ASSERT_EQUAL(got.r, 9);
  ASSERT_EQUAL(got.g, 8);
  ASSERT_EQUAL(got.b, 7);
}

// ✅ NEW: set multiple pixels and verify all via get (stronger than print-only)
TEST(test_set_multiple_pixels_and_get_all) {
  Image img;
  Image_init(&img, 3, 2);

  const Pixel a = {1, 2, 3};
  const Pixel b = {4, 5, 6};
  const Pixel c = {7, 8, 9};

  Image_set_pixel(&img, 0, 0, a);
  Image_set_pixel(&img, 2, 0, b);
  Image_set_pixel(&img, 1, 1, c);

  Pixel got_a = Image_get_pixel(&img, 0, 0);
  Pixel got_b = Image_get_pixel(&img, 2, 0);
  Pixel got_c = Image_get_pixel(&img, 1, 1);
  Pixel still_zero = Image_get_pixel(&img, 1, 0);

  ASSERT_EQUAL(got_a.r, 1);
  ASSERT_EQUAL(got_a.g, 2);
  ASSERT_EQUAL(got_a.b, 3);

  ASSERT_EQUAL(got_b.r, 4);
  ASSERT_EQUAL(got_b.g, 5);
  ASSERT_EQUAL(got_b.b, 6);

  ASSERT_EQUAL(got_c.r, 7);
  ASSERT_EQUAL(got_c.g, 8);
  ASSERT_EQUAL(got_c.b, 9);

  ASSERT_EQUAL(still_zero.r, 0);
  ASSERT_EQUAL(still_zero.g, 0);
  ASSERT_EQUAL(still_zero.b, 0);
}

TEST(test_print_1x1) {
  Image img;
  Image_init(&img, 1, 1);

  const Pixel p = {7, 8, 9};
  Image_set_pixel(&img, 0, 0, p);

  ostringstream s;
  Image_print(&img, s);

  ostringstream correct;
  correct << "P3\n1 1\n255\n";
  correct << "7 8 9 \n";

  ASSERT_EQUAL(s.str(), correct.str());
}

TEST(test_print_rectangular_2x3) {
  Image img;
  Image_init(&img, 2, 3);

  const Pixel a = {10, 20, 30};
  const Pixel b = {40, 50, 60};
  const Pixel c = {70, 80, 90};

  Image_set_pixel(&img, 0, 0, a);
  Image_set_pixel(&img, 1, 1, b);
  Image_set_pixel(&img, 0, 2, c);

  ostringstream s;
  Image_print(&img, s);

  ostringstream correct;
  correct << "P3\n2 3\n255\n";
  correct << "10 20 30 0 0 0 \n";
  correct << "0 0 0 40 50 60 \n";
  correct << "70 80 90 0 0 0 \n";

  ASSERT_EQUAL(s.str(), correct.str());
}

TEST_MAIN() // Do NOT put a semicolon here
