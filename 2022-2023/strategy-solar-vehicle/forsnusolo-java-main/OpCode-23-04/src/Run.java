import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;

public class Run {
    public static void main(String[] args) {
        Route route= new Route();
        Vehicle vehicle= new Vehicle(route);

        vehicle.setCellArea(4.0);
        vehicle.setMass(280);
        vehicle.setCdA(0.3);
        vehicle.setK_regen(0.3);
        vehicle.setMinor_loss(50);

        vehicle.setEff_pv(0.225);
        // vehicle.setEff_pv(0.22);
        // vehicle.setEff_pv(0.215);
        // vehicle.setEff_pv(0.21);

        vehicle.setTotalStates_goal(1236);  // NOT CONSTANT  // day4-948, day5-1236, day6-1524
        // vehicle.setConstantV(70);    // CONSTANT
        // vehicle.setConstantV(65);    // CONSTANT
        // vehicle.setConstantV(60);    // CONSTANT


        while(!vehicle.FINISHED_RACE && vehicle.getStateNum()<=2100){
            vehicle.run();
        }
        // writeResults_eff_csv("eff-23-02-temp.csv",vehicle);

        String temp;
        if(vehicle.constantV_true){temp= ""+vehicle.constantV;}
        else{temp="False";}
        // String name= "eff-"+vehicle.getEff_pv()*100+"-Constant"+temp+".csv";
        // writeResults_forOne_csv(name,vehicle);
        // System.out.println("speed size"+vehicle.getSpeedList().size());
        // System.out.println("soc size"+vehicle.getSoCList().size());
        // System.out.println("solarInput size"+vehicle.getInList_solar().size());
        // writeResults_forOne_csv("23-03-10-eff-"+vehicle.getEff_pv()*100+"-Constant-"+temp+".csv",vehicle);
        writeResults_forOne_csv("23-04-14-eff21-Constant-"+temp+".csv",vehicle);
        writeResults_eff_csv("23-04-14-summary.csv",vehicle);
    }


    private static void writeResults_forOne_csv(String filename, Vehicle vehicle){
        File file = new File(filename);
        try {
            FileWriter fileWriter = new FileWriter(file, true);   // new file made

            List<Integer> woZero= new ArrayList<>(vehicle.getSpeedList());
            woZero.removeAll(Arrays.asList(Integer.valueOf(0)));
            IntSummaryStatistics statistics_final = woZero.stream().mapToInt(num -> num).summaryStatistics();

            List<Double> solar= new ArrayList<>(vehicle.getInList_solar());
            DoubleSummaryStatistics statistics_solar = solar.stream().mapToDouble(num -> num).summaryStatistics();

            String target;
            if(vehicle.constantV_true){target="INVALID";}
            else{target=checkDay(vehicle,vehicle.getTotalStates_goal());}
            String info= String.format("%.2f,%b,%.2f,%.1f,%s,%s,%.2f,%d,%.2f,%.2f",vehicle.getEff_pv()*100,vehicle.constantV_true,statistics_final.getAverage(),woZero.size()*5.0/60.0,target,checkDay(vehicle),vehicle.getSoC()*100,vehicle.getPenaltyList().size(),statistics_solar.getSum()/1000.0,statistics_solar.getAverage());
            fileWriter.write("pv efficiency (%),constant v (t/f),average v (kph),actual travel time (hr),arrival target (days),arrival,SoC at arrival (%),SoC<10% (times in 5m units),total E input (kWh),average E input (W),-,-");
            fileWriter.write("\n"+info+",-,-"+"\n");

            fileWriter.write("\ntime,speed (kph),SoC (%),solar input (W/m^2),final solar input (W),final solar input (Wh),regen input (W),total state output (Wh),output (W),air (W),roll (W),acc (W)");

            // FIXME size?? speedlist+1==soclist==inputlist
            for(int i=1; i<vehicle.getSpeedList().size()-1; i++){
                String day= checkDay(i);
                int speed= vehicle.getSpeedList().get(i);
                int SoC= vehicle.getSoCList().get(i);
                double in_sol= vehicle.getInList_solar().get(i);
                double in_fin_w= vehicle.getInList().get(i)/vehicle.timeState;
                double in_fin_wh= vehicle.getInList().get(i);
                double in_regen= vehicle.getInList_regen().get(i);
                double out_wh= vehicle.getOutList().get(i);
                double out_w= vehicle.getOutList().get(i)/vehicle.timeState;
                double out_air_w= vehicle.getOutList_air().get(i);;
                double out_roll_w= vehicle.getOutList_roll().get(i);;
                double out_acc_w= vehicle.getOutList_acc().get(i);;
                fileWriter.write("\n"+day+","+speed+","+SoC+","+in_sol+","+in_fin_w+","+in_fin_wh+","+in_regen+","+out_wh+","+out_w+","+out_air_w+","+out_roll_w+","+out_acc_w);
            }

            fileWriter.close();
        } catch (IOException e) {e.printStackTrace();}
    }


    private static void writeResults_eff_csv(String filename, Vehicle vehicle){
        File file = new File(filename);
        try {
            FileWriter fileWriter = new FileWriter(file, true);   // new file made

            List<Integer> woZero= new ArrayList<>(vehicle.getSpeedList());
            woZero.removeAll(Arrays.asList(Integer.valueOf(0)));
            IntSummaryStatistics statistics_final = woZero.stream().mapToInt(num -> num).summaryStatistics();

            List<Double> solar= new ArrayList<>(vehicle.getInList_solar());
            DoubleSummaryStatistics statistics_solar = solar.stream().mapToDouble(num -> num).summaryStatistics();

            String target;
            if(vehicle.constantV_true){target="INVALID";}
            else{target=checkDay(vehicle,vehicle.getTotalStates_goal());}
            String info= String.format("%.2f,%b,%.2f,%.1f,%s,%s,%.2f,%d,%.2f,%.2f",vehicle.getEff_pv()*100,vehicle.constantV_true,statistics_final.getAverage(),woZero.size()*5.0/60.0,target,checkDay(vehicle),vehicle.getSoC()*100,vehicle.getPenaltyList().size(),statistics_solar.getSum()/1000.0,statistics_solar.getAverage());

            fileWriter.write("\n"+info);
            fileWriter.close();
        } catch (IOException e) {e.printStackTrace();}
    }

    private static void writeResults_solar(String filename, Vehicle vehicle){
        File file = new File(filename);
        try {
            FileWriter fileWriter = new FileWriter(file, true);   // new file made

            String info= String.format(
                    "[Mass: %.1fkg  -  CdA: %.2fm^2  -  Cell Angle: %.2f  -  PV Efficiency %.2f%%  -  Temperature Coefficient: %.2f%%  -  MPPT Efficiency: %.2f%%  -  Battery Capacity: %.3fkWh  -  SoC Bounds: %d%%~%d%%  -  Regenerative Efficiency: %.2f%%  -  Acceleration Coefficient: %.2f",
                    vehicle.getMass(),vehicle.getCda(),vehicle.getCellAngle(),vehicle.getEff_pv()*100,vehicle.getK_pv()*100,vehicle.getEff_mppt()*100,vehicle.getBattery_capacity()/1000,(int)(vehicle.getSoC_min()*100),(int)(vehicle.getSoC_max()*100),vehicle.getK_regen()*100,vehicle.getK_pedal());
            // fileWriter.write("\n"+info);

            fileWriter.write("\nFor Cell Angle "+vehicle.getCellAngle()+" degrees:");
            List<Double> solar= new ArrayList<>(vehicle.getInList_solar());
            DoubleSummaryStatistics statistics_solar = solar.stream().mapToDouble(num -> num).summaryStatistics();
            fileWriter.write(String.format("\n*Total Solar Energy: %.2fkW",statistics_solar.getSum()/1000.0));
            fileWriter.write(String.format("\n*Average Solar Energy: %.2fW",statistics_solar.getAverage()));
/*
            fileWriter.write("\n\nSolar Input Change (W): "+vehicle.getInList_solar().toString());
            fileWriter.write("\n*Arrival: "+checkDay(vehicle));
            fileWriter.write(String.format("\n*SoC at Arrival: %.2f%%",vehicle.getSoC()*100));
            fileWriter.write(String.format("\n*Number of States w SoC Under %d%%: %d",(int)(vehicle.getSoC_min()*100),vehicle.getPenaltyList().size()));

            List<Integer> woZero= new ArrayList<>(vehicle.getSpeedList());
            woZero.removeAll(Arrays.asList(Integer.valueOf(0)));
            IntSummaryStatistics statistics_final = woZero.stream().mapToInt(num -> num).summaryStatistics();
            fileWriter.write(String.format("\n*Average Speed w/o Zero: %.2fkph",statistics_final.getAverage()));

            fileWriter.write("\n\nSoC (%) Change: "+vehicle.getSocList().toString());
            fileWriter.write("\nInput (Wh) Change: "+vehicle.getInList().toString());
*/
            fileWriter.write("\n\n==============================================================================================\n");
            fileWriter.close();
        } catch (IOException e) {e.printStackTrace();}
    }

    private static void writeResults(String filename, Vehicle vehicle){
        File file = new File(filename);
        try {
            FileWriter fileWriter = new FileWriter(file, true);   // new file made

            String info= String.format(
                    "*Mass: %.1fkg \n*CdA: %.2fm^2 \n*Cell Angle: %.2fdegrees tilted from the ground \n*PV Efficiency / Temperature Coefficient: %.2f%% / %.2f%% \n*MPPT Efficiency: %.2f%% \n*Battery Capacity: %.3fkWh \n*SoC Bounds: %d%%~%d%% \n*Regenerative Efficiency: %.2f%% \n*Acceleration Coefficient: %.2f",
                    vehicle.getMass(),vehicle.getCda(),vehicle.getCellAngle(),vehicle.getEff_pv()*100,vehicle.getK_pv()*100,vehicle.getEff_mppt()*100,vehicle.getBattery_capacity()/1000,(int)(vehicle.getSoC_min()*100),(int)(vehicle.getSoC_max()*100),vehicle.getK_regen()*100,vehicle.getK_pedal());
            fileWriter.write("\nVehicle Info\n"+info+"\n------------------------------------------");

            fileWriter.write("\nRace_Solar Info");
            List<Double> solar= new ArrayList<>(vehicle.getInList_solar());
            DoubleSummaryStatistics statistics_solar = solar.stream().mapToDouble(num -> num).summaryStatistics();
            fileWriter.write(String.format("\n*Total Solar Energy Generated w Cell at %.2fdegrees: %.2fkW",vehicle.getCellAngle(),statistics_solar.getSum()/1000.0));
            fileWriter.write(String.format("\n*Average Solar Energy Generated w Cell at %.2fdegrees: %.2fW",vehicle.getCellAngle(),statistics_solar.getAverage()));
            fileWriter.write("\n\nSolar Input Change (W) w Cell at "+vehicle.getCellAngle()+"degrees: "+vehicle.getInList_solar().toString());
            fileWriter.write("\n------------------------------------------");

            fileWriter.write("\nRace Info\n***Race Time Goal: "+checkDay(vehicle,vehicle.getTotalStates_goal())+" days");
            fileWriter.write("\n*Arrival: "+checkDay(vehicle));
            fileWriter.write(String.format("\n*SoC at Arrival: %.2f%%",vehicle.getSoC()*100));
            fileWriter.write(String.format("\n*Number of States w SoC Under %d%%: %d",(int)(vehicle.getSoC_min()*100),vehicle.getPenaltyList().size()));

            List<Integer> woZero= new ArrayList<>(vehicle.getSpeedList());
            woZero.removeAll(Arrays.asList(Integer.valueOf(0)));
            IntSummaryStatistics statistics_final = woZero.stream().mapToInt(num -> num).summaryStatistics();
            fileWriter.write(String.format("\n*Average Speed w/o Zero: %.2fkph",statistics_final.getAverage()));

            // fileWriter.write("\n------------------------------------------");

            fileWriter.write("\n\nSpeed (kph) Change: "+vehicle.getSpeedList().toString());

            //fileWriter.write("\n\nSoC (%) Change: "+vehicle.getSocList().toString());
            fileWriter.write("\nInput (Wh) Change: "+vehicle.getInList().toString());
            fileWriter.write("\nOutput (Wh) Change: "+vehicle.getOutList().toString());
            fileWriter.write("\n\n==============================================================================================\n");
            fileWriter.close();
        } catch (IOException e) {e.printStackTrace();}
    }

    private static String checkDay(Vehicle vehicle, int states){
        String day;
        if(states<=84){day="1";}
        else if(states<=372 && states>=265){day="2";}
        else if(states<=660 && states>=553){day="3";}
        else if(states<=948 && states>=841){day="4";}
        else if(states<=1236 && states>=1129){day="5";}
        else if(states<=1524 && states>=1417){day="6";}
        else{day="TAKES MORE THAN 6 DAYS";}
        return day;
    }

    private static String checkDay(Vehicle vehicle){
        int states=vehicle.getSpeedList().size();
        int day;
        String str;
        if(states<=84){
            int time=states*5;
            int hour=10+(int)(time/60);
            int min= time-(hour-10)*60;
            str="Day1 "+hour+":"+min+"~"+(min+5);
            return str;
        }
        else if(states<=372 && states>=265){day=2;states-=265;}
        else if(states<=660 && states>=553){day=3;states-=553;}
        else if(states<=948 && states>=841){day=4;states-=841;}
        else if(states<=1236 && states>=1129){day=5;states-=1129;}
        else if(states<=1524 && states>=1417){day=6;states-=1417;}
        else{str="TAKES MORE THAN 6 DAYS";   return str;}

        int time=states*5;
        int hour=8+(int)(time/60);
        int min= time-(hour-8)*60;
        str="Day"+day+" "+hour+":"+min+"~"+hour+":"+(min+5);
        return str;
    }

    private static String checkDay(int states){
        int day;
        String str;
        if(states<=84){
            int time=states*5;
            int hour=10+(int)(time/60);
            int min= time-(hour-10)*60;
            str="Day1 "+hour+":"+min+"~"+(min+5);
            return str;
        }
        else if(states<=372 && states>=265){day=2;states-=265;}
        else if(states<=660 && states>=553){day=3;states-=553;}
        else if(states<=948 && states>=841){day=4;states-=841;}
        else if(states<=1236 && states>=1129){day=5;states-=1129;}
        else if(states<=1524 && states>=1417){day=6;states-=1417;}
        else{str="TAKES MORE THAN 6 DAYS";   return str;}

        int time=states*5;
        int hour=8+(int)(time/60);
        int min= time-(hour-8)*60;
        str="Day"+day+" "+hour+":"+min+"~"+hour+":"+(min+5);
        return str;
    }

}