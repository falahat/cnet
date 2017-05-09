//
// Created by Aryan on 5/8/2017.
//

#ifndef CNET_LOADER_H
#define CNET_LOADER_H

#define BIG_PRIME 1299647


#include <vector>
#include <array>

using namespace std;
class Loader {
public:
    Loader() {

    }

    void loadXorData(vector<vector<double>> *X, vector<vector<double>> *Y) {
        X->push_back({0,0}); Y->push_back({0});
        X->push_back({0,1}); Y->push_back({1});
        X->push_back({1,0}); Y->push_back({1});
        X->push_back({1,1}); Y->push_back({0});
    }

    void loadAndData(vector<vector<double>> *X, vector<vector<double>> *Y) {
        X->push_back({0,0}); Y->push_back({0});
        X->push_back({0,1}); Y->push_back({0});
        X->push_back({1,0}); Y->push_back({0});
        X->push_back({1,1}); Y->push_back({1});
    }

    void loadOrData(vector<vector<double>> *X, vector<vector<double>> *Y) {
        X->push_back({0,0}); Y->push_back({0});
        X->push_back({0,1}); Y->push_back({1});
        X->push_back({1,0}); Y->push_back({1});
        X->push_back({1,1}); Y->push_back({1});
    }


    void loadLineData(vector<vector<double>> *X, vector<vector<double>> *Y, double slope, double intercept) {
        int NUM_POINTS = 20;
        double AMPLITUDE = 1.0;
        for (int i = 0; i < NUM_POINTS; ++i) {
            double x = (AMPLITUDE * i) / NUM_POINTS;
            double y = slope * x + intercept;
            X->push_back({x}); Y->push_back({y});

        }
    }

};

void shuffleInplace(vector<vector<double>> *X, vector<vector<double>> *Y) {
    for (int iOg = 0; iOg < X->size(); ++iOg) {
        int iNew = (iOg * BIG_PRIME) % X->size();
        vector<double> temp;

        temp = (*X)[iOg];
        (*X)[iOg] = (*X)[iNew];
        (*X)[iNew] = temp;
        temp = (*Y)[iOg];
        (*Y)[iOg] = (*Y)[iNew];
        (*Y)[iNew] = temp;
    }
}

#endif //CNET_LOADER_H
