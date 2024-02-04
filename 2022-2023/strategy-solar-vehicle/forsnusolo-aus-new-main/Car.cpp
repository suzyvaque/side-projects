//
// Created by Suzy Vaque on 2023-10-14.
//

#include "Car.h"

Car::Car(){
    // reg = ByReg();   // CAUTION : "Object of type 'ByReg' cannot be assigned because its copy assignment operator is implicitly deleted"
    // static_data = StaticData();

    static_data.read_data_from_csv();   // prepare the static solcast data

    v_status_2d.resize(total_states);     // otherwise, vector[run_num].emplace access will cause an error

    // When called, set initial state for static, index [0][0]
    store_status(0,code_race, default_speed,0,0,"Darwin","Katherine",reg.katherine,0,0,0);
    // calculations based on the default speed
    v_status_2d[0][0].set_estimated_distance(status_estimated_distance(0,0));
    v_status_2d[0][0].set_power_generated(status_power_generated(0,0));
    v_status_2d[0][0].set_power_consumed(status_power_consumed_initial(0,0));
    v_status_2d[0][0].set_battery_soc(status_battery_soc_static_initial(0, 0));
}

void Car::store_status(int run_num, Status& status){
    v_status_2d[run_num].emplace_back(status);
}

void Car::store_status(int run_num, int run_state, int speed, double travelled_distance, double estimated_distance, string controlstop_nearest, string controlstop_next, double distance_to_controlstop_next, double power_generated, double power_consumed, double battery_soc){
    Status status_current = Status(run_state, speed, travelled_distance, estimated_distance, controlstop_nearest, controlstop_next, distance_to_controlstop_next, power_generated, power_consumed, battery_soc);
    v_status_2d[run_num].emplace_back(status_current);
}

int Car::status_run_state(int run_num, int time_state){
    int cs_or_adelaide = reached_controlstop(run_num, time_state, v_status_2d[run_num][time_state].get_travelled_distance());
    if(cs_or_adelaide != -1){
        return cs_or_adelaide;  // code_at_finishline or code_charge_stop
    }

    // check time next
    if(time_state%96>=0 && time_state%96<=35){
        return code_race;
    } else if(time_state%96>=36 && time_state%96<=41){
        return code_charge_stop;
    } else if(time_state%96>=42 && time_state%96<=89){
        return code_total_stop;
    } else {    // time_state%36>=90 && time_state%96<=95
        return code_charge_stop;
    }
}

int Car::reached_controlstop(int run_num, int time_state, double travelled_distance) {
    string was_controlstop_next = v_status_2d[run_num][time_state - 1].get_controlstop_next();   // this getter at Status class
    double cs_loc= reg.get_distance_from_string(was_controlstop_next); // this getter at ByReg class
    double distance_to_controlstop_next = v_status_2d[run_num][time_state - 1].get_distance_to_controlstop_next();

    if(travelled_distance >= cs_loc){
        if(was_controlstop_next == "Adelaide" && distance_to_controlstop_next == 0.0){
            return code_at_finish_line;
        } else{
            return code_charge_stop;
        }
    } else{
        return -1;
    }
}

int Car::status_speed(int run_num, int time_state){
    int speed = v_status_2d[run_num][time_state - 1].get_speed();
    int run_state = v_status_2d[run_num][time_state].get_run_state();
    int run_state_prev = v_status_2d[run_num][time_state - 1].get_run_state();
    if(run_state == code_charge_stop || run_state == code_total_stop){
        speed = 0;
    } else if(run_state_prev == code_charge_stop || run_state_prev == code_total_stop){  // was at controlstop or sunset but now running again
        speed = default_speed;
    } else{ // run_state == code_race
        double soc_prev = v_status_2d[run_num][time_state - 1].get_battery_soc();
        double power_gen = v_status_2d[run_num][time_state].get_power_generated();
        double distance_to_controlstop_next = v_status_2d[run_num][time_state - 1].get_distance_to_controlstop_next();

        if(distance_to_controlstop_next > distance_near_cs && soc_prev <= soc_should_charge){
            speed = 0;
            return speed;
        } else if(distance_to_controlstop_next <= distance_near_cs && soc_prev <= soc_should_charge_near_cs){
            speed = 0;
            return speed;
        }

        if(speed == 0){ // has stopped and was charging during the race time
            if(distance_to_controlstop_next > distance_near_cs && soc_prev <= soc_can_run){
                return speed;
            } else if(distance_to_controlstop_next <= distance_near_cs && soc_prev <= soc_can_run_near_cs){
                return speed;
            }
        }
        speed = default_speed;

        /*
        if(soc_prev >= soc_high && power_gen >= power_high){
            speed = speed_accelerate(speed);
        } else if(distance_to_controlstop_next <= distance_near_cs){
            speed = speed_accelerate(speed);
        } else if(soc_prev <= soc_low && distance_to_controlstop_next > distance_near_cs){
            speed = speed_decelerate(speed);
        } else if(power_gen <= power_low){
            speed = speed_decelerate(speed);
        }
        */
    }
    return speed;
}

int Car::speed_accelerate(int speed){
    if(speed >= speed_high){
        speed += speed_adjust_small;
    } else if(speed <= speed_low){
        speed += speed_adjust_large;
    } else{
        speed += speed_adjust_med;
    }
    if(speed > max_speed){
        speed = max_speed;
    }
    return speed;
}

int Car::speed_decelerate(int speed){
    if(speed >= speed_high){
        speed -= speed_adjust_large;
    } else if(speed <= speed_low){
        speed -= speed_adjust_small;
    } else{
        speed -= speed_adjust_med;
    }
    if(speed < min_speed){
        speed = min_speed;
    }
    return speed;
}

double Car::status_estimated_distance(int run_num, int time_state){
    return v_status_2d[run_num][time_state].get_speed() * speed_state;
}

double Car::status_travelled_distance(int run_num, int time_state){
    return v_status_2d[run_num][time_state-1].get_travelled_distance() + v_status_2d[run_num][time_state-1].get_estimated_distance();

}

string Car::status_controlstop_nearest(int run_num, int time_state){
    string next_controlstop = v_status_2d[run_num][time_state].get_controlstop_next();
    double travelled_distance = v_status_2d[run_num][time_state].get_travelled_distance();

    string controlstop_prev = reg.v_cs_string[reg.get_csnum_from_string(next_controlstop)-1];

    double cs_loc1 = reg.get_distance_from_string(controlstop_prev);
    double cs_loc2 = reg.get_distance_from_string(next_controlstop);
    double distance1 = travelled_distance - cs_loc1;
    double distance2 = cs_loc2 - travelled_distance;
    if(distance1 < distance2){
        return controlstop_prev;
    } else{
        return next_controlstop;
    }
}

string Car::status_controlstop_next(int run_num, int time_state){
    string was_controlstop_next = v_status_2d[run_num][time_state-1].get_controlstop_next();
    double travelled_distance = v_status_2d[run_num][time_state].get_travelled_distance();

    if(reached_controlstop(run_num, time_state, travelled_distance) != -1){
        int run_state_prev = v_status_2d[run_num][time_state - 1].get_run_state();
        if(run_state_prev == code_charge_stop){
            // already stopped for 2 states, that is, 30 minutes
            int new_csnum = reg.get_csnum_from_string(was_controlstop_next) + 1;
            return reg.v_cs_string[new_csnum];
        } else{
            // prev_prev was code_race, that is, has only stopped for 1 state, that is, 15 minutes
            return was_controlstop_next;
        }
    } else{
        return was_controlstop_next;
    }
}

double Car::status_distance_to_controlstop_next(int run_num, int time_state){
    string next_controlstop = v_status_2d[run_num][time_state].get_controlstop_next();
    double cs_loc= reg.get_distance_from_string(next_controlstop);
    double distance = cs_loc - v_status_2d[run_num][time_state].get_travelled_distance();
    if(distance < 0){
        distance=0;
    }
    return distance;
}

double Car::status_power_generated(int run_num, int time_state){
    string controlstop_nearest = v_status_2d[run_num][time_state].get_controlstop_nearest();
    int index = time_state*(speed_state*60/5);
    vector<double> data = static_data.csstring_to_csvector(controlstop_nearest)[index]; // 5 being the period of data
    double airtemp = data[0];
    double dhi = data[1];
    double dni = data[2];
    double ghi = data[3];
    double zenith = data[4];
    double azimuth = data[5];


    double solar_angle; // deg

    if (v_status_2d[run_num][time_state].get_speed() == 0){   // when car has stopped
        const double panel_angle_max = 30;  // Maximum panel angle that we can adjust manually
        solar_angle = (zenith < panel_angle_max) ? 0 : (zenith - panel_angle_max);
    }
    else{
        solar_angle = zenith;
    }

    double solar_irradiance = dhi + dni * cos(solar_angle * PI / 180);  // W/m^2

    double eff_change_temp = 1 + pv_temp_coef * (airtemp - 25);     // efficiency drop due to temperature
    double solar_energy = solar_irradiance / 1000 * pv_mpp_stc * eff_change_temp;   // W

    mppt_eff = (98.23 + 98.58) / 2 / 100;   // Elmar MPPT eff, avg of 240W and 360W of V_mpp = 60V

    return solar_energy * mppt_eff;
}

double Car::status_power_consumed(int run_num, int time_state){
    double speed_ms = v_status_2d[run_num][time_state].get_speed() / 3.6;                   // m/s
    double speed_ms_prev = v_status_2d[run_num][time_state-1].get_speed() / 3.6;   // m/s

    // rolling resistance
    double rolling_force = crr_coef * mass * g;     // N
    // may have to adjust for control stops (dirt road -> higher Crr)

    // aerodynamic drag
    double aero_force = 0.5 * cda_coef * density * speed_ms * speed_ms; // N

    // gradient force
    double gradient_force = 0;  // may have to adjust through route

    // acceleration resistance
    double acc = (speed_ms - speed_ms_prev) / (speed_state * 3600); // m/s^2
    double acc_force = mass * acc * (1 + eps_coef);
    // may have to add additional force when starting from control stop

    // power consumption at constant speed
    double con_motor_force = rolling_force + aero_force + gradient_force + acc_force; // N
    double con_motor_power = con_motor_force * speed_ms / motor_eff; // W

    /*
    cout<<"Consumption by   aero "<<aero_force<<"N,   roll "<<rolling_force<<"N,   acc "<<acc_force<<"N"<<endl; // FIXME erase
    */

    // minor loss
    // may need to adjust by adding all electrical losses + I^2 R
    double elec_loss = 10;


    if(v_status_2d[run_num][time_state].get_run_state() == code_total_stop){
        return 0;
    } else if(v_status_2d[run_num][time_state].get_run_state() == code_charge_stop) {
        return 10;
    } else if(v_status_2d[run_num][time_state].get_speed() == 0.0) {
        return 10;
    }

    return con_motor_power + elec_loss;
}

double Car::status_power_consumed_initial(int run_num, int time_state){
    double speed_ms = v_status_2d[run_num][time_state].get_speed() / 3.6;                   // m/s
    double speed_ms_prev = 0;   // m/s

    // rolling resistance
    double rolling_force = crr_coef * mass * g;     // N
    // may have to adjust for control stops (dirt road -> higher Crr)

    // aerodynamic drag
    double aero_force = 0.5 * cda_coef * density * speed_ms * speed_ms; // N

    // gradient force
    double gradient_force = 0;  // may have to adjust through route

    // acceleration resistance
    double acc = (speed_ms - speed_ms_prev) / (speed_state * 3600); // m/s^2
    double acc_force = mass * acc * (1 + eps_coef);
    // may have to add additional force when starting from control stop

    // power consumption at constant speed
    double con_motor_force = rolling_force + aero_force + gradient_force + acc_force; // N
    double con_motor_power = con_motor_force * speed_ms / motor_eff; // W

    /*
    cout<<"Consumption by   aero "<<aero_force<<"N,   roll "<<rolling_force<<"N,   acc "<<acc_force<<"N"<<endl; // FIXME erase
    */

    // minor loss
    // may need to adjust by adding all electrical losses + I^2 R
    double elec_loss = minor_loss;

    if(v_status_2d[run_num][time_state].get_run_state() == code_total_stop){
        elec_loss = 0;
    }

    return con_motor_power + elec_loss;
}

double Car::status_battery_soc_static(int run_num, int time_state){
    double power_generated = v_status_2d[run_num][time_state].get_power_generated(); // W
    double power_consumed = v_status_2d[run_num][time_state].get_power_consumed();   // W

    double energy_net = (power_generated - power_consumed) * speed_state; // Wh

    double soc_current = v_status_2d[run_num][time_state-1].get_battery_soc(); // 0.xx
    double soc_update = soc_current + energy_net / battery_capacity_energy;

    if (soc_update > 1.0) {
        soc_update = 1.0;
    }

    return soc_update;
}

double Car::status_battery_soc_static_initial(int run_num, int time_state){
    double power_generated = v_status_2d[run_num][time_state].get_power_generated(); // W
    double power_consumed = v_status_2d[run_num][time_state].get_power_consumed();   // W

    double energy_net = (power_generated - power_consumed) * speed_state; // Wh

    double soc_current = 1.0;
    double soc_update = soc_current + energy_net / battery_capacity_energy;

    if (soc_update > 1.0) {
        soc_update = 1.0;
    }

    return soc_update;
}

double Car::status_battery_soc_acc(int run_num, int time_state){
    // TODO implement at dynamic
    return 0.0;
}