//
// Created by Suzy Vaque on 2023-10-14.
//

#include "ByReg.h"

ByReg::ByReg(){
    v_cs_string.emplace_back("Darwin");
    v_cs_string.emplace_back("Katherine");
    v_cs_string.emplace_back("Dunmarra");
    v_cs_string.emplace_back("TennantCreek");
    v_cs_string.emplace_back("BarrowCreek");
    v_cs_string.emplace_back("AliceSprings");
    v_cs_string.emplace_back("Erldunda");
    v_cs_string.emplace_back("CooberPedy");
    v_cs_string.emplace_back("Glendambo");
    v_cs_string.emplace_back("PortAugusta");
    v_cs_string.emplace_back("Adelaide");

    v_cs_distance_from_darwin.emplace_back(darwin);
    v_cs_distance_from_darwin.emplace_back(katherine);
    v_cs_distance_from_darwin.emplace_back(dunmarra);
    v_cs_distance_from_darwin.emplace_back(tennant);
    v_cs_distance_from_darwin.emplace_back(barrow);
    v_cs_distance_from_darwin.emplace_back(alice);
    v_cs_distance_from_darwin.emplace_back(erldunda);
    v_cs_distance_from_darwin.emplace_back(coober);
    v_cs_distance_from_darwin.emplace_back(glendambo);
    v_cs_distance_from_darwin.emplace_back(port);
    v_cs_distance_from_darwin.emplace_back(adelaide);
}

// and the getters in .h