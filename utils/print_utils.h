#pragma once
#include <iostream>
#include <vector>

/**
Print functions
**/
using namespace std;

namespace print {
    template <typename T> 
    // print vector
    void vector_print(std::vector<T> vec) {
        for (int i = 0; i < vec.size(); i++) {
            cout << vec.at(i) << ' ';
        }
        cout << endl;
    }

    //print list of pairs
    template <typename T>
    void list_of_pairs_print(list<pair<T,T>> result) {
        for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
            cout << pair_it->first << ", " << pair_it->second << endl;
        }
        cout << endl;
    }
};