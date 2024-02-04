import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;

public class Route {
    public double total_distance;
    ArrayList<ControlStop> stops= new ArrayList<>();
    Route(){
        stops.add(new ControlStop("Darwin"));
        stops.add(new ControlStop("Katherine"));
        stops.add(new ControlStop("DalyWaters"));
        stops.add(new ControlStop("TennantCreek"));
        stops.add(new ControlStop("BarrowCreek"));
        stops.add(new ControlStop("AliceSprings"));
        stops.add(new ControlStop("Kulgera"));
        stops.add(new ControlStop("CooberPedy"));
        stops.add(new ControlStop("Glendambo"));
        stops.add(new ControlStop("PortAugusta"));
        stops.add(new ControlStop("Adelaide"));

        for(ControlStop stop: stops){
            readData(stop);
            total_distance+=stop.distanceTilNext;
        }

        for(int i=0; i<10; i++){
            calculate_azi(this.stops.get(i), this.stops.get(i+1));  // FIXME 02-10
        }
    }

    private void readData(ControlStop stop){
        // int stateNum=0;
        File data= new File(stop.dataPath);
        try(Scanner scanner= new Scanner(data)){
            if(scanner.hasNextLine()){
                scanner.nextLine(); // index 0
                stop.temp.add(null);
                stop.ghi.add(null);
                stop.dhi.add(null);
                stop.dni.add(null);
                stop.zenith.add(null);
                stop.ghiData.add(null);
                stop.windDir.add(null);
                stop.windSpeed.add(null);
            }
            while(scanner.hasNextLine()){   // index 1~2100 // data time formats in UTC --> +9h30m: ACST
                String[] stateData= scanner.nextLine().split(",");
                stop.temp.add(Double.parseDouble(stateData[3]));
                stop.dhi.add(Double.parseDouble(stateData[7]));
                stop.dni.add(Double.parseDouble(stateData[8]));
                stop.ghi.add(Double.parseDouble(stateData[10]));
                stop.zenith.add(Double.parseDouble(stateData[19]));
                stop.azimuth.add(Double.parseDouble(stateData[4]));
                stop.ghiData.add(Double.parseDouble(stateData[10]));
                stop.windDir.add(Double.parseDouble(stateData[17]));
                stop.windSpeed.add(Double.parseDouble(stateData[18]));
            } scanner.close();
        } catch(FileNotFoundException e){}
    }

    /*  TODO an arraylist of hashmaps?
    5. cloud opacity
    */

    private void calculate_azi(ControlStop stop1, ControlStop stop2){
        double lat1= stop1.latitude;
        double long1= stop1.longitude;
        double lat2= stop2.latitude;
        double long2= stop2.longitude;
        // FIXME FIXME FIXME FIXME FIXME check for negative vals
        double azi_loc= Math.pow(Math.sin(Math.abs(lat1-lat2)/2),2)+Math.cos(lat1)*Math.cos(lat2)*Math.pow(Math.sin(Math.abs(long1-long2)/2),2);
        stop1.azi_loc= azi_loc;
    }



}
