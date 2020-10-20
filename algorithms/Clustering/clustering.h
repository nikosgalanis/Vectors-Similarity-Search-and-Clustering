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

		/** assignment method chosen by the user. Options are:
		  		- lloyds
				- reverse_LSH
				- reverse_Hypercube 
		 **/

		std::string assignment_method;
		
		// vectors of the centroids
		std::vector<std::vector <T>> centroids;

		// vector of the vectors given to us, stored here for convinience
		std::vector<std::vector<T>> feature_vectors;

		// vector that hols the inex of the centroid that the current indexe's vector is assigned to
		std::vector<int> assigned_centroid;

		// in case of lsh, we need these extra variables
		uint32_t lsh_l;
		uint32_t lsh_k;
		LSH<T> lsh_instant;

		// in case of hypercube, we need these extra variables
		uint32_t hc_M;
		uint32_t hc_k;
		uint32_t hc_probes;
		Hypercube<T> hc_instant;

	public:
		// constructor when lloyds assignment is requested
		Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors)
		: feature_vectors(feature_vectors) {
			if (assignment_method != "lloyds") {
				cout << "Wrong assignment method selected";
				exit(EXIT_FAILURE);
			}
			this->assignment_method = assignment_method;
		};

		// constructor when reverse assignment using lsh is requested
		Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors,
			uint32_t lsh_l, uint32_t lsh_k): feature_vectors(feature_vectors), lsh_l(lsh_l), lsh_k(lsh_k) {
				if (assignment_method != "reverse_LSH") {
					cout << "Wrong assignment method selected";
					exit(EXIT_FAILURE);
				}	
				this->assignment_method = assignment_method;

				// default lsh arguments
				uint32_t m = pow(2,32) - 5;
				uint32_t M = 256;
				uint32_t w = 10000;
				
				// make sure that the feature vectors have at least one vector in it
				assert(feature_vectors.size());

				// initialize the lsh class
				lsh_instant(lsh_l, m, M, feature_vectors.size(), lsh_k, feature_vectors.at(0).size(), w, feature_vectors);
		};

		// constructor when reverse assignment using hypercube is requested
		Clustering(std::string assignment_method, std::vector<std::vector<T>> feature_vectors,
			uint32_t hc_M, uint32_t hc_k, uint32_t hc_probes): feature_vectors(feature_vectors), 
			hc_M(hc_M), hc_k(hc_k), hc_probes(hc_probes) {
				if (assignment_method != "reverse_Hypercube") {
					cout << "Wrong assignment method selected";
					exit(EXIT_FAILURE);
				}	
				this->assignment_method = assignment_method;

				// default hypercube arguments
				uint32_t m = pow(2,32) - 5;
				uint32_t w = 10000;

				// make sure that the feature vectors have at least one vector in it
				assert(feature_vectors.size());

				hc_instant(hc_k, m, hc_M, hc_probes, feature_vectors.size(), w, feature_vectors.at(0).size(), feature_vectors);
		};

		// default destructor for the clustering class
		~Clustering();

		void initialization(void);
		void update(void);

		void assignment_lloyds(void);
		void assignment_LSH(void);
		void assignment_hypercube(void);

		int compute_silhouette(void);
};