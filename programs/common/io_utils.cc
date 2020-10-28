#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <cstring>
#include <assert.h>


#include "../../utils/math_utils.h"
#include "io_utils.h"
using namespace std;

/**
Function to parse the input given. Both the train and the test datasets are in the form:
 - Magic number (int)
 - N of images in dataset (int)
 - n of columns and rows for the images
 - Binary representation of the images pixels (0-255)

 NOTE: all the numbers are given in Big-Endian
**/
vector<vector<double>> parse_input(string filename) {
	// open the dataset
	ifstream input(filename, ios::binary);

	// read the magic number of the image
	int magic_number = 0;
	input.read((char*)&magic_number, sizeof(int));
	magic_number = our_math::big_to_litte_endian(magic_number);
	// find out how many images we are going to parse
	int n_of_images;
	input.read((char*)&n_of_images, sizeof(int));
	n_of_images = our_math::big_to_litte_endian(n_of_images);

	// create a list of vectors to return
	vector<vector<double>> list_of_vectors;
	// read the dimensions
	int rows = 0;
	input.read((char*)&rows, sizeof(int));
	rows = our_math::big_to_litte_endian(rows);
	int cols;
	input.read((char*)&cols, sizeof(int));
	cols = our_math::big_to_litte_endian(cols);

	double x;
	// for each image start filling the vectors
	for (int i = 0; i < 1000; i++) {
		// create a vector to store our image data
		vector<double> vec;
		// store each byte of the image in the vector, by parsing boh of the image's dimensions
		for (int j = 0; j < rows; j++) {
			for (int k = 0; k < cols; k++) {
				// the pixels are from 0-255, so an unsinged char type is enough
				unsigned char pixel = 0;
				input.read((char*)(&pixel), sizeof(pixel));
				// change its value to double
				x = (double)pixel;
				// push the byte into the vector
				vec.push_back(x);
			}
		}
		// push the vector in our list
		list_of_vectors.push_back(vec);
	}
	
	input.close();
	// return the list of vectors that we created
	return list_of_vectors;
}


void parse_lsh_args(int argc, char* argv[], char** input, char** output,
	char** query, int* k, int* L, int* n_neighs, int* radius) {
		// default arguments
		*k = 4; *L = 5; *n_neighs = 1; *radius = 10000;
		// parse through comand line arguments
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-d")) {
				*input = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-q")) {
				*query = strdup(argv[++i]);
			}	
			if (!strcmp(argv[i], "-o")) {
				*output = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-k")) {
				*k = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-L")) {
				*L = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-N")) {
				*n_neighs = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-R")) {
				*radius = atoi(argv[++i]);
			}
		}
		assert(*input && *query && *output);
	} 

void parse_hc_args(int argc, char* argv[], char** input, char** output,
	char** query, int* k, int* M, int* probes, int* n_neighs, int* radius) {
		// default arguments
		*k = 14; *M = 10; *probes = 2; *n_neighs = 1; *radius = 10000;
		// parse through comand line arguments
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-d")) {
				*input = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-q")) {
				*query = strdup(argv[++i]);
			}	
			if (!strcmp(argv[i], "-o")) {
				*output = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-k")) {
				*k = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-M")) {
				*M = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-probes")) {
				*probes = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-N")) {
				*n_neighs = atoi(argv[++i]);
			}
			if (!strcmp(argv[i], "-R")) {
				*radius = atoi(argv[++i]);
			}
		}
		assert(*input && *query && *output);
	}

void parse_clustering_args(int argc, char* argv[], char** input,
	char** output, char** config, bool* complete, char** method) {
		// default arguments
		*complete = false;
		// parse through comand line arguments
		for (int i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "-i")) {
				*input = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-c")) {
				*config = strdup(argv[++i]);
			}	
			if (!strcmp(argv[i], "-o")) {
				*output = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-m")) {
				*method = strdup(argv[++i]);
			}
			if (!strcmp(argv[i], "-complete")) {
				*complete = true;
			}
		}
		assert(*input && *config && *output && *method);
	}

void parse_clustering_config(char* file, int* k, int* l_lsh, int* k_lsh,
	int* M_hc, int* k_hc, int* probes_hc) {
		// default arguments
		*l_lsh = 3; *k_lsh = 4; *M_hc = 10; *k_hc = 3; *probes_hc = 2;
		// open the config file 
		ifstream config;
		config.open(file);
		char data[100];
		// garbage for k
		config >> data;

		config >> data;
		*k = atoi(data);

		if (! config.eof()) {
			// garbage
			config >> data;

			config >> data;
			*l_lsh = atoi(data);
		}

		if (! config.eof()) {
			// garbage
			config >> data;

			config >> data;
			*k_lsh = atoi(data);
		}

		if (! config.eof()) {
			// garbage
			config >> data;

			config >> data;
			*M_hc = atoi(data);
		}

		if (! config.eof()) {
			// garbage
			config >> data;

			config >> data;
			*k_hc = atoi(data);
		}

		if (! config.eof()) {
			// garbage
			config >> data;

			config >> data;
			*probes_hc = atoi(data);
		}
		config.close();
	}