#include <iostream>
#include <fstream>
#include <string>
#include <list> 
#include <vector> 

using namespace std;


template <typename <T, U>>
void print(T x) {
    vector<T> y;
    cout << x << endl;
}


int main(void) {
    print(5);
    print("hey");
    print(4.3);
}