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
    inline void vector_print(std::vector<T> vec) {
        for (int i = 0; i < vec.size(); i++) {
            cout << vec.at(i) << ' ';
        }
        cout << endl;
    }

    //print list of pairs
    template <typename T, typename M>
    inline void list_of_pairs_print(list<pair<T,M>> result) {
        typename std::list<std::pair<T,M>>::iterator pair_it;
        for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
            cout << pair_it->first << " " << pair_it->second << endl;
        }
        cout << endl;
    }

    //print list of pairs same line
    template <typename T, typename M>
    inline void same_line_list_of_pairs_print(list<pair<T,M>> result) {
        typename std::list<std::pair<T,M>>::iterator pair_it;
        for ( pair_it = result.begin(); pair_it !=result.end(); pair_it++) {
            cout << pair_it->first << " " << pair_it->second << " | ";
        }
        cout << endl;
    }
};