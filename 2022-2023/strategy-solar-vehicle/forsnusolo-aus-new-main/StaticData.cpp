//
// Created by Suzy Vaque on 2023-10-14.
//

#include "StaticData.h"

/*
void StaticData::read_data_from_csv() {
    string cs = "Darwin";
    string filename = "StaticDataFolder/"+cs+".csv";

    ifstream fs(filename);
    string oneline;

    if(fs.is_open()){
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

}
*/


void StaticData::read_data_from_csv(){
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

    for(int cs=0; cs<11; cs++){
        string filename = "StaticDataFolder/"+controlstops[cs]+".csv";
        filename = "StaticDataFolder/Darwin.csv";   // FIXME erase

        ifstream fs(filename);
        string oneline;

        vector<vector<double>>& csvector = csstring_to_csvector(controlstops[cs]);

        // set the number of rows for the outer vector only. Needed for the direct index access for writing data at store method.
        // FIXME set the exact number, for extra space may cause unexpected behaviors.
        csvector.resize(1548);  //FIXME erase

        if(fs.is_open()){
            getline(fs,oneline);    // skip line 0, which contains info of data columns below.
            int linenum = 0;
            while(getline(fs,oneline)){
                store_data_in_vector(csvector, oneline, linenum);
                linenum++;
            }
        } else{
            cout<<"Wrong Static Data File"<<endl;
        }
        fs.close();
    }
}


void StaticData::store_data_in_vector(vector<vector<double>>& csvector, const string& oneline, int linenum){
    // Called only once, at the start of an execution.
    // Writes read data in vector<vector<double>>

    istringstream iss(oneline);
    string token;
    int num = 0;

    while(getline(iss,token,',')){  // CAUTION : "," causes an error. delimiter should be char, not string.
        if(num++==6){break;}    // exclude time strings. Assumes there are only 6 parameters- temp, dhi, dni, ghi, zenith, azimuth.
        csvector[linenum].emplace_back(stod(token));
    }
}

vector<vector<double>>& StaticData::csstring_to_csvector(const string& controlstop){
    // const string& since controlstop val is simply being compared
    if(controlstop=="Darwin"){
        return v_Darwin;
    } else if(controlstop=="Katherine"){
        return v_Katherine;
    } else if(controlstop=="Dunmarra"){
        return v_Dunmarra;
    } else if(controlstop=="TennantCreek"){
        return v_TennantCreek;
    } else if(controlstop=="BarrowCreek"){
        return v_BarrowCreek;
    } else if(controlstop=="AliceSprings"){
        return v_AliceSprings;
    } else if(controlstop=="Erldunda"){
        return v_Erldunda;
    } else if(controlstop=="CooberPedy"){
        return v_CooberPedy;
    } else if(controlstop=="Glendambo"){
        return v_Glendambo;
    } else if(controlstop=="PortAugusta"){
        return v_PortAugusta;
    } else{
        return v_Adelaide;
    }
}