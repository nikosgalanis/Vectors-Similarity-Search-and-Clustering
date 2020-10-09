#pragma once

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <stdint.h>
#include <list>
#include "../../../utils/math_utils.h"
#include<bits/stdc++.h> 


// TODO: Change when makefile
// #include "../../metrics.h"
using namespace std;

namespace lsh {
	class HashFunction {
		private:
			const int dim; // dimension of the space
			const double radius; // radius of the range search
			const double w; // real number, considerably grater than radius
			std::vector<double> s; // real random vector, for hash comparison
			const uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
			const uint32_t M; // size of the hash table
		public:
			// class constructor
			HashFunction(const int dim, const double radius, const double w, const uint32_t m, const uint32_t M) : dim(dim), radius(radius), w(w), m(m), M(M), s(dim, 0) {
				// we want to use a uniform real distributor, thus we must initialize it
				// we want to randomize the input each time
				std::random_device mch;
				std::default_random_engine generator(mch());
				std::uniform_real_distribution<double> distribution(0.0, w);
				// initialize the random vector in the space `dim` using the uniform distribution
				for (int i = 0; i < this->dim; i++) {
					s.at(i) = distribution(generator);
				}
			};
			// class destructor
			~HashFunction(){};
			template <typename T>
			uint32_t hash(std::vector<T> x) {
				uint32_t h = 0;
				// vector a computed by the lsh method
				std::vector<int> a(dim, 0);
				// compute the values of the vector a
				for (int i = 0; i < dim; i++) {
					a[i] = (int)floor((x.at(i) - s.at(i)) / (w * 1.0));
				}
				// compute the hash result of the vector x
				for (int i = 0; i < dim; i++) {
					// each time apply a modulo operation
					h += our_math::modulo(our_math::modulo(a.at(dim - i - 1), M) * our_math::modulo((uint64_t)pow(m, i), M), M);
				}
				// return the position in the hash
				return h % M;
			};
	};

	class AmplifiedHashFunction {
		private:
			std::list<HashFunction> hlist;
			const int dim; // dimension of the space
			const double radius; // radius of the range search
			const double w; // real number, considerably grater than radius
			const uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
			const uint32_t M; // size of the hash table
			const int k; // number of HashFunctions used
		public:
			//class constructor
			AmplifiedHashFunction(const int dim, const double radius, const double w, const uint32_t m, const uint32_t M, const int k) : dim(dim), radius(radius), w(w), m(m), M(M), k(k) {
					// initialize the Hash Functions
					for (int i = 0; i < k; i++) {
						hlist.push_front(HashFunction(dim, radius, w, m, M));
					}
			};
			//class destructor
			~AmplifiedHashFunction(){};
			template <typename T>
			// Amplified function by concatenation of the result as strings
			uint32_t hash(std::vector<T> x) {
				int concatenated = 0;
				for (std::list<HashFunction>::iterator it = hlist.begin(); it != hlist.end(); it++) {
					// each time, concatenate with a bit-wise manner the next h
					concatenated = our_math::bitwise_concat(concatenated, it->hash(x), k);
				}

				// return the final random number
				return concatenated;
			}
			// assign a previously computed g function result to a bucket in our hashtable
			uint32_t assign_to_bucket(int g_func_result) {
				// use our own modulo function
				return our_math::modulo(g_func_result, (M / 8));
			}
	};
}