//
// Created by Suzy Vaque on 2023-10-14.
//

#ifndef FORSNUSOLO_AUS_NEW_STATUS_H
#define FORSNUSOLO_AUS_NEW_STATUS_H

using namespace std;
#include <string>
#include <vector>

// Global
const int code_race = 111;
const int code_charge_stop = 222;
const int code_total_stop = 333;
const int code_at_finish_line = 777;

class Status {
public:

    explicit Status();  // used for pseudo-Statuses at main.cpp
    explicit Status(int run_state);  // should be used for code_at_finish_line only // made explicit for "Clang-Tidy: Single-argument constructors must be marked explicit to avoid unintentional implicit conversions"
    Status(int run_state, int speed, double travelled_distance, double estimated_distance, string controlstop_nearest, string controlstop_next, double distance_to_controlstop_next, double power_generated, double power_consumed, double battery_soc);

    // Getters
    int get_run_state(){
        return run_state;
    }
    int get_speed(){
        return speed;
    }
    double get_travelled_distance(){
        return travelled_distance;
    }
    double get_estimated_distance(){
        return estimated_distance;
    }
    string get_controlstop_nearest(){
        return controlstop_nearest;
    }
    string get_controlstop_next(){
        return controlstop_next;
    }
    double get_distance_to_controlstop_next(){
        return distance_to_controlstop_next;
    }
    double get_power_generated(){
        return power_generated;
    }
    double get_power_consumed(){
        return power_consumed;
    }
    double get_battery_soc() {
        return battery_soc;
    }

    // Setters for Car's generator, the initial state
    void set_run_state(int state){
        this->run_state = state;
    }
    void set_speed(int speed){
        this->speed = speed;
    }
    void set_travelled_distance(double distance){
        this->travelled_distance = distance;
    }
    void set_estimated_distance(double distance){
        this->estimated_distance = distance;
    }
    void set_controlstop_nearest(string cs){
        this->controlstop_nearest = cs;
    }
    void set_controlstop_next(string cs){
        this->controlstop_next = cs;
    }
    void set_distance_to_controlstop_next(double distance){
        this->distance_to_controlstop_next = distance;
    }
    void set_power_generated(double power){
        this->power_generated = power;
    }
    void set_power_consumed(double power){
        this->power_consumed = power;
    }
    void set_battery_soc(double soc){
        this->battery_soc = soc;
    }

private:
    int run_state;  // code_xx      // for the next 15 minutes (this state)
    int speed;      // kph          // for this state
    double estimated_distance;  // km  // for this state
    double travelled_distance; // km   // at the start of the state
    string controlstop_nearest;     // at the start of the state
    string controlstop_next;        // at the start of the state
    double distance_to_controlstop_next;   // at the start of the state
    double power_generated; // W    // for this state
    double power_consumed;  // W    // for this state
    double battery_soc; // 0.xx     // for this state

};


#endif //FORSNUSOLO_AUS_NEW_STATUS_H
