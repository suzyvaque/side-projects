import java.util.ArrayList;
import java.util.Collections;

public class Vehicle {
    public boolean FINISHED_RACE =false;

    public boolean constantV_true= false;
    public int constantV;
    public void setConstantV(int constantV){
        this.constantV_true= true;
        this.constantV= constantV;
    }

    final double g = 9.8066;
    final double density = 1.15;

    private double mass = 260.0;
    private double k_regen = 0.3;   // k_ : coefficient
    private double k_crr = 0.005;
    private double cda = 0.2;
    private double k_pedal = 0.1;
    private double eff_motor = 0.95;
    private double minor_loss = 50;   //W

    private double cellAngle= 0;    // degrees
    private double cellArea= 4.0; // m^2
    private double eff_pv= 0.225;
    private double k_pv = -0.0026;    // temperature coefficient per celsius
    private double eff_mppt = 0.99;

    private double p_consumption = 3000;
    private double nominal_v = 100;
    private double internal_r = 0.0653;
    private double soc_max = 1;
    private double soc_min = 0.1;
    private double battery_capacity = 5065;
    private double soc; // 0.1 ~ 1.0
    private double e_battery;   // energy Wh saved in battery
    private double heat_loss;

    final double timeState = 0.0833;   // 5 minutes
    private int stateNum = 1;
    private int speed;  // will be rounded
    final private ArrayList<Integer> speedList= new ArrayList<>(1524);
    final private ArrayList<Integer[]> penaltyList= new ArrayList<>(1524);    // when soc<0, stateNum
    final private ArrayList<Integer> socList= new ArrayList<>(1524);
    final private ArrayList<Double> inList= new ArrayList<>(1524);
    final private ArrayList<Double> inList_solar= new ArrayList<>(1524);
    final private ArrayList<Double> inList_regen= new ArrayList<>(1524);
    final private ArrayList<Double> outList= new ArrayList<>(1524);
    final private ArrayList<Double> outList_air= new ArrayList<>(1524);
    final private ArrayList<Double> outList_roll= new ArrayList<>(1524);
    final private ArrayList<Double> outList_acc= new ArrayList<>(1524);
    private double travelledDistance= 0;    // Simply, each state speed * state num
    private int totalStates_goal = 1524;  // day4-948, day5-1236, day6-1524
    private double time_left;

    Route route;
    private int formerCSNum= 0; // promptly passed (or even current)
    private ControlStop formerCS;
    private ControlStop nextCS;
    private double toTravel;
    private int formerSSNum=0;  // number of days-1

    private boolean atCS= false;
    private int csStateNum;
    private boolean duringSS= false;
    private int ssStateNum;

    public double getSoC(){return this.soc;}
    public int getStateNum(){return this.stateNum;}
    public ArrayList<Integer> getSpeedList(){return this.speedList;}
    public ArrayList<Integer[]> getPenaltyList(){return this.penaltyList;}
    public ArrayList<Integer> getSoCList(){return this.socList;}
    public ArrayList<Double> getInList(){return this.inList;}
    public ArrayList<Double> getInList_solar(){return this.inList_solar;}
    public ArrayList<Double> getInList_regen(){return this.inList_regen;}
    public ArrayList<Double> getOutList(){return this.outList;}
    public ArrayList<Double> getOutList_air(){return this.outList_air;}
    public ArrayList<Double> getOutList_roll(){return this.outList_roll;}
    public ArrayList<Double> getOutList_acc(){return this.outList_acc;}
    public double getTravelledDistance(){return this.travelledDistance;}

    public double getMass(){return this.mass;}
    public double getCda(){return this.cda;}
    public double getCellAngle(){return this.cellAngle;}
    public double getEff_pv(){return this.eff_pv;}
    public double getK_pv(){return this.k_pv;}
    public double getEff_mppt(){return this.eff_mppt;}
    public double getBattery_capacity(){return this.battery_capacity;}
    public double getSoC_min(){return this.soc_min;}
    public double getSoC_max(){return this.soc_max;}
    public double getK_regen(){return this.k_regen;}
    public double getK_pedal(){return this.k_pedal;}
    public int getTotalStates_goal(){return this.totalStates_goal;}

    public void setMass(double mass){this.mass= mass;}
    public void setCdA(double cda){this.cda= cda;}
    public void setEff_pv(double eff_pv){this.eff_pv= eff_pv;}
    public void setCellAngle(double angle){this.cellAngle= angle;}
    public void setCellArea(double area){this.cellArea= area;}
    public void setK_regen(double k_regen){this.k_regen= k_regen;}
    public void setMinor_loss(double minor_loss){this.minor_loss= minor_loss;}
    public void setSoc_bounds(double min, double max){this.soc_min= min;  this.soc_max= max;}
    public void setSoc_bounds_forAlteringV(double low, double high){this.soc_low=low;  this.soc_high= high;}
    public void setTotalStates_goal(int states){this.totalStates_goal=states;}

    Vehicle(Route route){
        this.soc= 1;
        this.e_battery= this.battery_capacity;
        // this.heat_loss= Math.pow((this.p_consumption/this.nominal_v),2)*this.internal_r;
        this.route= route;

        this.formerCS= route.stops.get(formerCSNum);
        this.nextCS= route.stops.get(formerCSNum+1);
        this.toTravel= route.stops.get(formerCSNum).distanceTilNext;

        this.speedList.add(0);
        if(this.constantV_true){this.speed= this.constantV;}
        else{this.speed= default_speed(route);}
    }

    public void run(){
        if(checkCS()){this.speed= 0;}   // checkCS should come first; overlaps will be considered in checkSunset later on
        else if(checkSunset()){this.speed= 0;}
        else{
            if(this.constantV_true){this.speed= this.constantV;}
            else{this.speed= alter_speed();}

            updateDistance();
        }

        this.speedList.add(this.speed);
        updateSOC();
        this.stateNum++;
    }

    private int default_speed(Route route){
        // when v_T-1==0 && atCS==false // done at checkCS, checkSS.
        // FIXME number of states w/o v==0 moments
        this.time_left = (this.totalStates_goal -this.stateNum)*0.4*this.timeState;   // approximately...out of total, 40% driveable
        double average=(route.total_distance-this.travelledDistance)/(time_left);
        // double average =75;
        return (int)Math.round(average*(0.5*this.soc+0.6))-15;
    }

    // private int v_min =;
    private int v_max = 120;
    private double soc_low = 0.3;
    private double soc_high = 0.6;
    // private int nearCS = 40; // km left until next CS -distance
    // private int nearSS = 6; // states left until next SS-state
    private double alter_rate_stop = 0.02;
    private double alter_rate_e1 = 0.1;
    private double alter_rate_e2 = 0.01;

    public void setV_max(int v){this.v_max=v;}

    private int alter_speed(){
/*
        if(this.toTravel-this.travelledDistance<40) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(78<this.stateNum && this.stateNum<=84) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(366<this.stateNum && this.stateNum<=372) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(654<this.stateNum && this.stateNum<=660) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(942<this.stateNum && this.stateNum<=948) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(1230<this.stateNum && this.stateNum<=1236) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(1518<this.stateNum && this.stateNum<=1524) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(1806<this.stateNum && this.stateNum<=1812) {return (int)(this.speed*(1-this.alter_rate_stop));}
        else if(2094<this.stateNum && this.stateNum<=2100) {return (int)(this.speed*(1-this.alter_rate_stop));}
        // FIXME need prediction
        else*/ if(this.soc<this.soc_low){this.speed= (int)(this.speed*(1-alter_rate_e2));}
        else if(this.soc>this.soc_high&&this.stateNum>20){this.speed= (int)(this.speed*(1+alter_rate_e1));}

        if(this.speed>v_max) {this.speed=v_max; return v_max;}
        else{return this.speed;}
    }

    public void updateSOC(){
        double speedDiff= this.speed-this.speedList.get(stateNum-1);
        double e_in;
        double e_out;
        if(speedDiff>=0){
            e_in= e_solar()*this.timeState;
            e_out= e_con()*this.timeState;
            this.inList_regen.add(0.0);    // W
        } else {
            e_in= (e_solar()+e_con()*this.k_regen)*this.timeState;
            this.inList_regen.add((0.5*this.mass*this.speed*k_regen));    // W
            // this.inList_regen.add((int)(e_con()*this.k_regen));    // W
            e_out=0;
        }

        /*  // PV-MOTOR DIRECT
        // Wh
        double e_margin= e_in-e_out;
        if(e_margin>0){
            if(this.e_battery+e_margin>= this.battery_capacity*soc_max){this.e_battery= this.battery_capacity*soc_max;}
            else{this.e_battery+= e_margin;}
        } else {
            this.e_battery-=e_margin;
            if(this.e_battery<=this.battery_capacity*soc_min){
                // this.e_battery=this.battery_capacity*soc_min;   // FIXME..............
                Integer arr[]= {this.stateNum,this.speed};
                this.penaltyList.add(arr);
            }
        }
        */

        // PV-MOTOR INDIRECT
        double e_total= e_in-e_out;
        if(this.e_battery+e_total>=this.battery_capacity*soc_max){this.e_battery=this.battery_capacity*soc_max;}
        else if(this.e_battery+e_total<=this.battery_capacity*soc_min) {
            this.e_battery = this.battery_capacity * soc_min;
            this.e_battery = this.battery_capacity * soc_min;
            Integer arr[] = {this.stateNum, this.speed};
            this.penaltyList.add(arr);
        }
        else{this.e_battery+=e_total;}

        this.soc= this.e_battery/this.battery_capacity;

        this.socList.add((int)(this.soc*100));
        this.inList.add((e_in));  // Wh
        this.outList.add((e_out));  // Wh
    }

    private double solar_angle(){
        double ele= 90-this.formerCS.zenith.get(stateNum);
        double tilt= this.cellAngle;
        double azi_sol= this.formerCS.azimuth.get(stateNum);
        double azi_loc= this.formerCS.azi_loc;
        double angle= Math.sin(tilt)*Math.cos(ele)* (Math.sin(azi_loc)*Math.sin(azi_sol)-Math.cos(azi_loc)*Math.cos(azi_sol))+ Math.cos(tilt)*Math.sin(ele);
        return angle;
    }

    private double solar_angle_re(){
        System.out.println(stateNum+" : zen--> "+this.formerCS.zenith.get(stateNum));
        return Math.cos( Math.toRadians(this.formerCS.zenith.get(stateNum)));
    }

    private double e_solar_atStop(){
        double solar = (this.formerCS.dhi.get(stateNum)+this.formerCS.dni.get(stateNum)) * this.cellArea;
        double d_t= this.formerCS.temp.get(stateNum)-25;
        double eff;
        if(d_t<0){eff = this.eff_pv*Math.pow((1+this.k_pv),d_t);}
        else{eff = this.eff_pv*Math.pow((1-this.k_pv),d_t);}

        this.inList_solar.add((solar*eff));   // W
        return solar*eff*this.eff_mppt; // W
    }

    private double e_solar(){
        double solar_angle;
        if(this.speed==0){solar_angle=1;}   // Already e_solar_atStop
        else{
            // solar_angle= Math.toRadians(solar_angle());
            solar_angle= solar_angle_re();
        }

        System.out.println(stateNum+" : cos--> "+solar_angle);

        // else {solar_angle=Math.toRadians(this.formerCS.zenith.get(stateNum)+this.cellAngle);}    // angle in degrees
        // double solar = this.formerCS.dni.get(stateNum) * Math.cos(solar_angle) + this.formerCS.dhi.get(stateNum); // FIXME
        double solar = this.formerCS.dni.get(stateNum) * solar_angle + this.formerCS.dhi.get(stateNum);
        solar = solar*this.cellArea;

        double d_t= this.formerCS.temp.get(stateNum)-25;
        double eff;
        if(d_t<0){eff = this.eff_pv*Math.pow((1+this.k_pv),d_t);}
        else{eff = this.eff_pv*Math.pow((1-this.k_pv),d_t);}

        this.inList_solar.add((solar*eff));   // W    // FIXME
        // System.out.println(stateNum+" in (W): "+solar*eff*this.eff_mppt);
        return solar*eff*this.eff_mppt; // W
    }

    private double apparentWind(ControlStop formerCS){
        double wd= formerCS.windDir.get(stateNum);
        double angle;
        if(wd>180){angle= wd-180+formerCS.azi_loc;}
        else{angle= 180-wd-formerCS.azi_loc;}

        double ws= formerCS.windSpeed.get(stateNum);
        double temp= Math.pow(ws,2)+Math.pow(this.speed,2)+2*ws*this.speed*Math.cos(angle);
        return Math.sqrt(temp);
    }

    private double e_con(){
        double speed_ms= (int)(this.speed/3.6);
        double speed_ms_prev= (int)(this.speedList.get(stateNum-1)/3.6);

        // FIXME eff_motor? 02-10
        /*
        double rolling_and_air= this.eff_motor * (this.mass * g * speed_ms * this.k_crr) +
                0.5 * this.cda * this.density * Math.pow((this.speed - apparentWind(this.formerCS)),3);
        // Vw= root( Sw^2 + Sv^2 + 2*Sw*Sv*cos(vehicleDirection - windDirection) )
        */
        double rolling= this.eff_motor * (this.mass * g * this.k_crr) * speed_ms;
        double air= 0.5 * this.cda * this.density * Math.pow(this.speed,2);
        // double rolling= this.eff_motor * (this.mass * g * (this.speed - apparentWind(this.formerCS)) * this.k_crr);
        // double air= 0.5 * this.cda * this.density * Math.pow((this.speed - apparentWind(this.formerCS)),2);

        double speedDiff= speed_ms-speed_ms_prev;
        double acc= (speedDiff/(this.timeState*60*60))*this.g*(this.mass+this.k_pedal);
        if(acc<0){acc=acc*(-1);}

        double resistance= rolling + air + acc;

        this.outList_air.add(air);
        this.outList_roll.add(rolling);
        this.outList_acc.add(acc);

        /*  // FIXME outdated 02-10
        double rolling= this.mass*this.g*this.k_crr;
        double air= 0.5*this.density*this.cda*Math.pow(speed_ms,2);
        double speedDiff= speed_ms-speed_ms_prev;
        double acc= (speedDiff/(this.timeState*60*60))*this.g*(this.mass+this.k_pedal);
        double resistance= rolling+air+acc;
        */

        /*
        this.heat_loss= Math.pow((resistance/this.nominal_v),2)*this.internal_r;    // FIXME CHECK 01-25
        double loss;
        if(this.speed!=0){loss= this.heat_loss+this.minor_loss;}
        else{loss=0;}    // FIXME CHECK 01-25

        // System.out.println(String.format("%.2f,%.2f,%.2f: %.2f/// loss: %.2f /// total consumed: %.2f",rolling,air,acc,resistance*speed_ms,loss,(resistance*speed_ms+loss)*this.timeState));
        return (resistance*speed_ms+loss);    // W
         */
        double loss;
        if(this.speed!=0){loss= this.minor_loss;}
        else{loss=0;}

        return (resistance+loss);    // W
    }

    public void updateDistance(){
        // DO NOT USE SPEED LIST; SPEED LIST IS UPDATED AFTER!!! UPDATING LOCATION
        this.travelledDistance+= this.speed * this.timeState;
    }

    public void updateToTravel(){
        this.toTravel+= this.formerCS.distanceTilNext;
    }

    private boolean checkCS(){
        if(!atCS){
            if(this.travelledDistance>=this.toTravel){
                if(this.nextCS.distanceTilNext==0){this.FINISHED_RACE = true; return true;}   // at Adelaide
                this.atCS= true;
                this.csStateNum= this.stateNum;
                this.formerCSNum++;
                this.formerCS= this.route.stops.get(formerCSNum);
                this.nextCS= this.route.stops.get(formerCSNum+1);
                this.travelledDistance= this.toTravel;  // for (minor) error correction
                updateToTravel();
            }
        } else{ // atCS==true
            // System.out.print(stateNum+": CS   ");
            if(this.csStateNum+6<=this.stateNum){
                this.atCS= false;   // can resume running after 30minutes
                this.speed= default_speed(route);
            }
        }
        return atCS;
    }

    private boolean checkSunset(){
        if(!duringSS){
            if(this.stateNum>=85+288*formerSSNum){  // 5pm!!! However, CS overlap may happen- should be >=, not ==
                this.ssStateNum= this.stateNum;
                this.duringSS= true;
            }
        } else{ // duringSS==true
            if(this.stateNum==265+288*formerSSNum){    // CS overlap may happen- cannot simply check if 15hours have passed(this.ssStateNum+180<=this.stateNum)
                this.duringSS= false;
                formerSSNum++;
                this.speed= default_speed(route);
            }
        }
        return duringSS;
    }
}

