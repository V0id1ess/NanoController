#include <Arduino.h>

class Switch{
    public:
        const int switchPin;
        int state = 0;
        int prevState = 0;

        Switch(const int swPin): switchPin(swPin) {
            pinMode(switchPin, INPUT_PULLUP);
        }

        void read(){
            if (digitalRead(switchPin) == HIGH){
                state = 0;
            } else {
                state = 1;
            }
        }
};