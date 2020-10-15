#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <cstring>
#include <assert.h>
#include <ctime>

#include "../../utils/math_utils.h"
#include "../common/io_utils.h"
#include "../../algorithms/LSH/headers/lsh.h"
#include "../../algorithms/BruteForce/headers/brute_force.h"

using namespace std;

int main(int argc, char* argv[]) {
	// first step: parse the arguments
	char* input_file, *query_file, *output_file;
	int k = 0, L = 0, n_neighbors = 0, radius = 0;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-d")) {
			input_file = strdup(argv[++i]);
		}
		if (!strcmp(argv[i], "-q")) {
			query_file = strdup(argv[++i]);
		}	
		if (!strcmp(argv[i], "-o")) {
			output_file = strdup(argv[++i]);
		}
		if (!strcmp(argv[i], "-k")) {
			k = atoi(argv[++i]);
		}
		if (!strcmp(argv[i], "-L")) {
			L = atoi(argv[++i]);
		}
		if (!strcmp(argv[i], "-N")) {
			n_neighbors = atoi(argv[++i]);
		}
		if (!strcmp(argv[i], "-R")) {
			radius = atoi(argv[++i]);
		}
	}
	assert(input_file && query_file && output_file
		&& k && L && n_neighbors && radius);

	// parse the dataset in order to get a vector of our feature vector
	vector<vector<double>> feature_vectors = parse_input(input_file);

	// parse the query set in order to find all of the queries that the user wants to ask
	vector<vector<double>> query_vectors = parse_input(query_file);

	// create the output file that we are going to use to print the LSH results
	ofstream output;
	output.open(output_file);

	// defalt lsh values, that we've learned for theory
	uint64_t m = pow(2,32) - 5;
	uint32_t M = pow(2,8);
	uint32_t w = 10 * our_math::compute_w_value(feature_vectors, 1000);

	// lsh initialization values that we've learned from our dataset
	int n_points = feature_vectors.size();
	int space_dimension = feature_vectors.at(0).size();
	// initialize our LSH class with the deature vectors and the aprropriate given values
	LSH<double> lsh_instant(L, m, M, n_points, k, space_dimension, w, feature_vectors);

	// initialize our brute force class, in order to find the true distance of the query and its neighbors
	BruteForce<double> bf_instant(n_points, space_dimension, feature_vectors);

	// run all the queries with LSH
	for (uint64_t i = 0; i < 20; i++) {
		// output the query number in the file
		output << "Query: " << i << endl;

		time_t knn_start, knn_finish, bf_start, bf_finish;

		time(&knn_start);
		// run the k-nearest neighbor algorithm, in order to obtain n neighbors
		list<pair<int,double>> kNNs = lsh_instant.LSH::kNearestNeighbour(query_vectors.at(i), n_neighbors);
		time(&knn_finish);
		
		time(&bf_start);
		// run the brute force algorithm, in order to obtain the true nearest neighbors
		list<pair<int, double>> kBF = bf_instant.kNeighboursBF(query_vectors.at(i), n_neighbors);
		time(&bf_finish);

		// denote the number of neighbor we are dealing with
		int k_value = 0;

		// itterate the list of results of the knn
		list<pair<int,double>>::iterator knn_it = kNNs.begin();
		list<pair<int,double>>::iterator bf_it = kBF.begin();

		for (; knn_it != kNNs.end() && bf_it != kBF.end() ; bf_it++, knn_it++) {
			// hold the results of the knn here
			auto knn_tuple = knn_it;
			auto bf_tuple = bf_it;
			// output the findings of kNN
			output << "Nearest neighbor-" << k_value << ": " << knn_tuple->first << endl;
			// by the LSH
			output << "DistanceLSH:" << knn_tuple->second << endl;
			// and by brute force
			output << "DistanceTrue:" << bf_tuple->second << endl;
			k_value++;
		}
		
		// Print the time elapsed while computing the neighbors with kNN and BF
		output << "tLSH: " << difftime(knn_finish, knn_start) << endl;
		output << "tTrue: " << difftime(bf_finish, bf_start) << endl;

		// RUn the Range Search algorithm
		list<pair<int, double>> range_search = lsh_instant.RangeSearch(query_vectors.at(i), radius, 1); //TODO: Probably change c?

		// print in the output file all of our findings
		output << "R-near neighbors:" << endl;
		for (list<pair<int,double>>::iterator it = range_search.begin(); it != range_search.end(); it++) {
			output << it->first << endl;
		}
		
		output << "\n\n\n";
	}
}