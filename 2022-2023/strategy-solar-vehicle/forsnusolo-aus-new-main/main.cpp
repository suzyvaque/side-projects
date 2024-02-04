//
// Created by Suzy Vaque on 2023-10-14.
//

#include "Car.h"

#include <iostream>
#include <fstream>
#include <filesystem>

int main(){

    Car car = Car();

    int total_states = car.get_total_states();
    int total_race_states = total_states;
    int run_num = 0;    // TODO implement at dynamic - should update this run_num as another iteration

    int time_state = 0;
    string actual_time = car.get_run_time_of(run_num, time_state);
    int run_state = car.v_status_2d[run_num][time_state].get_run_state();
    int speed = car.v_status_2d[run_num][time_state].get_speed();
    double soc = car.v_status_2d[run_num][time_state].get_battery_soc();
    double power_gen = car.v_status_2d[run_num][time_state].get_power_generated();
    double power_con = car.v_status_2d[run_num][time_state].get_power_consumed();
    string controlstop_next = car.v_status_2d[run_num][time_state].get_controlstop_next();
    double distance_to_controlstop = car.v_status_2d[run_num][time_state].get_distance_to_controlstop_next();

    /*
    cout << actual_time << endl;
    cout<<run_state<<":   speed "<<speed<<"kph,   soc "<<soc<<",   power_gen "<<power_gen<<"W,   power_con "<<power_con<<"W,   controlstop_next "<<controlstop_next<<endl; // FIXME erase
    cout<< "Distance to next control stop: "<<distance_to_controlstop<<"km"<<endl<<endl;
    */

    string filename = car.generate_filename_cda();
    // string filename = "cda20_at_17d09h38m.csv";
    car.write_one_csv(filename,actual_time,run_state,speed,soc,power_gen,power_con,controlstop_next,distance_to_controlstop);


    // time_state == 0 is already stored when Car obj is created
    for(int time_state=1; time_state<total_states; time_state++){
        if(car.v_status_2d[run_num][time_state-1].get_run_state() != code_at_finish_line){
            Status status_new = Status();
            car.store_status(run_num, status_new);

            car.v_status_2d[run_num][time_state].set_travelled_distance(car.status_travelled_distance(run_num, time_state));    // (1) whether we've just arrived at a cs, or it's sunset now, we have to merge the previous journeys
            car.v_status_2d[run_num][time_state].set_run_state(car.status_run_state(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_power_generated(car.status_power_generated(run_num, time_state));   // should come before status_speed, since the W for this state should be also considered as a factor
            car.v_status_2d[run_num][time_state].set_speed(car.status_speed(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_estimated_distance(car.status_estimated_distance(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_power_consumed(car.status_power_consumed(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_battery_soc(car.status_battery_soc_static(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_controlstop_next(car.status_controlstop_next(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_controlstop_nearest(car.status_controlstop_nearest(run_num, time_state));
            car.v_status_2d[run_num][time_state].set_distance_to_controlstop_next(car.status_distance_to_controlstop_next(run_num,time_state));

            string actual_time = car.get_run_time_of(run_num, time_state);
            int run_state = car.v_status_2d[run_num][time_state].get_run_state();
            int speed = car.v_status_2d[run_num][time_state].get_speed();
            double soc = car.v_status_2d[run_num][time_state].get_battery_soc();
            double power_gen = car.v_status_2d[run_num][time_state].get_power_generated();
            double power_con = car.v_status_2d[run_num][time_state].get_power_consumed();
            string controlstop_next = car.v_status_2d[run_num][time_state].get_controlstop_next();
            double distance_to_controlstop = car.v_status_2d[run_num][time_state].get_distance_to_controlstop_next();

            /*
            cout << actual_time << endl;
            cout<<run_state<<":   speed "<<speed<<"kph,   soc "<<soc<<",   power_gen "<<power_gen<<"W,   power_con "<<power_con<<"W,   controlstop_next "<<controlstop_next<<endl; // FIXME erase
            cout<< "Distance to next control stop: "<<distance_to_controlstop<<"km"<<endl<<endl;
            */

            car.write_one_csv(filename,actual_time,run_state,speed,soc,power_gen,power_con,controlstop_next,distance_to_controlstop);

            if (car.v_status_2d[run_num][time_state].get_controlstop_next() == "PortAugusta") {
                // for debugging
                if (car.v_status_2d[run_num][time_state].get_distance_to_controlstop_next() < 20) {
                    int temp = 0;
                }
            }
        } else{ // at finish line before day6 17:00
            total_race_states = time_state;
            break;
        }
    }
    car.write_summary(filename,run_num,total_race_states);
}

