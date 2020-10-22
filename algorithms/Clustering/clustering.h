#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <utility>

#include "../Search/LSH/headers/lsh.h"
#include "../Search/Hypercube/headers/hypercube.h"

template <typename T>
class Clustering {
	private:
		uint32_t k; // number of clusters needed
		uint32_t n_points; //number of points in the dataset
		uint32_t space_dim; // space dimension

		/** assignment method chosen by the user. Options are:
		  		- lloyds
				- reverse_LSH
				- reverse_Hypercube 
		 **/

		std::string assignment_method;
		
		// vectors of the centroids
		std::vector<std::vector<T>> centroids;

		// vector of the vectors given to us, stored here for convinience
		std::vector<std::vector<T>> feature_vectors;

		// vector that hols the index of the centroid that the current index's vector is assigned to
		std::vector<int> assigned_centroid;

		// in case of lsh, we need these extra variables
		uint32_t lsh_l;
		uint32_t lsh_k;
		// LSH<T> lsh_instant;

		// in case of hypercube, we need these extra variables
		uint32_t hc_M;
		uint32_t hc_k;
		uint32_t hc_probes;
		// Hypercube<T> hc_instant;

	public:
		// constructor when lloyds assignment is requested
		Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors, uint32_t k)
		: feature_vectors(feature_vectors), k(k) {
			if (assignment_method != "lloyds") {
				cout << "Wrong assignment method selected";
				exit(EXIT_FAILURE);
			}
			this->assignment_method = assignment_method;
			n_points = feature_vectors.size();
			cout << n_points << endl;
			assert(n_points);
			space_dim = feature_vectors.at(0).size();

		};

		// // constructor when reverse assignment using lsh is requested
		// Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors,
		// 	uint32_t lsh_l, uint32_t lsh_k): feature_vectors(feature_vectors), lsh_l(lsh_l), lsh_k(lsh_k) {
		// 		if (assignment_method != "reverse_LSH") {
		// 			cout << "Wrong assignment method selected";
		// 			exit(EXIT_FAILURE);
		// 		}	
		// 		this->assignment_method = assignment_method;

		// 		// default lsh arguments
		// 		uint32_t m = pow(2,32) - 5;
		// 		uint32_t M = 256;
		// 		uint32_t w = 10000;
				
		// 		n_points = feature_vectors.size();
		// 		assert(n_points);
		// 		space_dim = feature_vectors.at(0).size();

		// 		// initialize the lsh class
		// 		lsh_instant(lsh_l, m, M, n_points, lsh_k, space_dim, w, feature_vectors);

		// };

		// // constructor when reverse assignment using hypercube is requested
		// Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors,
		// 	uint32_t hc_M, uint32_t hc_k, uint32_t hc_probes): feature_vectors(feature_vectors), 
		// 	hc_M(hc_M), hc_k(hc_k), hc_probes(hc_probes) {
		// 		if (assignment_method != "reverse_Hypercube") {
		// 			cout << "Wrong assignment method selected";
		// 			exit(EXIT_FAILURE);
		// 		}	
		// 		this->assignment_method = assignment_method;

		// 		// default hypercube arguments
		// 		uint32_t m = pow(2,32) - 5;
		// 		uint32_t w = 10000;

				
		// 		n_points = feature_vectors.size();
		// 		assert(n_points);
		// 		space_dim = feature_vectors.at(0).size();

		// 		hc_instant(hc_k, m, hc_M, hc_probes, n_points, w, space_dim, feature_vectors);
		// };

		// default destructor for the clustering class
		~Clustering(){};

		// initialize our clusters using the init++ method
		void initialization(void) {
		// chose a first index randomly
		std::random_device mch;
		std::default_random_engine generator(mch());
		std::uniform_int_distribution<int> distribution_int(0,n_points - 1);
		int index = distribution_int(generator);
		cout << index << endl;
		// this is our first centroid
		centroids.push_back(feature_vectors.at(index));
			// find the other centroids with the algorithm init++
			cout << "k issss " << k << endl;
			for (int t = 1; t < k; t++) {

				// define a distance vector
				vector<T> D(n_points, INT_MAX);
				// define a vector of the partial sums
				vector<T> P(n_points, 0);
				// for every point
				for (int i = 0; i < n_points; i++) {
					// cout << "i = " << i << endl;
					// check if it is centroid
					vector<T> curr = feature_vectors.at(i);
					if (std::find(centroids.begin(), centroids.end(), curr) == centroids.end()) {
						//compute D(i)
						for (int j = 0; j < t; j++) {
							T dist = metrics::ManhatanDistance(feature_vectors.at(i), centroids.at(j), space_dim);
							if (dist < D.at(i))
								D.at(i) = dist;
						}
					} else {
						// if it is a centroid, set its D value to 0
						D.at(i) = 0;
					}
					// compute the partial sums effectively
					if (i == 0)
						P.at(i) = pow(D.at(0), 2);
					else 
						P.at(i) = P.at(i - 1) + pow(D.at(i), 2);
				}
				//pick a uniformly distributed x
				std::uniform_real_distribution<double> distribution_real(0, P.at(n_points -1));
				double x = distribution_real(generator);

				// find its correct position
				int r_index = -1;
				int i = 0;
				for (i = 0; i < n_points; i++) {
					if (P.at(i) > (T)x) {
						r_index = i;
						break;
					}
				}
				// ensure that the point is not a csntroid
				while ((i - 1 > 0) && P.at(i) == P.at(i - 1)) {
					r_index--;
				}
				assert(r_index > 0);
				cout << r_index << endl;
				// now the r_index has a correct value, so its our new centroid
				centroids.push_back(feature_vectors.at(r_index));
			}
		}
		void update(void);

		void assignment_lloyds(void);
		void assignment_LSH(void);
		void assignment_hypercube(void);

		int compute_silhouette(void);
};