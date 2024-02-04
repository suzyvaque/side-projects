//
// Created by Suzy Vaque on 2023-10-14.
//

#ifndef FORSNUSOLO_AUS_NEW_CAR_H
#define FORSNUSOLO_AUS_NEW_CAR_H

using namespace std;
#include <string>
#include <vector>
#include <math.h>
#include <iomanip>

#include "Status.h"
#include "ByReg.h"
#include "StaticData.h"

class Car {
public:
    Car();
    vector<vector<Status>> v_status_2d;
    ByReg reg;
    StaticData static_data;

    // Status Calculation
    void store_status(int run_num, Status& status);
    void store_status(int run_num, int run_state, int speed, double travelled_distance, double estimated_distance, string controlstop_nearest, string controlstop_next, double distance_to_controlstop_next, double power_generated, double power_consumed, double battery_soc);
    int status_run_state(int run_num, int time_state);
    int status_speed(int run_num, int time_state);
    double status_estimated_distance(int run_num, int time_state);
    double status_travelled_distance(int run_num, int time_state);
    string status_controlstop_nearest(int run_num, int time_state);
    string status_controlstop_next(int run_num, int time_state);
    double status_distance_to_controlstop_next(int run_num, int time_state);
    double status_power_generated(int run_num, int time_state);
    double status_power_consumed(int run_num, int time_state);
    double status_power_consumed_initial(int run_num, int time_state);
    double status_battery_soc_static(int run_num, int time_state);
    double status_battery_soc_static_initial(int run_num, int time_state);
    double status_battery_soc_acc(int run_num, int time_state);

    // Getters
    int get_total_states(){
        return total_states;
    }
    double get_speed_state(){
        return speed_state;
    }
    int get_default_speed(){
        return default_speed;
    }

    // Getters - They call the getters of Status.h
    int get_run_state_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_run_state();
    }
    int get_speed_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_speed();
    }
    double get_travelled_distance_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_travelled_distance();
    }
    double get_estimated_distance_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_estimated_distance();
    }
    string get_controlstop_nearest_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_controlstop_nearest();
    }
    string get_controlstop_next_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_controlstop_next();
    }
    double get_distance_to_next_controlstop_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_distance_to_controlstop_next();
    }
    double get_power_generated_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_power_generated();
    }
    double get_power_consumed_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_power_consumed();
    }
    double get_battery_soc_of(int run_num, int time_state){
        return v_status_2d[run_num][time_state].get_battery_soc();
    }
    string get_run_time_of(int run_num, int time_state){
        string day = to_string(1 + (time_state + 32) / 96);
        string hour = to_string((32 + time_state) % 96 / 4);
        string time = to_string((time_state % 4) * 15);

        if (hour.length() == 1) {
            hour = "0" + hour;
        }
        if (time.length() == 1) {
            time = "0" + time;
        }

        return "Day " + day + " " + hour + ":" + time;
    }



    // Write
    void write_one_csv(const string& filename, const string& actual_time, const int& run_state, const int& speed, const double& soc, const double& power_gen, const double& power_con, const string& controlstop_next, const double& distance_to_controlstop){
        int soc_int = soc*100;
        int power_gen_int = power_gen;
        int power_con_int = power_con;
        int distance_to_controlstop_int = distance_to_controlstop;

        string str = actual_time+","+to_string(run_state)+","+to_string(speed)+","+to_string(soc_int)+","+to_string(power_gen_int)+","+to_string(power_con_int)+","+controlstop_next+","+to_string(distance_to_controlstop_int);

        ifstream file(filename);
        bool fileExists = file.good();   // true if exits

        ofstream outFile(filename, ios::app);

        if (outFile.is_open()){
            if(!fileExists){
                outFile<<"Actual Time, Run State, Speed (kph), SoC (%), Power Generated (W), Power Consumed (W), ControlStop Next, Distance to ControlStop Next (km)\n";
            }
            outFile << str << endl; // write one line and enter
            outFile.close();
        } else{
            cerr << "ERROR : Could not open file" << filename << endl;
        }
    }

    string generate_filename_cda(){
        time_t timer = time(NULL);
        struct tm *current_time = localtime(&timer);
        string mday = to_string(current_time->tm_mday);
        string hour = to_string(current_time->tm_hour);
        string min = to_string(current_time->tm_min);
        string sec = to_string(current_time->tm_sec);
        string filename = mday+"d"+hour+"h"+min+"m"+sec+"s";
        string cda = to_string((int)(100*this->cda_coef));
        string speed_default = to_string(default_speed);

        filename = "cda."+to_string((int)(100*this->cda_coef))+"-constantv"+speed_default+"-"+filename+".csv";
        return filename;
    }

    void write_summary(const string& filename, int run_num, int total_race_states){
        int total_days = 1+(total_race_states/96);

        int daily_speednotzero = 0;
        int daily_sum_power_gen = 0;
        int daily_sum_power_con = 0;
        int daily_sum_low_soc_stop = 0;
        int daily_sum_speed = 0;

        int daily_average_power_gen[total_days];
        int daily_average_power_con[total_days];
        int daily_average_low_soc_stop[total_days];
        int daily_average_speed[total_days];
        int daily_initial_soc[total_days];
        int daily_final_soc[total_days];
        int daily_final_distance_to_adelaide[total_days];
        int daily_final_travelled_distance[total_days];

        int final_soc;
        int final_travelled_distance;
        int final_average_speed = 0;

        for(int state=0; state < total_race_states; state++){
            int day = state / 96;   // 0 to 5

            if(get_run_state_of(run_num, state) == code_race){
                if(get_speed_of(run_num, state)==0){
                    // has stopped due to low soc
                    daily_sum_low_soc_stop++;
                } else{
                    // only when speed > 0
                    daily_speednotzero++;
                    daily_sum_speed += get_speed_of(run_num, state);
                    daily_sum_power_con += get_power_consumed_of(run_num, state);
                    daily_sum_power_gen += get_power_generated_of(run_num, state);
                }
            }

            if(state == 0){
                // Day 1, 7:45
                daily_initial_soc[day] = 100;
            } else if(state!=1 && state%96 == 1){
                // Day 2 to 6, at 7:45 AM
                daily_initial_soc[day] = get_battery_soc_of(run_num, state) * 100;  // %
            } else if(state%96 == 36 || state == total_race_states-1){
                // at 17:00, right after race of the day ends
                // or at day 6, 16:45
                if(daily_speednotzero == 0){
                    daily_average_power_gen[day] = 0;
                    daily_average_power_con[day] = 0;
                    daily_average_speed[day] = 0;
                } else{
                    daily_average_power_gen[day] = daily_sum_power_gen / daily_speednotzero;
                    daily_average_power_con[day] = daily_sum_power_con / daily_speednotzero;
                    daily_average_speed[day] = daily_sum_speed / daily_speednotzero;
                }
                daily_average_low_soc_stop[day] = daily_sum_low_soc_stop;
                daily_final_soc[day] = get_battery_soc_of(run_num, state) * 100;

                if(day==0){
                    daily_final_travelled_distance[day] = get_travelled_distance_of(run_num, state);
                } else{
                    daily_final_travelled_distance[day] = get_travelled_distance_of(run_num, state) - get_travelled_distance_of(run_num, state-36);
                }
                daily_final_distance_to_adelaide[day] = reg.adelaide - get_travelled_distance_of(run_num, state);
            }
        }

        final_soc = get_battery_soc_of(run_num,total_race_states-1) * 100;
        final_travelled_distance = get_travelled_distance_of(run_num,total_race_states-1);
        for(int day=0; day<total_days; day++){
            final_average_speed += daily_average_speed[day];
        }
        final_average_speed = final_average_speed / total_days;

        string filename_summary = "summary.csv";
        ofstream outFile(filename_summary, ios::app);
        if (outFile.is_open()){

            outFile<<"summary for "<<filename<<", , , , , , , , \n";
            outFile<<" , , , , , , , , \n";
            outFile<<"Raced Until, Average Speed, Total Travelled Distance (km), Final SoC (%), Default Speed, CdA, , , \n";
            string data = get_run_time_of(run_num,total_race_states)+","+to_string(final_average_speed)+","+to_string(final_travelled_distance)+","+to_string(final_soc)+","+to_string(default_speed)+","+to_string(cda_coef);
            outFile<<data<<", , , \n";

            outFile<<" , , , , , , , , \n";
            outFile<<"Day, Average Power Gen (W), Average Power Con (W), Average Speed (kph), SoC at 8:00 (%), SoC at 17:00 (%), Number of Stops due to Low SoC, Travelled Distance Today (km), Distance to Adelaide (km)\n";

            for(int day=0; day<total_days; day++){
                // 8 data
                outFile << to_string(day+1) << "," << to_string(daily_average_power_gen[day]) << "," << to_string(daily_average_power_con[day]) << "," << to_string(daily_average_speed[day]) << "," << to_string(daily_initial_soc[day]) << "," << to_string(daily_final_soc[day]) << "," << to_string(daily_average_low_soc_stop[day]) << "," << to_string(daily_final_travelled_distance[day]) << "," << to_string(daily_final_distance_to_adelaide[day]) << "\n";
            }
            outFile<<" , , , , , , , , \n";
            outFile<<" , , , , , , , , \n";
            outFile<<" , , , , , , , , \n";
            outFile.close();
        } else{
            cerr << "ERROR : Could not open file" << filename << endl;
        }

    }




private:
// [1] Coefficients

    // [1-1] fixed by nature
    const double g = 9.8066;
    const double density = 1.15;
    const double PI = 3.1415926;

    // [1-2] fixed by user
    const int data_num = 1548;
    const int hour = 60;    // minutes
    const int data_period = 5;  // minutes
    const double speed_state = 0.25;  // that is, 15 minutes

    const int total_states = data_num / (speed_state * hour / data_period);

    // [1-3] car spec, related to consumption
    const double mass = 270.0;      // with driver
    const double cda_coef = 0.2;
    const double crr_coef = 0.005;  // can test with our car
    const double eps_coef = 0.07;   // equivalent mass coefficent of rotation

    double motor_eff = 0.947;   // FIXME have to adjust with motor and wheel specification
    double minor_loss = 50; // W

    // [1-4] car spec, related to generation
    double mppt_eff = 0.981;
    double pv_eff = 0.23;
    const double pv_temp_coef = -0.0026;

    double pv_area = 3.947786;  // m^2 (144 M6 cells)
    double pv_mpp_stc = 880.836; // W (MPP at STC condition, suppose best efficient modules)
    double battery_capacity_energy = 5065;  // Wh


// [2] needed for calculating status_run_state

    int reached_controlstop(int run_num, int time_state, double travelled_distance);


// [3] needed for calculating status_speed

    int speed_accelerate(int speed);
    int speed_decelerate(int speed);
    const int default_speed = 62;       // kph
    const int max_speed = 80;
    const int min_speed = 55;
    const int speed_high = 90;
    const int speed_low = 50;
    const int speed_adjust_large = 0;  // kph
    const int speed_adjust_med = 0;
    const int speed_adjust_small = 0;
    const double soc_high = 0.75;   // 1.0 is 100%
    const double soc_low = 0.20;
    const double soc_min = 0.10;

    const double soc_should_charge = 0.15;
    const double soc_can_run = 0.20;
    const double soc_should_charge_near_cs = 0.10;
    const double soc_can_run_near_cs = 0.15;

    const double distance_near_cs = 30; // km
    const double power_high = 800;  // W
    const double power_low = 500;

};


#endif //FORSNUSOLO_AUS_NEW_CAR_H
