#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <limits>

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
		std::pair<std::vector<T>,T> RunBruteForce(vector<T> query) {
			// initialize the minimum distance and the result vector index
			T min_distance = (T)INT_MAX;
			int result_vector_index = -1;
			// run through all of the dataset points
			for (int i = 0; i < n_points; i++) {
				// compute each vector's distance form the query vector
				T distance = metrics::ManhatanDistance(feature_vectors.at(i), query_vector, space_dim);
				// if its minimum
				if (distance < min_distance) {
					// update the values
					min_distance = distance;
					result_vector_index = i;
				}
			}

			// return a pair of the vector and its distance from the query
			return std::pair(feature_vectors.at(result_vector_index), min_distance); 
		}
		~BruteForce(){};
};