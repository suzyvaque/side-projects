//
// Created by sjvaque on 2023-10-14.
//

#include <iostream>
using namespace std;
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "StaticData.h"

int main() {

    // test 3 - success, StaticData all tested.
    StaticData sd;
    sd.read_data_from_csv();

    cout<<"should be 29.8, and is "<<sd.v_Darwin[0][0]<<endl;
    cout<<"should be 98, and is "<<sd.v_Darwin.back().back()<<endl;


    /*
    // test 2 - success
    // Called only once, at the start of an execution.
    // Reads data from csv
    vector<string> controlstops;
    controlstops.emplace_back("Darwin");
    controlstops.emplace_back("Katherine");
    controlstops.emplace_back("Dunmarra");
    controlstops.emplace_back("TennantCreek");
    controlstops.emplace_back("BarrowCreek");
    controlstops.emplace_back("AliceSprings");
    controlstops.emplace_back("Erldunda");
    controlstops.emplace_back("CooberPedy");
    controlstops.emplace_back("Glendambo");
    controlstops.emplace_back("PortAugusta");
    controlstops.emplace_back("Adelaide");

    for(int cs=0; cs<11; cs++) {
        string filename = "StaticDataFolder/" + controlstops[cs] + ".csv";
        filename = "StaticDataFolder/Darwin.csv";   // FIXME erase

        ifstream fs(filename);
        string oneline;

        vector<vector<double>> csvector;
        //vector<vector<double>>& csvector = csstring_to_csvector(controlstops[cs]);

        // set the number of rows for the outer vector only. Needed for the direct index access for writing data at store method.
        // FIXME set the exact number, for extra space may cause unexpected behaviors.
        csvector.resize(1548);  //FIXME erase

        if (fs.is_open()) {
            getline(fs, oneline);    // skip line 0, which contains info of data columns below.
            int linenum = 0;
            while (getline(fs, oneline)) {
                //store_data_in_vector(csvector, oneline, linenum);
                istringstream iss(oneline);
                string token;
                int num = 0;

                while (getline(iss, token,
                               ',')) {  // CAUTION : "," causes an error. delimiter should be char, not string.
                    if (num++ ==
                        6) { break; }    // exclude time strings. Assumes there are only 6 parameters- temp, dhi, dni, ghi, zenith, azimuth.
                    csvector[linenum].emplace_back(stod(token));
                }


                cout << oneline << endl;
                linenum++;
            }
        } else {
            cout << "Wrong Static Data File" << endl;
        }
        fs.close();
    }
     */



    /*
    // test 1 - success
    string filename = "Darwin.csv";

    ifstream fs(filename);
    string oneline;

    if(fs.good()){
        getline(fs,oneline);    // skip line 0, which contains info of data columns below.
        int linenum = 0;
        while(getline(fs,oneline)){
            cout<<oneline<<endl;
            linenum++;
        }
    } else{
        cout<<"Wrong Static Data File"<<endl;
    }
    fs.close();
     */



    return 0;
}