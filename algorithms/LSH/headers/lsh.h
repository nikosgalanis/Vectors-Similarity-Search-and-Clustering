#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>
#include <ctime>
#include <utility>


#include "hashing.h"

typedef std::vector<std::list<int>> lsh_hash_table; 

template <typename T>
class LSH {
    //TODO: W initialization??
    private:
        uint32_t L; // number of hash tables used in LSH
        uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
        uint32_t M; // size of each hash table
        uint32_t n_points; // number of points to be hashed initially
        uint32_t k; // given from the user, affects number of the buckets
        uint32_t w; // a number signifficantly larger thar the radius, affects range search
        uint32_t space_dim; // the dimension of the space we are going to work with
        // a list of the hash functions for every hash table
        std::list<lsh::AmplifiedHashFunction> amplified_hash_fns;

        // vector of the hash tables used by LSH. We are just hashing an vector's index in order to save space
        std::vector<lsh_hash_table> lsh_tables;

        // vector of the vectors given to us, stored here for convinience
        std::vector<std::vector<T>> feature_vectors;

        public:
            // constructor of the LSH class
            LSH(uint32_t L, uint64_t m, uint32_t M,
                uint32_t n_points, uint32_t k,
                uint32_t space_dim, uint32_t w, std::vector<std::vector<T>> init_vectors) : 
                L(L), m(m), M(M), n_points(n_points), k(k), w(w), space_dim(space_dim), feature_vectors(init_vectors) {
                    /**
                     Preprocessing: We want to uniformly select L amplified h.fs g1, .., g_l.
                     Afterwards, we are going to initialize all of the hash tables, before hashing the 
                     vectors.
                     **/
                    // calculate the time of the initialization
                    time_t start, finish;
                    fprintf(stdout, "LSH initialization with parameters: L = %d, m = %ld, M = %d, n_points = %d "
                    "k = %d, w = %d, space_dim = %d\n", L, m, M, n_points, k, w, space_dim);
                    time(&start);
                    for (uint32_t i = 0; i < L; i++) {
                        lsh::AmplifiedHashFunction ampl(space_dim, w, m, M, k);
                        // Insert each amplified hash fn into the apropriate list
                        amplified_hash_fns.push_back(ampl);
                    }
                    // // initialize our local copy of all the vectors given to hash
                    // feature_vectors(init_vectors.begin(), init_vectors.end());

                    // initialize all of our hash tables
                    for (uint32_t i = 0; i < L; i++) {
                        // for each hash table
                        lsh_hash_table current_hash(M);
                        for (uint32_t j = 0; j < M; j++) {
                            // create n buckets, that each one has a list of integers (a.k.a indices of the vectros)
                            list<int> current_list;
                            current_hash[j] = current_list;
                        }
                        lsh_tables.push_back(current_hash);
                    }
                    
                    // insert every vector given to us into every hash table
                    int i = 0;
				    for (std::list<lsh::AmplifiedHashFunction>::iterator it = amplified_hash_fns.begin(); 
                    it != amplified_hash_fns.end(); it++) {
                        // refer to the correct hash table in the vetor of hashes
                        lsh_hash_table current_table = lsh_tables.at(i);
                        // insert each vector in the i-th hash table
                        for (uint32_t n = 0; n < n_points; n++) {
                            // compute the bucket that the vector will be inserted
                            int index = it->assign_to_bucket(feature_vectors.at(n));
                            // insert the index of the vector in the correct bucket
                            lsh_tables.at(i)[index].push_back(n);
                        }
                        i++;
                    }

                    // lsh initialization done. print the time differenct
                    time(&finish);
                   	cout << "Initialization completed in " << difftime(finish, start) << " seconds" << endl;
                };
                
                // default destructor of the LSH class
                ~LSH() {};

                // Search functions, each implemented in a differend file under `algorithms/LSH/src`

                std::pair<int,T> NearestNeighbour(std::vector<T> query_vector);
                list<pair<int,T>> kNearestNeighbour(std::vector<T> query_vector, int k);
                list<pair<int,T>> RangeSearch(vector<T> query_vector, double radius, int c);
};