#pragma once

#include <iostream>
#include <random>
#include <string>
#include <cmath>

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
            const int m; // constant that denotes an operation in the hash fn. Typically 2^32 - 5
            const int M; // size of the hash table
        public:
            // class constructor
            HashFunction(const int dim, const double radius, const double w, int m, int M) : dim(dim), radius(radius), w(w), m(m), M(M), s(dim, 0) {
                // we want to use a uniform real distributor, thus we must initialize it
                std::default_random_engine generator;
                std::uniform_real_distribution<double> distribution(0.0, w);
                // initialize the random vector in the space `dim` using the uniform distribution
                for (int i = 0; i < this->dim; i++) {
                    s[i] = distribution(generator);
                }
            };
            // class destructor
            ~HashFunction(){};
            template <typename T>
            int Hash(std::vector<T> x) {
                int h = 0;
                // vector a computed by the lsh method
                std::vector<int> a(dim, 0);
                // compute the values of the vector a
                for (int i = 0; i < dim; i++) {
                    cout << x[i]-s[i] << endl;
                    a[i] = (int)floor((x[i] - s[i]) / (w * 1.0));
                }
                // compute the hash result of the vector x
                for (int i = 0; i < dim; i++) {
                    h += ((a[dim - i - 1] % M) * ((uint64_t)pow(m, i)) % M) % M;
                }
                // return the position in the hash
                return h % M;
            };
    };
}