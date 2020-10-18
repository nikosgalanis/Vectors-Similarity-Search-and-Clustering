#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 
#include <limits>
#include <utility>


#include "../../LSH/headers/hashing.h"
#include "../headers/hypercube.h"
#include "../../common/metrics.h"
#include "../../../utils/print_utils.h"

using namespace std;
using namespace metrics;

template <typename T>
// function that implements NearestNeighbour
list<pair<int,T>> Hypercube<T>::kNearestNeighbour(std::vector<T> query_vector, int k) {

	list<pair<int,T>> result;
	
	//the greatest of the k min distances
	T kth_min_distance = (T) INT_MAX;

	int visited = 0;

	/**
	 Step 1: Hash the given vector with the random projection technique
	 **/
	int bucket_int = hash_to_hypercube(query_vector);

	/**
	 Step 2: Search in the bucket that the query hashes for neighbors
	**/
	list<int> bucket = hypercube[bucket_int];

	for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {

		visited++;

		// distance between vectors
		T distance =  ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
		
		// if the element is already in the list, skip the checks
		pair<int, T> new_pair = make_pair(*bucket_it, distance);
		
		if (find(result.begin(), result.end(), new_pair) != result.end()) {
			continue;
		}

		// insert the pair into the list if it's distance is less than the kth_min
		if (result.size() > 0 && result.size() < k) {
			// size < k so we just insert it
			
			if (distance >= kth_min_distance) {
				// insert at back if distance > kth_min_distance
				// kth_min_distance is the distance of the last element
				kth_min_distance = distance;
				result.push_back(new_pair);
			} else {
				// iterate through the list to insert it in the right position
				typename std::list<std::pair<int,T>>::iterator pair_it;
				for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
					if ( distance < pair_it->second) {
						result.insert(pair_it, new_pair);
						break;
					}
				}
			}
		} else if (result.size() > 0 && (result.size() == k)) {
			// insert, pop the last, and make the kth_min the new last
			if (distance < kth_min_distance) {
				// iterate through the list to insert it in the right position
				for (typename std::list<pair<int,T>>::iterator pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
					if ( distance < pair_it->second) {
						result.insert(pair_it, new_pair);
						break;
					}
				}
				result.pop_back();
				kth_min_distance = result.back().second;
			}
		} else if (result.size() == 0) {
			// size is 0, insert 1st element
			kth_min_distance = distance;
			result.push_back(new_pair);
		} else {
			cout << "size " << result.size() << endl;
			cout << "knearest problem" << endl;
			exit(EXIT_FAILURE);
		}

		// if we've reached the threshold, just return the result list
		if (visited >= threshold) 
			return result;
	}

	/**
	 Step 3 .. n: If necessary, check all the buckets with hamming distance 1 .. n
	 			until the threshold is reached.
	 **/
	// set the desired hamming distance to 1 initially
	int dist_count = 1;
	// while we are under the threshold
	while (true) {
		// parse through all the buckets
		for (int i = 0; i < n_buckets; i++) {
			// if the hamming distance from the bucket_int is the desired
			if (our_math::hamming_distance(i, bucket_int) == dist_count) {
				// check the bucket for neighbors in the range given
				list<int> bucket = hypercube[i];
				for (std::list<int>::iterator bucket_it = bucket.begin(); bucket_it != bucket.end(); bucket_it++) {

					visited++;

					// distance between vectors
					T distance =  ManhatanDistance(feature_vectors.at(*bucket_it), query_vector, space_dim);
					
					// if the element is already in the list, skip the checks
					pair<int, T> new_pair = make_pair(*bucket_it, distance);
					
					if (find(result.begin(), result.end(), new_pair) != result.end()) {
						continue;
					}

					// insert the pair into the list if it's distance is less than the kth_min
					if (result.size() > 0 && result.size() < k) {
						// size < k so we just insert it
						
						if (distance >= kth_min_distance) {
							// insert at back if distance > kth_min_distance
							// kth_min_distance is the distance of the last element
							kth_min_distance = distance;
							result.push_back(new_pair);
						} else {
							// iterate through the list to insert it in the right position
							typename std::list<std::pair<int,T>>::iterator pair_it;
							for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
								if ( distance < pair_it->second) {
									result.insert(pair_it, new_pair);
									break;
								}
							}
						}
					} else if (result.size() > 0 && (result.size() == k)) {
						// insert, pop the last, and make the kth_min the new last
						if (distance < kth_min_distance) {
							// iterate through the list to insert it in the right position
							for (typename std::list<pair<int,T>>::iterator pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
								if ( distance < pair_it->second) {
									result.insert(pair_it, new_pair);
									break;
								}
							}
							result.pop_back();
							kth_min_distance = result.back().second;
						}
					} else if (result.size() == 0) {
						// size is 0, insert 1st element
						kth_min_distance = distance;
						result.push_back(new_pair);
					} else {
						cout << "size " << result.size() << endl;
						cout << "knearest problem" << endl;
						exit(EXIT_FAILURE);
					}

					// if we've reached the threshold, just return the result list
					if (visited >= threshold) 
						return result;
				}
			}
		}
		// again, if necessary, increase the hamming distance, so we check further buckets
		dist_count++;
	}
}