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


void NRF_init(){
    radio.begin();
    radio.openWritingPipe(addressB);
    radio.openReadingPipe(1, addressA);
    radio.setPALevel(RF24_PA_HIGH);
    radio.setDataRate(RF24_2MBPS);
}

void transmit(int LSX, int LSY, int RSX, int RSY, int ledPot, int bat, int mode, int startGPS){
    radio.stopListening();

    // 7 Bytes per packet
    struct {
        unsigned int mode : 3;
        unsigned int lsx : 8;
        unsigned int lsy : 8;
        unsigned int rsx : 8;
        unsigned int rsy : 8;
        unsigned int led : 5;
        unsigned int bat : 4;
        unsigned int gps : 1;
    } dataToSend;

    // Integers
    dataToSend.mode = Mode;
    dataToSend.lsx = (int)(LSX/4);
    dataToSend.lsy = (int)(LSY/4);
    dataToSend.rsx = (int)(RSX/4);
    dataToSend.rsy = (int)(RSY/4);
    dataToSend.led = (int)(ledPot/32);
    dataToSend.bat = (int)(LogicV/10);
    dataToSend.gps = startGPS;

    transmitted = false;
    for (int i = 0; i < 25 && !transmitted; i++){
        transmitted = radio.write(&dataToSend, sizeof(dataToSend));
    }
    
    if (transmitted) {
        connLoss = 0;
    } else {
        connLoss += 1;
    }
}

void receive(){
    radio.startListening();

    // 11 Bytes per packet
    struct {
        unsigned int mode : 3;
        unsigned int mpu_t : 7;
        unsigned int alt : 10;
        unsigned int lbat : 4;
        unsigned int pbat : 4;
        int32_t lat : 30;
        int32_t lon : 30;
    } dataToReceive;

    received = false;

    for (int i = 0; i < 10 && !received; i++){
        if (radio.available()){
            radio.read(&dataToReceive, sizeof(dataToReceive));

            Mode =       dataToReceive.mode;
            MPU_Temp =   dataToReceive.mpu_t;
            Alt =        dataToReceive.alt;
            LogicV =     dataToReceive.lbat*10; 
            PowerV =     dataToReceive.pbat*10;
            Lat =        (double)(dataToReceive.lat/1000000);
            Lon =        (double)(dataToReceive.lon/1000000);

            received = true;

        }
    }

    if (received) {
        connLoss = 0;
    } else {
        connLoss += 1;
    }
}