#include <SPI.h>
#include <RF24.h>
#include <global.h>
#include <Arduino.h>

#define NRF_CE 9
#define NRF_CSN 10

RF24 radio(NRF_CE, NRF_CSN);

const byte addressA[6] = "y2024";
const byte addressB[6] = "z2024";

int Mode = 0;
unsigned int connLoss = 0;
float RollAngle, PitchAngle, MPU_Temp, Alt, LogicV, PowerV;
double Lat, Lon;
bool Signal_Strength = true, transmitted = false, received = false;

// 7 Bytes per packet
typedef struct {
    unsigned int mode : 3;
    unsigned int lsx : 8;
    unsigned int lsy : 8;
    unsigned int rsx : 8;
    unsigned int rsy : 8;
    unsigned int led : 5;
    unsigned int bat : 4;
    unsigned int gps : 1;
} dataToSend;

typedef struct {
    unsigned int mode : 3;
    unsigned int mpu_t : 7;
    unsigned int alt : 10;
    unsigned int lbat : 4;
    unsigned int pbat : 4;
    double lat : 27;
    double lon : 27;
} dataToReceive;

void NRF_init(){
    radio.begin();
    radio.openWritingPipe(addressB);
    radio.openReadingPipe(1, addressA);
    radio.setPALevel(RF24_PA_HIGH);
}

void transmit(int LSX, int LSY, int RSX, int RSY, int ledPot, int bat, int mode, int startGPS){
    radio.stopListening();
    dataToSend data;

    // Integers
    data.mode = Mode;
    data.lsx = (int)(LSX/4);
    data.lsy = (int)(LSY/4);
    data.rsx = (int)(RSX/4);
    data.rsy = (int)(RSY/4);
    data.led = (int)(ledPot/32);
    data.bat = (int)(LogicV/10);
    data.gps = startGPS;


    byte mixedArray[sizeof(data)];
    memcpy(mixedArray, &data, sizeof(data));

    transmitted = false;
    for (int i = 0; i < 25 && !transmitted; i++){
        transmitted = radio.write(&mixedArray, sizeof(mixedArray));
    }
    
    if (transmitted) {
        connLoss = 0;
    } else {
        connLoss += 1;
    }
}

void receive(){
    Signal_Strength = radio.testRPD();
    int DArray[9];

    received = false;

    radio.startListening();

    for (int i = 0; i < 10 && !received; i++){
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

    if (received) {
        connLoss = 0;
    } else {
        connLoss += 1;
    }
}