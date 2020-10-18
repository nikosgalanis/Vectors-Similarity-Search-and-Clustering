#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector>
#include <utility> 

#include "../../LSH/headers/hashing.h"
#include "../headers/hypercube.h"
#include "../../common/metrics.h"
#include "../../../utils/print_utils.h"


template <typename T>
// find all the vectors with distance leq than r * c from a given vector
list<pair<int,T>> Hypercube<T>::RangeSearch(vector<T> query_vector, double radius, int c) {
	list<pair<int,T>> result;

	// store the visited vectors, to find out when we do reach the threshold
	int visited = 0;

	/**
	 Step 1: Hash the given vector with the random projection technique
	 **/
	std::string bucket_str;
	// hash k times
	int k = 0;
	for (std::list<lsh::HashFunction>::iterator it = hash_fns.begin(); 
		it != hash_fns.end(); it++) {
			// hash with the i-th hash fn
			int res = it->hash(query_vector);
			// find its pre-maped result of a coin flip
			bool bit = flipped_coins.at(k)[res];
			// append the 0/1 value to the string
			bucket_str += to_string(bit);
	}
	// convert the string into an integer
	int bucket_int = (int)std::bitset<32>(bucket_str).to_ulong();

	/**
	 Step 2: Search in the bucket that the query hashes for neighbors
	**/
	list<int> bucket = hypercube[bucket_int];
	// get all the indexes of the vectors in the bucket
	for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
		// increase the visited items
		visited++;
		
		// calculate the distance between the selected vector and our query vector
		T distance = metrics::ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
		
		// create the new pair
		pair<int, T> new_pair = make_pair(*bucket_it, distance);

		// if the distance is smaller than the radius, push it in our result list
		if (distance < (T)(radius * c)) {
			result.push_back(new_pair);
		}

		// if we've reached the threshold, just return the result list
		if (visited >= threshold)
			return result;
	}

	/**
	 Step 3 .. n: If necessary, check all the buckets with hamming distance 1 .. n
	 			until the threshold is reached.
	 **/
	// set the desired hamming distance to 1 initially
	int dist_count = 1;
	// while we are under the threshold
	while (true) {
		// parse through all the buckets
		for (int i = 0; i < n_buckets; i++) {
			// if the hamming distance from the bucket_int is the desired
			if (our_math::hamming_distance(i, bucket_int) == dist_count) {
				// check the bucket for neighbors in the range given
					list<int> bucket = hypercube[i];
					// get all the indexes of the vectors in the bucket
					for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
						// increase the visited items
						visited++;
						
						// calculate the distance between the selected vector and our query vector
						T distance = metrics::ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
						
						// create the new pair
						pair<int, T> new_pair = make_pair(*bucket_it, distance);

						// if the distance is smaller than the radius, push it in our result list
						if (distance < (T)(radius * c)) {
							result.push_back(new_pair);
						}
						
						// if we've reached the threshold, just return the result list
						if (visited >= threshold)
							return result;
					}
			}
		}
		// again, if necessary, increase the hamming distance, so we check further buckets
		dist_count++;
	}
}


vector<vector<double>> parse_input(string filename) {
	// open the dataset
	ifstream input(filename, ios::binary);

	// read the magic number of the image
	int magic_number = 0;
	input.read((char*)&magic_number, sizeof(int));
	magic_number = our_math::big_to_litte_endian(magic_number);
	// find out how many images we are going to parse
	int n_of_images;
	input.read((char*)&n_of_images, sizeof(int));
	n_of_images = our_math::big_to_litte_endian(n_of_images);

	// create a list of vectors to return
	vector<vector<double>> list_of_vectors;
	// read the dimensions
	int rows = 0;
	input.read((char*)&rows, sizeof(int));
	rows = our_math::big_to_litte_endian(rows);
	int cols;
	input.read((char*)&cols, sizeof(int));
	cols = our_math::big_to_litte_endian(cols);

	double x;
	// for each image start filling the vectors
	for (int i = 0; i < n_of_images; i++) {
		// create a vector to store our image data
		vector<double> vec;
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



int main(int argc, char* argv[]) {
	vector<vector<double>> items = parse_input("../../../misc/datasets/train-images-idx3-ubyte");
	vector<vector<double>> queries = parse_input("../../../misc/querysets/t10k-images-idx3-ubyte");

	vector<double> first = queries.at(2);
	// vector<vector<float>> a;
	// a.push_back(x); a.push_back(y);

	Hypercube<double> instant(14, (pow(2,32)-5), 10, items.size(), 20000, items.at(1).size(), items);
	list<pair<int,double>> result = instant.RangeSearch(first, 10000, 2);
	for (list<pair<int,double>>::iterator pair_it = result.begin(); pair_it != result.end(); pair_it++) {
		cout << pair_it->first << "  " << pair_it->second << endl;
	}
	cout << "size is" << result.size() << endl;

    return 0;
}