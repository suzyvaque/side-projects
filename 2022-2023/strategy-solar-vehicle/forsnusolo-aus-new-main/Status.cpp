//
// Created by Suzy Vaque on 2023-10-14.
//

#include "Status.h"

Status::Status(){
    run_state = code_race;
    speed = 0;
    estimated_distance = 0.0;
    travelled_distance = 0.0;
    controlstop_nearest = "";
    controlstop_next = "";
    distance_to_controlstop_next = 0.0;
    power_generated = 0.0;
    power_consumed = 0.0;
    battery_soc = 0.0;
}

Status::Status(int run_state){
    this-> run_state = run_state;
    speed = 0;
    estimated_distance = 0;
    travelled_distance = 3035;  // sigma ByRegulation.dar_to_kat ... por_to_ade
    controlstop_nearest = "Adelaide";
    controlstop_next = "Adelaide";
    distance_to_controlstop_next = 0;
    power_generated = 0;
    power_consumed = 0;
    battery_soc = 0;
}

Status::Status(int run_state, int speed, double travelled_distance, double estimated_distance, string controlstop_nearest, string controlstop_next, double distance_to_controlstop_next, double power_generated, double power_consumed, double battery_soc) {
    // Status::time_state = time_state;
    this-> run_state = run_state;
    this-> travelled_distance = travelled_distance;
    this-> speed= speed;
    this-> estimated_distance = estimated_distance;
    this-> controlstop_nearest = controlstop_nearest;
    this-> controlstop_next = controlstop_next;
    this-> distance_to_controlstop_next = distance_to_controlstop_next;
    this-> power_generated = power_generated;
    this-> power_consumed = power_consumed;
    this-> battery_soc = battery_soc;
}

// and the getters in .h