#include <SPI.h>
#include <RF24.h>
#include <global.h>
#include <Arduino.h>

#define NRF_CE 9
#define NRF_CSN 10

RF24 radio(NRF_CE, NRF_CE);

const byte address[6] = "92024";


int Mode = 3;
float RollAngle, PitchAngle, MPU_Temp, Alt, LogicV, PowerV;
double Lat, Lon;
bool Signal_Strength = true, transmitted, received;


void NRF_init(){
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_HIGH);
    radio.stopListening();
}

struct DataArray{
    int integers[8];
};

void transmit(int LSX, int LSY, int RSX, int RSY, int ledPot, int bat, int mode, int startGPS){
    radio.stopListening();
    DataArray data;

    // Integers
    data.integers[0] = LSX;
    data.integers[1] = LSY;
    data.integers[2] = RSX;
    data.integers[3] = RSY;
    data.integers[4] = ledPot;
    data.integers[5] = bat;
    data.integers[6] = mode;
    data.integers[7] = startGPS;


    byte mixedArray[sizeof(DataArray)];
    memcpy(mixedArray, &data, sizeof(DataArray));

    transmitted = false;
    for (int i = 0; i < 15 && !transmitted; i++){
        transmitted = radio.write(&mixedArray, sizeof(mixedArray));
    }
}

void recieve(){
    Signal_Strength = radio.testRPD();
    Serial.println(Signal_Strength);
    int DArray[9];

    received = false;

    radio.startListening();

    for (int i = 0; i < 15 && !received; i++){
        if (radio.available()){
            radio.read(&DArray, sizeof(DArray));

            Mode =       DArray[0];
            RollAngle =  DArray[1];
            PitchAngle = DArray[2];
            MPU_Temp =   DArray[3];
            Alt =        DArray[4];
            LogicV =     DArray[5]; 
            PowerV =     DArray[6];
            Lat =        DArray[7];
            Lon =        DArray[8];

            received = true;

        }
    }
}