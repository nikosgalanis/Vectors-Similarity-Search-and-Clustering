#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <climits>
#include <utility>
#include <assert.h>

#include "../../common/metrics.h"

#include "../../../utils/math_utils.h"
#include "../../../utils/print_utils.h"

template <typename T>
// class to implement the brute force algorithm
class BruteForce {
	private:
        const uint32_t n_points; // number of points to be checked
        const uint32_t space_dim; // the dimension of the space we are going to work with

        // vector of the vectors given to us, stored here for convinience
        std::vector<std::vector<T>> feature_vectors;

	public:
		// constructor of the brute force class. Just
		BruteForce(const uint32_t n_points, const uint32_t space_dim, 
		std::vector<std::vector<T>> init_vectors): n_points(n_points), 
		space_dim(space_dim), feature_vectors(init_vectors){};

		// run the brute force algorithm to find the nearest neighbor
		std::pair<int,T> RunBruteForce(std::vector<T> query) {
			// initialize the minimum distance and the result vector index
			T min_distance = (T)INT_MAX;
			int result_vector_index = -1;
			// run through all of the dataset points
			for (uint32_t i = 0; i < n_points; i++) {
				// compute each vector's distance form the query vector
				T distance = metrics::ManhatanDistance(feature_vectors.at(i), query, space_dim);
				// if its minimum
				if (distance < min_distance) {
					// update the values
					min_distance = distance;
					result_vector_index = i;
				}
			}
			assert(result_vector_index > 0);
			// return a pair of the vector and its distance from the query
			return std::make_pair(result_vector_index, min_distance); 
		}

		~BruteForce(){};

		std::list<std::pair<int,T>> kNeighboursBF(std::vector<T> query_vector, int k) {

			list<pair<int,T>> result;

			//the greatest of the k min distances
			T kth_min_distance = (T) INT_MAX;

			// Traverse the hash tables
			// for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
			//     it != amplified_hash_fns.end(); it++) {
			for (uint32_t i = 0; i < n_points; i++) {
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
			return result;
		};
};

template class BruteForce <long>;
