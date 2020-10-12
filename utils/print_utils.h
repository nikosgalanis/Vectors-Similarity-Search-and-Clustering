#pragma once
#include <iostream>
#include <vector>

/**
Print functions
**/
using namespace std;

namespace print {
    template <typename T> 
    void vector_print(std::vector<T> vec) {
        for (int i = 0; i < vec.size(); i++) {
            cout << vec.at(i) << ' ';
        }
        cout << endl;
    }
};