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
		uint32_t max_probes; // max probes of the hypercube to be checked during search
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
		Hypercube(const uint32_t k, uint32_t m, uint32_t threshold, uint32_t max_probes, 
		uint32_t n_points, uint32_t w, uint32_t space_dim, std::vector<std::vector<T>> init_vectors) : 
		k(k), m(m), threshold(threshold), max_probes(max_probes), n_points(n_points), 
		w(w), space_dim(space_dim), feature_vectors(init_vectors) {
			//measure the time that it takes to initialize the hypercube
			time_t start, finish;
			fprintf(stdout, "Hypercube initialization began\n");
			time(&start);

			// we have a bit swquence of k bits, thus the size of the hypercube table will be 2^k
			n_buckets = (uint32_t)pow(2, k);

			// Initialize the hash functions of each bucket
			for (uint32_t i = 0; i < k; i++) {
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
				int bucket_int = hash_to_hypercube(feature_vectors.at(i));
				hypercube[bucket_int].push_back(i);
			}

			// hypercube initialization done. print the time differenct
			time(&finish);
			cout << "Initialization completed in " << difftime(finish, start) << " seconds" << endl;
		}

		~Hypercube(){};

		// hash to the hhypercube using the procedure from the theory
		int hash_to_hypercube(std::vector<T> vec) {
			// store the string produced
			std::string bucket_str;
			// hash k times
			int k = 0;
			for (std::list<lsh::HashFunction>::iterator it = hash_fns.begin(); 
				it != hash_fns.end(); it++) {
					// hash with the i-th hash fn
					int res = it->hash(vec);
					// find its pre-maped result of a coin flip
					bool bit = flipped_coins.at(k)[res];
					// append the 0/1 value to the string
					bucket_str += to_string(bit);
			}
			// convert the string into an integer
			return (int)std::bitset<32>(bucket_str).to_ulong();
		}

		std::pair<int,T> NearestNeighbour(std::vector<T> query_vector) {
			int vector_index;
			T distance_b = (T) INT_MAX;

			uint32_t visited = 0;
			uint32_t probes_checked = 0;

			/**
			 Step 1: Hash the given vector with the random projection technique
			**/
			int bucket_int = hash_to_hypercube(query_vector);

			/**
			 Step 2: Search in the bucket that the query hashes for neighbors
			**/
			list<int> bucket = hypercube[bucket_int];
			// get all the indexes of the vectors in the bucket
			for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
				// vector<T> curr_vector = feature_vectors.at(*bucket_it);
				visited++;
				T distance =  ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
				if (distance < distance_b) {
					vector_index = *bucket_it;
					distance_b = distance;
				}
				// if the distance is smaller than the radius, push it in our result list
				if (visited >= threshold)
					return std::make_pair(vector_index, distance_b);
			}

			// return if max probes were traversed
			if (++probes_checked >= max_probes)
				return std::make_pair(vector_index, distance_b);
			
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
						list<int> bucket = hypercube[bucket_int];
						// get all the indexes of the vectors in the bucket
						for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
							// vector<T> curr_vector = feature_vectors.at(*bucket_it);
							visited++;
							T distance =  ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
							if (distance < distance_b) {
								vector_index = *bucket_it;
								distance_b = distance;
							}
							if (visited >= threshold)
								return std::make_pair(vector_index, distance_b);
						}
						// return if max probes were traversed
						if (++probes_checked >= max_probes)
							return std::make_pair(vector_index, distance_b);
					}
				}
				// again, if necessary, increase the hamming distance, so we check further buckets
				dist_count++;
			}

		};

		list<pair<int,T>> kNearestNeighbour(std::vector<T> query_vector, int k) {

			list<pair<int,T>> result;
			
			//the greatest of the k min distances
			T kth_min_distance = (T) INT_MAX;

			uint32_t visited = 0;
			uint32_t probes_checked = 0;

			/**
			 Step 1: Hash the given vector with the random projection technique
			**/
			int bucket_int = hash_to_hypercube(query_vector);

			/**
			 Step 2: Search in the bucket that the query hashes for neighbors
			**/
			list<int> bucket = hypercube[bucket_int];

			for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {

				visited++;

				// distance between vectors
				T distance =  metrics::ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
				
				// if the element is already in the list, skip the checks
				pair<int, T> new_pair = make_pair(*bucket_it, distance);
				
				if (find(result.begin(), result.end(), new_pair) != result.end()) {
					continue;
				}

				// insert the pair into the list if it's distance is less than the kth_min
				if (result.size() > 0 && result.size() < (uint64_t) k) {
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
					}
				} else if (result.size() > 0 && (result.size() == (uint64_t)k)) {
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

				// if we've reached the threshold, just return the result list
				if (visited >= threshold) 
					return result;
			}

			// return if max probes were traversed
			if (++probes_checked >= max_probes)
				return result;
			
			/**
			 Step 3 .. n: If necessary, check all the buckets with hamming distance 1 .. n
						until the threshold is reached.
			**/
			// set the desired hamming distance to 1 initially
			int dist_count = 1;
			// while we are under the threshold
			while (true) {
				// parse through all the buckets
				for (uint32_t i = 0; i < n_buckets; i++) {
					// if the hamming distance from the bucket_int is the desired
					if (our_math::hamming_distance(i, bucket_int) == dist_count) {
						// check the bucket for neighbors in the range given
						list<int> bucket = hypercube[i];
						for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {

							visited++;

							// distance between vectors
							T distance =  metrics::ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
							
							// if the element is already in the list, skip the checks
							pair<int, T> new_pair = make_pair(*bucket_it, distance);
							
							if (find(result.begin(), result.end(), new_pair) != result.end()) {
								continue;
							}

							// insert the pair into the list if it's distance is less than the kth_min
							if (result.size() > 0 && result.size() < (uint64_t) k) {
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
								}
							} else if (result.size() > 0 && (result.size() == (uint64_t) k)) {
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

							// if we've reached the threshold, just return the result list
							if (visited >= threshold) 
								return result;
						}
						// return if max probes were traversed
						if (++probes_checked >= max_probes)
							return result;
					}
				}
				// again, if necessary, increase the hamming distance, so we check further buckets
				dist_count++;
			}
		};

		list<pair<int,T>> RangeSearch(vector<T> query_vector, double radius, int c) {
			list<pair<int,T>> result;

			// store the visited vectors, to find out when we do reach the threshold
			uint32_t visited = 0;
			uint32_t probes_checked = 0;

			/**
			 Step 1: Hash the given vector with the random projection technique
			**/
			int bucket_int = hash_to_hypercube(query_vector);

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

			// return if max probes were traversed
			if (++probes_checked >= max_probes)
				return result;

			/**
			 Step 3 .. n: If necessary, check all the buckets with hamming distance 1 .. n
						until the threshold is reached.
			**/
			// set the desired hamming distance to 1 initially
			int dist_count = 1;
			// while we are under the threshold
			while (true) {
				// parse through all the buckets
				for (uint32_t i = 0; i < n_buckets; i++) {
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
					// return if max probes were traversed
					if (++probes_checked >= max_probes)
						return result;
				}
				// again, if necessary, increase the hamming distance, so we check further buckets
				dist_count++;
			}
		};
};       
