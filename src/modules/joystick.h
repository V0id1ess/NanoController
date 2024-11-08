#include <Arduino.h>

class Joystick{
    public:
        const int pinX, pinY, pinSW;
        const int mapping;
        bool prevState = false;
        bool currentState = false;
        int x;
        int y;

        Joystick(const int pinNumx, const int pinNumy, const int pinNumsw, const int map_val=0): pinX(pinNumx), pinY(pinNumy), pinSW(pinNumsw), mapping(map_val) {
            pinMode(pinX, INPUT);
            pinMode(pinY, INPUT);
            pinMode(pinSW, INPUT_PULLUP);
        }

    
        void read(){
            if (mapping){
                x = analogRead(pinX);
                y = map(analogRead(pinY), 0, 1023, 1023, 0);
            } else {
                x = map(analogRead(pinX), 0, 1023, 1023, 0);
                y = analogRead(pinY);
            }
        }

        bool isPressed(){
            currentState = digitalRead(pinSW);
            if (!currentState && prevState){
                prevState = currentState;
                return true;
            } else {
                prevState = currentState;
                return false;
            }
        }
};