#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <utility>


#include "../../LSH/headers/hashing.h"
#include "../../common/metrics.h"
#include "../../../utils/print_utils.h"

typedef std::vector<std::list<int>> hypercube_table;

template <typename T>
class Hypercube {
	private:
		const uint32_t k; // number of hash functions that we are going to use -> size of hypercube
		uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
		uint32_t threshold; // max elements to be checked during the algorithm
		uint32_t n_points; // number of points to be hashed initially
		uint32_t w; // a number signifficantly larger thar the radius, affects range search
		uint32_t space_dim; // the dimension of the space we are going to work with
		uint32_t n_buckets; // number of buckets in the hash table

		// the hypercube is just a hash table
		hypercube_table hypercube;

		// vector of the vectors given to us, stored here for convinience
		std::vector<std::vector<T>> feature_vectors;   

		// a list of the hash functions for every point in the hypercube
		std::list<lsh::HashFunction> hash_fns;
	
		// mapping the fliped-coins decision for every possible outcoume of the hash function
		std::vector<std::unordered_map<int, bool>> flipped_coins;

	public:

		// Class constructor
		Hypercube(const uint32_t k, uint32_t m, uint32_t threshold, uint32_t n_points, uint32_t w, 
		uint32_t space_dim, std::vector<std::vector<T>> init_vectors) : 
		k(k), m(m), threshold(threshold), n_points(n_points), w(w), space_dim(space_dim),
		feature_vectors(init_vectors) {
			//measure the time that it takes to initialize the hypercube
			time_t start, finish;
			fprintf(stdout, "Hypercube initialization began\n");
			time(&start);

			// we have a bit swquence of k bits, thus the size of the hypercube table will be 2^k
			n_buckets = (uint32_t)pow(2, k);

			// Initialize the hash functions of each bucket
			for (int i = 0; i < k; i++) {
				lsh::HashFunction hash_fn(space_dim, w, m, n_buckets);
				// Insert each amplified hash fn into the apropriate list
				hash_fns.push_back(hash_fn);         
			}

			// initialize the hypercube table
			for (uint32_t i = 0; i < n_buckets; i++) {
				std::list<int> current_list;
				hypercube.push_back(current_list);
			}

			// define the generator and the random distribution for the mapping
			std::random_device mch;
			std::default_random_engine generator(mch());
			std::uniform_int_distribution<int> distribution(0,1);
			
			// create the list of the f functions
			for (uint32_t i = 0; i < k; i++) {
				std::unordered_map<int,bool> current_map;
				// pre-map all the possible hash function outcomes to 0-1, so we buy time during initialization
				for (uint32_t i = 0; i < n_buckets; i++) {
					bool result = distribution(generator);
					current_map.insert({i, result});
				}
				flipped_coins.push_back(current_map);
			}

			// insert all of the points into the hypercube
			for (uint32_t i = 0; i < n_points; i++) {
				// store the string produced
				std::string bucket_str;
				// hash k times
				int k = 0;
				for (std::list<lsh::HashFunction>::iterator it = hash_fns.begin(); 
					it != hash_fns.end(); it++) {
						// hash with the i-th hash fn
						int res = it->hash(feature_vectors.at(i));
						// find its pre-maped result of a coin flip
						bool bit = flipped_coins.at(k)[res];
						// append the 0/1 value to the string
						bucket_str += to_string(bit);
				}
				// convert the string into an integer
				int bucket_int = (int)std::bitset<32>(bucket_str).to_ulong();
				hypercube[bucket_int].push_back(i);
			}

			// hypercube initialization done. print the time differenct
			time(&finish);
			cout << "Initialization completed in " << difftime(finish, start) << " seconds" << endl;
		}

		~Hypercube(){};

		//TODO alogrithms
		std::pair<int,T> NearestNeighbour(std::vector<T> query_vector);
		list<pair<int,T>> kNearestNeighbour(std::vector<T> query_vector, int k);
		list<pair<int,T>> RangeSearch(vector<T> query_vector, double radius, int c);

};       
