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
#include "../../algorithms/Clustering/clustering.h"

using namespace std::chrono;


int main(int argc, char* argv[]) {
	// parse the arguments
	char* input_file = NULL, *config_file = NULL, *output_file = NULL, *method = NULL;
	bool complete;

	parse_clustering_args(argc, argv, &input_file, &output_file, &config_file, &complete, &method);

	// get the clustering parameters from the config file
	int k, l_lsh, k_lsh, M_hc, k_hc, probes_hc; 

	parse_clustering_config(config_file, &k, &l_lsh, &k_lsh, &M_hc, &k_hc, &probes_hc);

	// parse the dataset in order to get a vector of our feature vector
	vector<vector<double>> feature_vectors = parse_input(input_file);

	// create the output file that we are going to use to print the clustering results
	ofstream output; 
	output.open(output_file);

	output << "Algorithm: " << method << endl << endl;
	Clustering<double>* instant;
	// construct the cluster class depending on which algorithm the user has chosen
	if (!strcmp(method, "Lloyds")) {
		instant = new Clustering<double>("lloyds", feature_vectors, k);
	}
	else if (!strcmp(method, "Range_Search_LSH")) {
		instant = new Clustering<double>("reverse_LSH", feature_vectors, k, l_lsh, k_lsh);

	}
	else if (!strcmp(method, "Range_Search_Hypercube")) {
		instant = new Clustering<double>("reverse_Hypercube", feature_vectors, k, M_hc, k_hc, probes_hc);

	} else {
		cout << "Clustering method not supported\n" << endl;
		exit(EXIT_FAILURE);
	}

	auto c_start = std::chrono::high_resolution_clock::now();
	// run the clustering algorithm
	instant->run_clustering();
	auto c_end = std::chrono::high_resolution_clock::now();

	duration<double> cluster_time = duration_cast<duration<double>>(c_end - c_start);  
	
	// get the map of the centroids
	vector<vector<int>> centroids_map = instant->get_centroids_map();
	// get the actual centroids
	vector<vector<double>> centroids = instant->get_centroids();
	
	// print stats for each cluster
	for (int i = 0; i < k; i++) {
		output << "CLUSTER-" << i << " {size: " << centroids_map.at(i).size() << ", centroid: ";
		print::vector_print_infile(centroids.at(i), &output);
		output << "}\n";
	}

	// print the clustering time
	output << "clustering_time: " <<  cluster_time.count() << endl;
	
	// compute the silhouette
	auto pair = instant->compute_silhouette();
	
	// print the silhouette results
	output << "\nSilhouette: [";
	print::vector_print_infile(pair.first, &output);
	output << ", " << pair.second << "]\n\n";

	// if the user wants to print the images of each cluster
	if (complete) {
		for (int i = 0; i < k; i++) {
			output << "CLUSTER-" << i << "{";
			print::vector_print_infile(centroids.at(i), &output);
			print::vector_print_infile(centroids_map.at(i), &output);
			output << "}\n";
		}
	}
	// free the class pointer to wrap it up
	
	delete instant;

	output.close();

	free(input_file);
	free(config_file);
	free(output_file);
}