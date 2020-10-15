#pragma once

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
}
