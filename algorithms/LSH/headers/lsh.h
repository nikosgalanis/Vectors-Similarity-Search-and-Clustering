#pragma once

#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

#include "hashing.h"


template <typename T>
class LSH {
    private:
        const uint32_t L; // number of hash tables used in LSH
        const uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
        const uint32_t M; // size of each hash table
        const uint32_t n_points; // number of points to be hashed initially
        const uint32_t k; // given from the user, affects number of the buckets
        const uint32_t w; // a number signifficantly larger thar the radius, affects range search
        const uint32_t space_dim; // the dimension of the space we are going to work with
        // a list of the hash functions for every hash table
        std::list<lsh::AmplifiedHashFunction<T>> amplified_hash_fns;

        // vector of the hash tables used by LSH. We are just hashing an vector's index in order to save space
        std::vector<std::unordered_map<int, int>> lsh_tables;

        // vector of the vectors given to us, stored here for convinience
        std::vector<std::vector<T>> feature_vectors;
        // TODO: Initialize the feature vector

        public:
            // constructor of the LSH class
            LSH(const uint32_t L, const uint32_t m, const uint32_t M
                const uint32_t n_points, const uint32_t k,
                const uint32_t space_dim, std::vector<std::vector<T>> init_vectors) : 
                L(L), m(m), M(M), n_points(n_points), k(k), w(w), space_dim(space_dim) {
                    /**
                     Preprocessing: We want to uniformly select L amplified h.fs g1, .., g_l.
                     Afterwards, we are going to initialize all of the hash tables, before hashing the 
                     vectors.
                     **/
                    for (int i = 0; i < L; i++) {
                        lsh::AmplifiedHashFunction ampl(space_dim, w, m, M, k);
                        // Insert each amplified hash fn into the apropriate list
                        amplified_hash_fns.push_front(ampl);
                    }

                    // initialize our local copy of all the vectors given to hash
                    feature_vectors(init_vectors.begin(), init_vectors.end());

                    // initialize the vector of the hash tables with the appropriate size
                    lsh_tables(L);
                    
                    // insert every vector given to us into every hash table
				    for (std::list<lsh::AmplifiedHashFunction<T>>::iterator it = amplified_hash_fns.begin(), int i = 0; 
                    it != amplified_hash_fns.end(); it++, i++) {
                        // initialize a hash table to map an index to a vector
                        std::unordered_map<int, std::vector<T>> hash_table;
                        // insert each vector in the i-th hash table
                        for (int n = 0; n < N; n++) {
                            // compute the bucket that the vector will be inserted
                            int index = it->assign_to_bucket(it->hash(init_vectors.at(n)));
                            // hash its index
                            // TODO: We are hashing the index. Many things can go wrong here. be very carefull
                            hash_table[index].push_back(n);
                        }
                        // insert the now full hash table to the vector of the hashes
                        lsh_tables[i] = hash_table;
                    }
                };
                
                // default destructor of the LSH class
                ~LSH() {};

                // Search functions, each implemented in a differend file under `algorithms/LSH/src`
                T NearestNerighbor(/**/); //TODO: Sofo
                T NNearestNerighbor(/**/); //TODO
                T AproximateRangeSearch(/**/); //TODO: Nikos
};