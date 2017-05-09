//
// Created by Aryan on 5/8/2017.
//

#ifndef CNET_LOADER_H
#define CNET_LOADER_H


#include <vector>
#include <array>

using namespace std;
class Loader {
public:
    Loader() {

    }

    void loadXorData(vector<array<double,2>> *X, vector<array<double,1>> *Y) {
        X->push_back({0,0}); Y->push_back({0});
        X->push_back({0,1}); Y->push_back({1});
        X->push_back({1,0}); Y->push_back({1});
        X->push_back({1,1}); Y->push_back({0});
    }
};


#endif //CNET_LOADER_H
