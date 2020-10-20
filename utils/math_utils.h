#pragma once

#include "../algorithms/Search/BruteForce/headers/brute_force.h"
#include "print_utils.h"
#include <bitset>
/** 
 Mathematical functions that are usefull for our project
 and C++ does not offer (or offers under a different definition) 
 **/

namespace our_math {
	// modulo that works for every integer number
	inline int modulo(int a, int b) {
		int c = a % b;
  		
		return (c < 0) ? c + b : c;
	};

	// executes the operation (x^y) mod p
	inline int modular_power(int x, int y, int p) {
		int res = 1;

		x = x % p;
		while (y > 0) {
			if (y & 1)
				res = (res * x) % p;
			y = y >> 1;
			x = (x * x) % p;
		}
		return res;
	};
	
	// Convert a number form big endian to little endian
	inline int big_to_litte_endian(int i) {
		unsigned char c1, c2, c3, c4;
		c1 = i & 255;
		c2 = (i >> 8) & 255;
		c3 = (i >> 16) & 255;
		c4 = (i >> 24) & 255;

		return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	};

	// Bitwise concatenate 2 integers
	inline uint32_t bitwise_concat(uint16_t current, uint32_t next, int k) {
		// we want to shift by 32/k
		int shift_amount = 32 / k;

		return (current << shift_amount) | next;
	};
	
	// compute the best w value
	inline double compute_w_value(std::vector<std::vector<double>>feature_vectors, int n_images) {
		// Initialize the BF class
		BruteForce<double> bf_instant(n_images, feature_vectors.at(1).size(), feature_vectors);
		int sum_result = 0;
		for (int i = 0; i < n_images; i++) {
			list<pair<int, double>> kBF = bf_instant.kNeighboursBF(feature_vectors.at(i), 2);
			// ge the second element of the list, as the first is the vector itself
			int distance_from_nn = kBF.back().second;
			sum_result += distance_from_nn;
		}
		return sum_result / (n_images * 1.0);
	}

	// find all of the numbers that have hamming distance = 1 with a given number of bitwise size k
	inline std::list<int> hamming_distance_1(int number, int k) {
		list<int> result;
		std::string str = std::bitset<32>(number).to_string();
		std::string new_str = str.substr(str.size() - k);
		for (int i = 0; i < (int)new_str.size(); i++) {
			int value = (new_str[i] - '0');
			if (value == 0)
				new_str.at(i) = '1';
			else 
				new_str.at(i) = '0';

			int res_number = (int)std::bitset<32>(new_str).to_ulong();
			result.push_back(res_number);

			if (value == 0)
				new_str.at(i) = '0';
			else 
				new_str.at(i) = '1';
		}
		
		return result;
	}

	// find the hamming distance between 2 integers
	inline int hamming_distance(int a, int b) {
		int x = a ^ b;
		int bits = 0;

		while (x > 0) {
			bits += x & 1;
			x >>= 1;
		}

		return bits;
	}
}
