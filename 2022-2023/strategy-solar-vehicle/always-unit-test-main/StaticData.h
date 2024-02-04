//
// Created by sjvaque on 2023-10-14.
//

#ifndef ALWAYS_UNIT_TEST_STATICDATA_H
#define ALWAYS_UNIT_TEST_STATICDATA_H

using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class StaticData {
public:
    vector<vector<double>> v_Darwin;
    vector<vector<double>> v_Katherine;
    vector<vector<double>> v_Dunmarra;
    vector<vector<double>> v_TennantCreek;
    vector<vector<double>> v_BarrowCreek;
    vector<vector<double>> v_AliceSprings;
    vector<vector<double>> v_Erldunda;
    vector<vector<double>> v_CooberPedy;
    vector<vector<double>> v_Glendambo;
    vector<vector<double>> v_PortAugusta;
    vector<vector<double>> v_Adelaide;

    void read_data_from_csv();
    void store_data_in_vector(vector<vector<double>>& csvector, const string& oneline, int linenum);
    vector<vector<double>>& csstring_to_csvector(const string& controlstop);
};

#endif //ALWAYS_UNIT_TEST_STATICDATA_H
