#include <iostream>
#include <stdio.h>
#include "SerialClass.h"   // Library described above
#include <string>

// https://github.com/dmicha16/simple_serial_port

/*
int main(){

    printf("STARTING...\n\n");

    Serial* SP = new Serial("\\\\.\\COM4");    // adjust as needed
    if (SP->IsConnected()){printf("COM CONNECTED.\n\n");}

    char incomingData[256] = "";         // pre-allocate memory
    int dataLength = 255;
    int readResult = 0;

    while(SP->IsConnected()){

        readResult = SP->ReadData(incomingData,dataLength);
        // printf("Bytes read: (0 means no data available) %i\n",readResult);

        incomingData[readResult] = 0;

        if(readResult!=0) {
            printf("%s\n",incomingData);
        }

        // Sleep(500);// deleted for real time connection
    }

    return 0;
}
*/

int main(){

    printf("STARTING...\n\n");

    Serial* SP = new Serial("\\\\.\\COM4");    // adjust as needed
    if (SP->IsConnected()){printf("COM CONNECTED.\n\n");}

    char incomingData[256] = "";         // pre-allocate memory
    int dataLength = 255;
    int readResult = 0;

    while(SP->IsConnected()){

        readResult = SP->ReadData(incomingData,dataLength);
        // printf("Bytes read: (0 means no data available) %i\n",readResult);

        incomingData[readResult] = 0;

        if(readResult!=0) {

            string filename = generate_filename();


            ifstream file(filename);
            bool fileExists = file.good();

            ofstream outFile(filename, ios::app);
            if(outFile.is_open()){
                if(!fileExists){
                    outFile<<"[MPPT1 Vin],[MPPT1 Iin],[MPPT1 Vout],[MPPT1 Iout],[MPPT1 FET Temp],[MPPT1 Con Temp],[MPPT1 Error],[MPPT2 Vin],[MPPT2 Iin],[MPPT2 Vout],[MPPT2 Iout],[MPPT2 FET Temp],[MPPT2 Con Temp],[MPPT2 Error],[MPPT3 Vin],[MPPT3 Iin],[MPPT3 Vout],[MPPT3 Iout],[MPPT3 FET Temp],[MPPT3 Con Temp],[MPPT3 Error],[Battery V],[Battery I],[Battery SoC],[Battery Temp],[Battery Core Temp],[Battery Error]";
                }
                string data(incomingData);
                vector<string> dataVector;
                istringstream iss(data);
                string token;
                int count = 0;

                while(getline(iss,token,',')){  // CAUTION : "," causes an error. delimiter should be char, not string.
                    dataVector.emplace_back(token);
                    if(count<30){
                        outFile<<token<<",";
                    }
                }

                // 0[MPPT1 Vin],1[MPPT1 Iin],7[MPPT2 Vin],8[MPPT2 Iin],14[MPPT3 Vin],15[MPPT3 Iin],21[Battery V],22[Battery I],24[Battery Temp]
                cout << dataVector[0] << "," << dataVector[1] << "," << dataVector[7] << "," << dataVector[8] << "," << dataVector[14] << "," << dataVector[15] << "," << dataVector[21] << "," << dataVector[22] << "," << dataVector[24] << "\n";
            }
            outFile.close();

            // printf("%s\n",incomingData);
        }

        // Sleep(500);// deleted for real time connection
    }

    return 0;
}
