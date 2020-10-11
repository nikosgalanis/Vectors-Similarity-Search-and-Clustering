#include <iostream>
#include <vector>

#include "hashing.h"
#include "lsh.h"

using namespace std;

int main(void) {
    vector<float> x = {1.0,2.2,3.4,4.5};
    vector<float> y = {6000.3,3023.3,9212.4,1544.2};

    // lsh::HashFunction instant(4, 5.0, 1000.0, (pow(2,16) - 5), pow(2,8));

    // cout << instant.Hash(x) << endl;

    // lsh::AmplifiedHashFunction ampl(4, 5.0, 1000.0, (pow(2,32) - 5), pow(2,8), 4);

    // cout <<  ampl.assign_to_bucket(ampl.hash(y)) << endl;
    return 0;

}