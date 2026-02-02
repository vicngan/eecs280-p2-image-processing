#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include <cstdlib>

#include "Image.hpp"
#include "processing.hpp"

using namespace std;

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Prints the Image in PPM format to the given output stream.

int main(int argc, char *argv[]) {

    if (argc != 4 && argc !=5) {
        cout << "resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
        return 1;
    }
    string in_filename = argv[1]; 
    string out_filename = argv[2]; 
    int new_width = atoi (argv[3]); 
    if (new_width <= 0){
        cout << "Error: dimension out of bounds" << endl; 
        return 1;
    }

    ifstream in_file (in_filename);
    if (!in_file.is_open()){
        cout << "Error: input file could not be opened" << endl; 
        return 1;
    }

    Image *img = new Image; 
    Image_init (img, in_file);
    in_file.close(); 

    int new_height = (argc == 5) ? atoi(argv[4]): Image_height(img); 
    if (new_width <= 0 || new_width > Image_width(img) ||
        new_height <= 0 || new_height > Image_height (img)){
            cout << "Error: dimension out of bounds" << endl; 
        delete img; 
        return 1;
    }
    seam_carve(img, new_width, new_height);

    ofstream out_file(out_filename); 
    if (!out_file.is_open()){
        cout << "Error: output file could not be opened" << endl;
        delete img; 
        return 1;
    }

    Image_print (img, out_file);
    out_file.close(); 

    delete img; 
    return 0;
}
