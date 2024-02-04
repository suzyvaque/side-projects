//
// Created by Suzy Vaque on 2023-10-14.
//

#ifndef FORSNUSOLO_AUS_NEW_BYREG_H
#define FORSNUSOLO_AUS_NEW_BYREG_H

using namespace std;
#include <string>
#include <vector>

class ByReg {
public:
    ByReg();

    vector<string> v_cs_string;
    vector<double> v_cs_distance_from_darwin;

    const double darwin = 0;
    const double katherine = 323;
    const double dunmarra = 634;
    const double tennant = 990;
    const double barrow = 1214;
    const double alice = 1499;
    const double erldunda = 1698;
    const double coober = 2186;
    const double glendambo = 2440;
    const double port = 2726;
    const double adelaide = 3035;

    // Getters
    double get_distance_from_string(const string& controlstop){
        if(controlstop=="Darwin"){return darwin;}
        else if(controlstop=="Katherine"){return katherine;}
        else if(controlstop=="Dunmarra"){return dunmarra;}
        else if(controlstop=="TennantCreek"){return tennant;}
        else if(controlstop=="BarrowCreek"){return barrow;}
        else if(controlstop=="AliceSprings"){return alice;}
        else if(controlstop=="Erldunda"){return erldunda;}
        else if(controlstop=="CooberPedy"){return coober;}
        else if(controlstop=="Glendambo"){return glendambo;}
        else if(controlstop=="PortAugusta"){return port;}
        else if(controlstop=="Adelaide"){return adelaide;}
        else{return -404;}
    }

    int get_csnum_from_string(const string& controlstop){
        if(controlstop=="Darwin"){return 0;}
        else if(controlstop=="Katherine"){return 1;}
        else if(controlstop=="Dunmarra"){return 2;}
        else if(controlstop=="TennantCreek"){return 3;}
        else if(controlstop=="BarrowCreek"){return 4;}
        else if(controlstop=="AliceSprings"){return 5;}
        else if(controlstop=="Erldunda"){return 6;}
        else if(controlstop=="CooberPedy"){return 7;}
        else if(controlstop=="Glendambo"){return 8;}
        else if(controlstop=="PortAugusta"){return 9;}
        else if(controlstop=="Adelaide"){return 10;}
        else{return -404;}
    }

};


#endif //FORSNUSOLO_AUS_NEW_BYREG_H
