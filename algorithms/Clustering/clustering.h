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
		int k; // number of clusters needed
		int n_points; //number of points in the dataset
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

		//compute the nearest center for a given vector
		int nearest_centroid(vector<T> vec) {
			T min_dist = (T)INT_MAX;
			int nearest = -1;
			// compute the distances to all the centroids
			for (int i = 0; i < k; i++) {
				T dist = metrics::ManhatanDistance(vec, centroids.at(i), space_dim);
				// set it as min
				if (dist < min_dist) {
					min_dist = dist;
					nearest = i;
				}
			}
			assert(nearest != -1);

			return nearest;
		}

		int second_nearest_centroid(vector <T> vec) {
			// get the nearest centroid
			int nearest = nearest_centroid(vec);
			T min_dist = (T)INT_MAX;
			int second_nearest = -1;
			// compute the distances to all the centroids
			for (int i = 0; i < k; i++) {
				T dist = metrics::ManhatanDistance(vec, centroids.at(i), space_dim);
				// set it as min
				if (dist < min_dist && i != nearest) {
					min_dist = dist;
					second_nearest = i;
				}
			}
			assert(second_nearest != -1);

			return second_nearest;
		}

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

			vector<int> assigned(n_points, 0);
			assigned_centroid = assigned;

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

		// get the vector of the centroids
		std::vector<std::vector<T>> get_centroids() {
			return this->centroids;
		};

		std::vector<int> get_assigned_centroid() {
			return this->assigned_centroid;
		};


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

		// update the centroids
		void update(void) {
			// define the new centroids
			for (int i = 0; i < k; i++) {
				vector<T> a(space_dim, 0);
				centroids.at(i) = a;
			}
			// keep track of how many vectors are currently assigned to each centroid
			vector<int> n_assinged(k, 0);

			// compute the sum of each vector assigned in every cluster
			for (int i = 0; i < n_points; i++) {
				// find in which centroid the vector is assigned to
				int index = assigned_centroid.at(i);
				// update the centroid value
				centroids.at(index) = our_math::add_vectors(centroids.at(index), feature_vectors.at(i));
				// increase the assgined vectors in the current centroid
				n_assinged.at(index)++;
			}

			// deivide each centroid so we can get the mean value
			for (int i = 0; i < k; i++) {
				centroids.at(i) = our_math::divide(centroids.at(i), n_assinged.at(i));
			}
		};

		// assign vectors to the nearest cluster using the lloyds algorithm
		int assignment_lloyds(void) {
			// keep track of the changes in assignments
			int changes = 0;
			// itterate each vector of the dataset
			for (int i = 0; i < n_points; i++) {
				int prev = assigned_centroid.at(i);
				assigned_centroid.at(i) = nearest_centroid(feature_vectors.at(i));
				// check and record changed centroids
				if (prev != assigned_centroid.at(i))
					changes++;
			}

			return changes;
		};
		int assignment_LSH(void) {
			return 0;
		};
		int assignment_hypercube(void) {
			return 0;
		};

		// compute the silhouette of each cluster, and the total one
		std::pair<std::vector<double>, double> compute_silhouette(void) {
			// declare a vector of each cluster's silhouette
			vector<double> silhouettes(k, 0);

			// declare a vector that holds how many vectors are assinged to each centroid
			vector<int> assigned(k, 0);
			// declare the total silhouette
			double total_sil = 0;

			// compute the silhouette of each point
			for (int i = 0; i < n_points; i++) {
				// find the nearest and the second nearest 
				int nearest = assigned_centroid.at(i);
				int second_nearest = second_nearest_centroid(feature_vectors.at(i));
				assigned.at(nearest)++;
				double a_i = 0, b_i = 0;
				// compute a and b values for this vector
				for (int j = 0; j < n_points; j++) {
					if (assigned_centroid.at(j) == nearest)
						a_i += (double)metrics::ManhatanDistance(feature_vectors.at(i), feature_vectors.at(j), space_dim);
					else if (assigned_centroid.at(j) == second_nearest)
						b_i += (double)metrics::ManhatanDistance(feature_vectors.at(i), feature_vectors.at(j), space_dim);
				}

				// compute the vecor's silhouette
				int sil = (b_i - a_i) / std::max(a_i, b_i);

				// add it to the cluster's silhouette
				silhouettes.at(nearest) += sil;

				// add it to the total silhouette
				total_sil += sil;
			}
			// compute the final value of the silhouette for each cluster
			for (int i = 0; i < k; i++)
				silhouettes.at(i) /= assigned.at(i);

			// compute the final value of the total silhouette
			total_sil /= n_points;

			// return a pair of the vector of the silhouettes for each cluster and the total silhouette
			return make_pair(silhouettes, total_sil);
		}


		// run the clustering algorithm
		void run_clustering(void) {
			// Step 1: Initialize the centroids using init++
			initialization();

			// set manually a threshold for stoping the algorithm
			int threshold = n_points / 1000;
			// number of changed vector. It is going to change in each itteration
			int changed = INT_MAX;

			// repeat until the threshold is not reached
			while (changed > threshold) {
				// Step 2: call the appropriate method of assignment
				if (assignment_method == "lloyds")
					changed = assignment_lloyds();

				else if (assignment_method == "LSH")
					changed = assignment_LSH();
				else if (assignment_method == "Hypercube")
					changed = assignment_hypercube();

				// Step 3: Update the centroids
				update();
			}
		}
};