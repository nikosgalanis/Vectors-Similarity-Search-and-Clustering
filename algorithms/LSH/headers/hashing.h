#pragma once

#include <iostream>
#include <random>
#include <string>
#include <cmath>
#include <stdint.h>
#include <list>
#include "../../../utils/math_utils.h"
#include <bits/stdc++.h> 


using namespace std;

namespace lsh {
	class HashFunction {
		private:
			int dim; // dimension of the space
			double w; // real number, considerably grater than radius
			std::vector<double> s; // real random vector, for hash comparison
			uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
			uint32_t M; // size of the hash table
			std::vector<int> const_m_values; // vector that will store (m ^ i) % M, which is needed in every hashing

		public:
			// class constructor
			HashFunction(int dim, double w, uint32_t m, uint32_t M) : dim(dim), w(w), s(dim, 0), m(m), M(M), const_m_values(dim) {
				// we want to use a uniform real distributor, thus we must initialize it
				// we want to randomize the input each time
				std::random_device mch;
				std::default_random_engine generator(mch());
				std::uniform_real_distribution<double> distribution(0.0, w);
				// initialize the random vector in the space `dim` using the uniform distribution
				for (int i = 0; i < this->dim; i++) {
					s.at(i) = distribution(generator);
				}

				// initialize the const m vector
				for (int i = 0; i < this->dim; i++) {
					const_m_values[i] = our_math::modular_power(m, i, M);
				}
			};

			// class destructor
			~HashFunction(){};

			// hash a vector with the 
			template <typename T>
			uint32_t hash(std::vector<T> x) {
				uint32_t h = 0;
				// vector a computed by the lsh method
				std::vector<int> a(dim, 0);
				// compute the values of the vector a
				for (int i = 0; i < dim; i++) {
					a[i] = floor((x.at(i) - s.at(i)) / (w * 1.0));
				}
				// compute the hash result of the vector x
				for (int i = 0; i < dim; i++) {
					// each time apply a modulo operation
					h += our_math::modulo(our_math::modulo(a.at(dim - i - 1), M) * const_m_values.at(i), M);
				}
				// return the position in the hash
				return h % M;
			};
	};

	class AmplifiedHashFunction {
		private:
			std::list<HashFunction> hlist;
			int dim; // dimension of the space
			double w; // real number, considerably grater than radius
			uint32_t m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
			uint32_t M; // size of the hash table
			int k; // number of HashFunctions used
		public:

			//class constructor
			AmplifiedHashFunction(int dim, double w, uint32_t m, uint32_t M, int k) : dim(dim), w(w), m(m), M(M), k(k) {
					// initialize the Hash Functions
					for (int i = 0; i < k; i++) {
						hlist.push_front(HashFunction(dim, w, m, M));
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

			template <typename T>
			// assign a previously computed g function result to a bucket in our hashtable
			uint32_t assign_to_bucket(std::vector<T> x) {
				// use our own modulo function
				int result = this->hash(x);
				return our_math::modulo(result, (M / 8));
			}
	};
}