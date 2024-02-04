import java.util.ArrayList;

public class ControlStop {
    public String name;
    public String dataPath;
    static int num=0;

    public double azi_loc;
    public double latitude;
    public double longitude;
    public int distanceTilNext;  // current~next CS
    // 6378.137* Math.acos(Math.sin(y1)*Math.sin(y2) + Math.cos(y1)*Math.cos(y2)*Math.cos(x2-x1))
    public ArrayList<Double> temp= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> ghi= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> dhi= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> dni= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> zenith= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> azimuth= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> ghiData= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> windDir= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty
    public ArrayList<Double> windSpeed= new ArrayList<>();    // Index num equals stateNum; Index0 will be kept empty

    ControlStop(String name){
        if(name.equals("Darwin")){this.latitude=-12.4637;   this.longitude=130.8444;    this.distanceTilNext=269;}
        else if(name.equals("Katherine")){this.latitude=-14.4520;   this.longitude=132.2699;    this.distanceTilNext=232;}
        else if(name.equals("DalyWaters")){this.latitude=-16.2533;  this.longitude=133.3692;    this.distanceTilNext=387;}
        else if(name.equals("TennantCreek")){this.latitude=-19.6459;    this.longitude=134.1910;    this.distanceTilNext=194;}
        else if(name.equals("BarrowCreek")){this.latitude=-21.3799; this.longitude=133.9756;    this.distanceTilNext=258;}
        else if(name.equals("AliceSprings")){this.latitude=-23.6980;    this.longitude=133.8807;    this.distanceTilNext=246;}
        else if(name.equals("Kulgera")){this.latitude=-25.8436; this.longitude=133.2879; this.distanceTilNext=381;}
        else if(name.equals("CooberPedy")){this.latitude=-29.0135; this.longitude=134.7544; this.distanceTilNext=237;}
        else if(name.equals("Glendambo")){this.latitude=-30.9677; this.longitude=135.7513; this.distanceTilNext=256;}
        else if(name.equals("PortAugusta")){this.latitude=-32.4936; this.longitude=137.7743; this.distanceTilNext=281;}
        else if(name.equals("Adelaide")){this.latitude=-34.9285; this.longitude=138.6007; this.distanceTilNext=0;}
        else{return;}
        this.name= name;
        this.dataPath= String.format("data/solcast/%02d_%s.csv",++num,this.name);
    }

}
