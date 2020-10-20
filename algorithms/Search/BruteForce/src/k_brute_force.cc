#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <limits>
#include <utility>


#include "../../../utils/math_utils.h"
#include "../headers/brute_force.h"
#include "../../../utils/print_utils.h"

using namespace std;

template <typename T>
// find the true k nearest neighbours of a vector
std::list<std::pair<int,T>> BruteForce<T>::kNeighboursBF(std::vector<T> query_vector, int k) {
	list<pair<int,T>> result;

    //the greatest of the k min distances
    T kth_min_distance = (T) INT_MAX;

    // Traverse the hash tables
	for (int i = 0; i < n_points; i++) {
		// distance between vectors
		T distance =  metrics::ManhatanDistance(feature_vectors.at(i), query_vector, space_dim);
		
		// if the element is already in the list, skip the checks
		pair<int, T> new_pair = make_pair(i, distance);

		// insert the pair into the list if it's distance is less than the kth_min
		if (result.size() > 0 && result.size() < (uint64_t)k) {
			// size < k so we just insert it
			
			if (distance >= kth_min_distance) {
				// insert at back if distance > kth_min_distance
				// kth_min_distance is the distance of the last element
				kth_min_distance = distance;
				result.push_back(new_pair);
			} else {
				// iterate through the list to insert it in the right position
				typename std::list<std::pair<int,T>>::iterator pair_it;
				for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
					if ( distance < pair_it->second) {
						result.insert(pair_it, new_pair);
						break;
					}
				}
				kth_min_distance = result.end()->second;

			}
		} else if (result.size() > 0 && (result.size() == k)) {
			// insert, pop the last, and make the kth_min the new last
			if (distance < kth_min_distance) {
				// iterate through the list to insert it in the right position
				for (typename std::list<pair<int,T>>::iterator pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
					if ( distance < pair_it->second) {
						result.insert(pair_it, new_pair);
						break;
					}
				}
				result.pop_back();
				kth_min_distance = result.back().second;
			}
		} else if (result.size() == 0) {
			// size is 0, insert 1st element
			kth_min_distance = distance;
			result.push_back(new_pair);
		} else {
			cout << "size " << result.size() << endl;
			cout << "knearest problem" << endl;
			exit(EXIT_FAILURE);
		}
		// if (visited > 10 * L) {
		//     return b;
		// }
    }
    return result;
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

// 	BruteForce<double> instant(items.size(), items.at(1).size(), items);
// 	list<pair<int,double>> result = instant.kNeighboursBF(queries.at(1), 10);
// 		for (list<pair<int,double>>::iterator pair_it = result.begin(); pair_it != result.end(); pair_it++) {
// 		cout << pair_it->first << "  " << pair_it->second << endl;
// 	}
// 	cout << "size is" << result.size() << endl;
//     //TODO print the vectors and distances to check them
// 	auto real_pair = instant.RunBruteForce(queries.at(1));
// 	cout << "real " << real_pair.first << " " << real_pair.second <<endl;


//     //TODO mem check

//     return 0;
// }
