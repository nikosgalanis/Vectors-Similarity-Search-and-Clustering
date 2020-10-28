#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 

#include "../../utils/math_utils.h"

std::vector<std::vector<double>> parse_input(std::string filename);
void parse_lsh_args(int argc, char* argv[], char** input, char** output, char** query, int* k, int* L, int* n_neighs, int* radius);
void parse_hc_args(int argc, char* argv[], char** input, char** output, char** query, int* k, int* M, int* probes, int* n_neighs, int* radius);
void parse_clustering_args(int argc, char* argv[], char** input, char** output, char** config, bool* complete, char** method);
void parse_clustering_config(char* file, int* k, int* l_lsh, int* k_lsh, int* M_hc, int* k_hc, int* probes_hc);