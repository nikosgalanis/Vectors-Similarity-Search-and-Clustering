#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <cstring>
#include <assert.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <assert.h>

#include "../../utils/math_utils.h"
#include "../../utils/print_utils.h"
#include "../common/io_utils.h"
#include "../../algorithms/Search/Hypercube/headers/hypercube.h"
#include "../../algorithms/Search/BruteForce/headers/brute_force.h"

using namespace std::chrono;

int main(int argc, char* argv[]) {
    // parsing the arguments

    char* input_file = NULL, *query_file = NULL, *output_file = NULL;
	int k, M, probes, n_neighbors, radius;
	double sum1 = 0, sum2 = 0;

	parse_hc_args(argc, argv, &input_file, &output_file, &query_file, &k, &M, &probes, &n_neighbors, &radius);

    // parse the dataset in order to get a vector of our feature vector
	vector<vector<double>> feature_vectors = parse_input(input_file);

	// parse the query set in order to find all of the queries that the user wants to ask
	vector<vector<double>> query_vectors = parse_input(query_file);

	// make sure that there is at least one element in the dataset and the queryset
	assert(feature_vectors.size() && query_vectors.size());

	// create the output file that we are going to use to print the Hypercube results
	ofstream output;
	output.open(output_file);

    // default hypercube values
    uint64_t m = pow(2,32) - 5;
    uint32_t w = 20 * our_math::compute_w_value(feature_vectors, 1000);
	uint32_t c = 1; 

    int correct_computed = 0;

    int n_points = feature_vectors.size();
	int space_dimension = feature_vectors.at(0).size();

    // Initialize hypercube class
    Hypercube<double> hypercube_instant(k, m, M, probes, n_points, w, space_dimension, feature_vectors);

    // initialize our brute force class, in order to find the true distance of the query and its neighbors
	BruteForce<double> bf_instant(n_points, space_dimension, feature_vectors);

    // run all the queries with Hypercube
    for (uint64_t i = 0; i < query_vectors.size(); i++) {
		// output the query number in the file
		output << "Query: " << i << endl;

		auto k_start = std::chrono::high_resolution_clock::now();
		// run the k-nearest neighbor algorithm, in order to obtain n neighbors
		list<pair<int,double>> kNNs = hypercube_instant.kNearestNeighbour(query_vectors.at(i), n_neighbors);
		auto k_end = std::chrono::high_resolution_clock::now();

		duration<double> hc_time_span = duration_cast<duration<double>>(k_end - k_start);  
		
		auto b_start = std::chrono::high_resolution_clock::now();
		// run the brute force algorithm, in order to obtain the true nearest neighbors
		list<pair<int, double>> kBF = bf_instant.kNeighboursBF(query_vectors.at(i), n_neighbors);
		auto b_end = std::chrono::high_resolution_clock::now();

		duration<double> bf_time_span = duration_cast<duration<double>>(b_end - b_start);  

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
			output << "Nearest neighbor-" << k_value + 1 << ": " << knn_tuple->first << endl;
			// by the Hypercube
			output << "DistanceHypercube:" << knn_tuple->second << endl;
			// and by brute force
			output << "DistanceTrue:" << bf_tuple->second << endl;
			if (knn_tuple->second == bf_tuple->second) {
				correct_computed++;
			}

			k_value++;
		}
		
		// Print the time elapsed while computing the neighbors with kNN and BF
		output << "tHC: " << hc_time_span.count() << endl;
		output << "tTrue: " << bf_time_span.count() << endl;
		sum1 += hc_time_span.count();
		sum2 += bf_time_span.count();

		// RUn the Range Search algorithm
		list<pair<int, double>> range_search = hypercube_instant.RangeSearch(query_vectors.at(i), radius, c);

		// print in the output file all of our findings
		output << "R-near neighbors:" << endl;
		for (list<pair<int,double>>::iterator it = range_search.begin(); it != range_search.end(); it++) {
			output << it->first << endl;
		}
		
		output << "\n\n\n";
	}
	cout << "mean hc time " << sum1 / query_vectors.size() << endl;
	cout << "mean bf time " << sum2 / query_vectors.size() << endl;

    cout << "corectly computed neighbours " << correct_computed  << " out of " << n_neighbors * query_vectors.size() << endl;

	output.close();

	free(input_file);
	free(query_file);
	free(output_file);

    return 0;
}