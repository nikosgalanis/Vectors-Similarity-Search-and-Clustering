#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <utility>


#include "hashing.h"

#include "../../common/metrics.h"
#include "../../../utils/print_utils.h"

typedef std::vector<std::list<int>> lsh_hash_table; 

template <typename T>
class LSH {
	private:
		uint32_t L; // number of hash tables used in LSH
		uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
		uint32_t M; // size of each hash table
		uint32_t n_points; // number of points to be hashed initially
		uint32_t k; // given from the user, affects number of the buckets
		uint32_t w; // a number signifficantly larger thar the radius, affects range search
		uint32_t space_dim; // the dimension of the space we are going to work with
		// a list of the hash functions for every hash table
		std::list<lsh::AmplifiedHashFunction> amplified_hash_fns;

		// vector of the hash tables used by LSH. We are just hashing an vector's index in order to save space
		std::vector<lsh_hash_table> lsh_tables;

		// vector of the vectors given to us, stored here for convinience
		std::vector<std::vector<T>> feature_vectors;

		public:
			// constructor of the LSH class
			LSH(uint32_t L, uint64_t m, uint32_t M,
				uint32_t n_points, uint32_t k,
				uint32_t space_dim, uint32_t w, std::vector<std::vector<T>> init_vectors) : 
				L(L), m(m), M(M), n_points(n_points), k(k), w(w), space_dim(space_dim), feature_vectors(init_vectors) {
					/**
					 Preprocessing: We want to uniformly select L amplified h.fs g1, .., g_l.
					 Afterwards, we are going to initialize all of the hash tables, before hashing the 
					 vectors.
					 **/
					// calculate the time of the initialization
					time_t start, finish;
					fprintf(stdout, "LSH initialization with parameters: L = %d, m = %ld, M = %d, n_points = %d "
					"k = %d, w = %d, space_dim = %d\n", L, m, M, n_points, k, w, space_dim);
					time(&start);
					for (uint32_t i = 0; i < L; i++) {
						lsh::AmplifiedHashFunction ampl(space_dim, w, m, M, k);
						// Insert each amplified hash fn into the apropriate list
						amplified_hash_fns.push_back(ampl);
					}
					
					// initialize all of our hash tables
					for (uint32_t i = 0; i < L; i++) {
						// for each hash table
						lsh_hash_table current_hash(M);
						for (uint32_t j = 0; j < M; j++) {
							// create n buckets, that each one has a list of integers (a.k.a indices of the vectros)
							list<int> current_list;
							current_hash[j] = current_list;
						}
						lsh_tables.push_back(current_hash);
					}
					
					// insert every vector given to us into every hash table
					int i = 0;
					for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
					it != amplified_hash_fns.end(); it++) {
						// refer to the correct hash table in the vetor of hashes
						lsh_hash_table current_table = lsh_tables.at(i);
						// insert each vector in the i-th hash table
						for (uint32_t n = 0; n < n_points; n++) {
							// compute the bucket that the vector will be inserted
							int index = it->assign_to_bucket(feature_vectors.at(n));
							// insert the index of the vector in the correct bucket
							lsh_tables.at(i)[index].push_back(n);
						}
						i++;
					}

					// lsh initialization done. print the time differenct
					time(&finish);
				   	cout << "Initialization completed in " << difftime(finish, start) << " seconds" << endl;
				};
				
				// default destructor of the LSH class
				~LSH() {};

				// Search functions for LSH

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
								vector_index = *bucket_it;
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
				};
				
				// return a sorted list of the k nearest neighbours of a vector
				list<pair<int,T>> kNearestNeighbour(std::vector<T> query_vector, int k) {
						
					list<pair<int,T>> result;

					//the greatest of the k min distances
					T kth_min_distance = (T) INT_MAX;

					int i = 0;
					int visited = 0;

					// Traverse the hash tables
					for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
						it != amplified_hash_fns.end(); it++) {
						// find the bucket that the query hashes into
						int index = it->assign_to_bucket(query_vector);
						list<int> bucket = lsh_tables.at(i)[index];
						// get all the indexes of the vectors in the bucket
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
									for (pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
										if (distance < pair_it->second) {
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
							// if (visited > 10 * L) {
							//     return b;
							// }
						}
						i++;
					}
					return result;
				};

				// return a list of all the vectors near the query, in range (radius * c)
				list<pair<int,T>> RangeSearch(vector<T> query_vector, double radius, int c) {
					list<pair<int,T>> result;

					int i = 0;
					int visited = 0;
					// Traverse all the hash tables. aka 1 ... L
					for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
						it != amplified_hash_fns.end(); it++) {
						// refer to the correct hash table in the vetor of hashes
						// find the bucket that the query hashes into
						int index = it->assign_to_bucket(query_vector);
						// access that specific bucket
						list<int> bucket = lsh_tables.at(i)[index];
						// get all the indexes of the vectors in the bucket
						for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {
							// increase the visited items
							visited++;
							
							// calculate the distance between the selected vector and our query vector
							T distance = metrics::ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
							
							// create the new pair
							pair<int, T> new_pair = make_pair(*bucket_it, distance);

							// check if the pair is already on a list
							if (find(result.begin(), result.end(), new_pair) != result.end()) {
								continue;
							}

							// if the distance is smaller than the radius, push it in our result list
							if (distance < (T)(radius * c)) {
								result.push_back(new_pair);
							}
							// if (visited > 20 * L) {
							//     return result;
							// }
						}
						i++;
					}
					return result;
				};
};