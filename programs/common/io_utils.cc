#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 

#include "../../utils/math_utils.h"
#include "io_utils.h"
using namespace std;

/**
Function to parse the input given. Both the train and the test datasets are in the form:
 - Magic number (int)
 - N of images in dataset (int)
 - n of columns and rows for the images
 - Binary representation of the images pixels (0-255)

 NOTE: all the numbers are given in Big-Endian
**/
vector<vector<double>> parse_input(string filename) {
    // open the dataset
    ifstream input(filename, ios::binary);

    // read the magic number of the image
    int magic_number = 0;
    input.read((char*)&magic_number, sizeof(int));
    magic_number = our_math::big_to_litte_endian(magic_number);
    cout << magic_number << endl;
    // find out how many images we are going to parse
    int n_of_images;
    input.read((char*)&n_of_images, sizeof(int));
    n_of_images = our_math::big_to_litte_endian(n_of_images);
    cout << n_of_images << endl;

    // create a list of vectors to return
    vector<vector<double>> list_of_vectors(n_of_images);

    // read the dimensions
    int rows = 0;
    input.read((char*)&rows, sizeof(int));
    rows = our_math::big_to_litte_endian(rows);
    int cols;
    input.read((char*)&cols, sizeof(int));
    cols = our_math::big_to_litte_endian(cols);
    cout << rows << cols << endl;

    double x;
    // for each image start filling the vectors
    for (int i = 0; i < n_of_images; i++) {
        // create a vector to store our image data
        vector<double> vec(rows * cols);
        // store each byte of the image in the vector, by parsing boh of the image's dimensions
        for (int j = 0; j < rows; j++) {
            for (int k = 0; k < cols; k++) {
                // the pixels are from 0-255, so an unsinged char type is enough
                unsigned char pixel = 0;
                input.read((char*)(&pixel), sizeof(pixel));
                // change its value to double
                x = (double)pixel;
                // push the byte into the vector
                vec.push_back(x);
            }
        }
        // push the vector in our list
        list_of_vectors.push_back(vec);
    }

    // return the list of vectors that we created
    return list_of_vectors;
}
