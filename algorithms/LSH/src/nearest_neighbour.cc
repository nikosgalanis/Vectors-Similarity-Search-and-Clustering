#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <limits>
#include <utility>


#include "../../../utils/math_utils.h"
#include "../headers/hashing.h"
#include "../headers/lsh.h"
#include "../../common/metrics.h"
#include "../../../utils/print_utils.h"

using namespace std;
using namespace metrics;

template <typename T>
// function that implements NearestNeighbour
pair<int,T> LSH<T>::NearestNeighbour(vector<T> query_vector) {	
    
	int vector_index;
    T distance_b = (T) INT_MAX;


    int i = 0;
    int visited = 0;

	//Traverse the hash tables
    for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
        it != amplified_hash_fns.end(); it++) {
        // find the bucket that the query hashes into
        int index = it->assign_to_bucket(query_vector);
        list<int> bucket = lsh_tables.at(i)[index];
        // get all the indexes of the vectors in the bucket
        for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
            // vector<T> curr_vector = feature_vectors.at(*bucket_it);
            visited++;
            T distance =  ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
            if (distance < distance_b) {
                vector_index = *bucket_it; //TODO: Almost certainly wrong
                distance_b = distance;
            }
            // if (visited > 10 * L) {
            //     return b;
            // }
        }
        i++;
    }

	// return pair
    return std::make_pair(vector_index, distance_b);
}   

// vector<vector<double>> parse_input(string filename) {
// 	// open the dataset
// 	ifstream input(filename, ios::binary);

// 	// read the magic number of the image
// 	int magic_number = 0;
// 	input.read((char*)&magic_number, sizeof(int));
// 	magic_number = our_math::big_to_litte_endian(magic_number);
// 	// find out how many images we are going to parse
// 	int n_of_images;
// 	input.read((char*)&n_of_images, sizeof(int));
// 	n_of_images = our_math::big_to_litte_endian(n_of_images);

// 	// create a list of vectors to return
// 	vector<vector<double>> list_of_vectors;
// 	// read the dimensions
// 	int rows = 0;
// 	input.read((char*)&rows, sizeof(int));
// 	rows = our_math::big_to_litte_endian(rows);
// 	int cols;
// 	input.read((char*)&cols, sizeof(int));
// 	cols = our_math::big_to_litte_endian(cols);

// 	double x;
// 	// for each image start filling the vectors
// 	for (int i = 0; i < n_of_images; i++) {
// 		// create a vector to store our image data
// 		vector<double> vec;
// 		// store each byte of the image in the vector, by parsing boh of the image's dimensions
// 		for (int j = 0; j < rows; j++) {
// 			for (int k = 0; k < cols; k++) {
// 				// the pixels are from 0-255, so an unsinged char type is enough
// 				unsigned char pixel = 0;
// 				input.read((char*)(&pixel), sizeof(pixel));
// 				// change its value to double
// 				x = (double)pixel;
// 				// push the byte into the vector
// 				vec.push_back(x);
// 			}
// 		}
// 		// push the vector in our list
// 		list_of_vectors.push_back(vec);
// 	}
// 	// return the list of vectors that we created
// 	return list_of_vectors;
// }




// int main(int argc, char* argv[]) {
// 	vector<vector<double>> items = parse_input("../../../misc/datasets/train-images-idx3-ubyte");
// 	vector<vector<double>> queries = parse_input("../../../misc/querysets/t10k-images-idx3-ubyte");

// 	vector<double> first = queries.at(2);
// 	// vector<vector<float>> a;
// 	// a.push_back(x); a.push_back(y);
// 	LSH<double> instant(4, (uint64_t)(pow(2,32) - 5), pow(2,8), 1000, 4, items.at(1).size(), 10, items);
// 	// list<vector<double>> result = instant.RangeSearch(first, 1.0, 1);
//     pair<int,double> result = instant.NearestNeighbour(first);
	
// 	// print::vector_print(result.first);
// 	cout << "Distance" << ' ' << result.second << endl;

// 	//TODO memory check

// 	return 0;
// }