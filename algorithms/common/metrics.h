#pragma once

#include <iostream>
#include <cmath>
#include <vector>

// Various metrics for vectors
namespace metrics {
	template <typename T> 
	// Compute the Manhatan Distance between 2 vectors in the dim-th dimension
	T ManhatanDistance(std::vector<T>x, std::vector<T>y, int dim) {
		T result{};
		for (int i = 0; i < dim; i++) {
			result += std::abs(x[i] - y[i]);
		}
		return result;
	}

	template <typename T> 
	// Compute the Eucledian Distance between 2 vectors in the dim-th dimension
	T EucledianDistance(std::vector<T>x, std::vector<T>y, int dim) {
		T result{};
		for (int i = 0; i < dim; i++) {
			result += pow(std::abs(x[i] - y[i]), 2);
		}
		return sqrt(result);
	}
};