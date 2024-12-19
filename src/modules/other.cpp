#include <Arduino.h>
#include <global.h>

// Battery Monitor
int batteryCharge(){
    int value = analogRead(21);
    // 0.82V voltage drop
    return map(value, 571, 736, 0, 100);
}

// Buzzer
void buzz(int times, int interval, int t){
    for (int x = 0; x < times; x++){
        tone(5, t);
        delay(interval);
        noTone(5);
        delay(interval);
    }
    return;
}

// Potentiometer
int readPot(){
    int value = analogRead(20);
    return value;
}