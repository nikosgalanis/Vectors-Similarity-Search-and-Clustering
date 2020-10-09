#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 



using namespace std;

int reverseInt (int i) {
    unsigned char c1, c2, c3, c4;

    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;

    return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void parse_input(string filename, list<vector<int>> list_of_vectors) {
    char data[5];
    ifstream input;
    input.open(filename); //TODO: Might add ios::binary
    if (input.is_open()) {
        // read the magic number of the image
        int magic_number = 0;
        input.read((char*)&magic_number, sizeof(int));
        magic_number = reverseInt(magic_number);
        // find out how many images we are going to parse
        int n_of_images;
        input.read((char*)&n_of_images, sizeof(int));
        n_of_images = reverseInt(n_of_images);
        for 
    }
}

int main(void) {
    list<vector<int>> list_of_vectors;
    // parse_input("/home/nikos/Desktop/7th Semester/Software Development for Algorithmic Problems/Projects/Vectors-Similarity-Search/misc/datasets/train-images-idx3-ubyte");
}