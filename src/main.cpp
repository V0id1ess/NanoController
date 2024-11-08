#include <modules/joystick.h>
#include <modules/switch.h>
#include <global.h>
#include <Arduino.h>

Joystick JL(16, 17, 3);
Joystick JR(14, 15, 2, 1);

Switch RTH(7);
Switch Kill(4);

int startGPS = 0;

void mode_init(){
    while (Mode == -1){
        JL.read();
        JR.read();
        if (JL.x < 5 && JR.x > 1018){
          Mode = 0;
        } else if (JL.x > 1018 && JR.x < 5){
          Mode = 1;
        } else if (JL.y > 1018 && JR.y > 1018){
          startGPS = 1;
        } else if (JL.y < 5 && JR.y < 5){
          startGPS = 0;
        }
        printOLED();
        delay(10);
    }
    buzz(2, 60, 1800);
}

void setup(){
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  Mode = -1;
  NRF_init();
  oled_init();
  mode_init();
}

void loop(){
  recieve();

  printOLED();

  JL.read(); JR.read();
  RTH.read(); Kill.read();
  if (Kill.prevState == 0 && Kill.state == 1){
    Mode = 4;
    buzz(5, 30, 600);
  } else if (RTH.prevState == 0 && RTH.state == 1 || batteryCharge() < 7.5){
    Mode = 3;
    buzz(3, 40, 1600);
  } else if (JL.isPressed()){
    Mode = 2;
    buzz(2, 50, 1600);
  } else if (JR.isPressed()){
    if (Mode == 0){
      Mode = 1;
    } else {
      Mode = 0;
    }
    buzz(1, 60, 1800);
  }

  delay(10);

  transmit(JL.x, JL.y, JR.x, JR.y, readPot(), batteryCharge(), Mode, startGPS);
}
