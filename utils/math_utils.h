#pragma once

/** 
 Mthematical functions that are usefull for our project
 and C++ does not offer (or offers under a different definition) 
 **/

namespace our_math{
	// modulo that works for every integer number
	int modulo(int a, int b) {
		int c = a % b;
  		
		return (c < 0) ? c + b : c;
	}
	// Convert a number form big endian to little endian
	int reverseInt(int i) {
		unsigned char c1, c2, c3, c4;
		c1 = i & 255;
		c2 = (i >> 8) & 255;
		c3 = (i >> 16) & 255;
		c4 = (i >> 24) & 255;

		return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
	}
};