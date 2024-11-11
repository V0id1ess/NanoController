#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// NRF24L01
void receive();
void transmit(int LSX, int LSY, int RSX, int RSY, int ledPot, int bat, int mode, int startGPS);
void NRF_init();

// OLED Display
void oled_init();
void printOLED();

// Battery Monitor
int batteryCharge();

// Buzzer
void buzz(int value, int interval, int t);

// Potentiometer
int readPot();

// Variables
extern int Mode;
extern unsigned int connLoss;
extern float RollAngle, PitchAngle, MPU_Temp, Alt, LogicV, PowerV;
extern double Lat, Lon;
extern bool Signal_Strength, transmitted, received;
extern int startGPS;

#endif