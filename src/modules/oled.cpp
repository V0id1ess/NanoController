#include <global.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <string.h>

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);

void oled_init(){
    u8x8.begin();
    u8x8.setPowerSave(0);
}

void printOLED(){
    char arrayD[20];

    u8x8.setFont(u8x8_font_5x7_r);

    // for (int i = 0; i < 8; i++){
    //     u8x8.clearLine(i);
    // }
    // u8x8.drawString(0, 0, dtostrf(Mode, 0, 0, arrayD));
    // u8x8.drawString(0, 1, dtostrf(RollAngle, 0, 2, arrayD));
    // u8x8.drawString(0, 2, dtostrf(PitchAngle, 0, 2, arrayD));
    // u8x8.drawString(0, 3, dtostrf(MPU_Temp, 0, 2, arrayD));

    // Latitude
    u8x8.clearLine(0);
    u8x8.drawString(0, 0, "LA: ");
    u8x8.drawString(4, 0, dtostrf(Lat, 0, 6, arrayD));

    // Longitude
    u8x8.clearLine(1);
    u8x8.drawString(0, 1, "LO: ");
    u8x8.drawString(4, 1, dtostrf(Lon, 0, 6, arrayD));
    
    // Altitude
    u8x8.clearLine(2);
    u8x8.drawString(0, 2, "Alt: ");
    u8x8.drawString(5, 2, dtostrf(Alt, 5, 2, arrayD));

    // Batteries
    u8x8.clearLine(3);
    u8x8.clearLine(4);
    u8x8.drawString(0, 3, " D   L   C   RPD");
    u8x8.drawString(0, 4, dtostrf(PowerV, 0, 0, arrayD));
    u8x8.drawString(4, 4, dtostrf(LogicV, 0, 0, arrayD));
    u8x8.drawString(8, 4, dtostrf(batteryCharge(), 0, 0, arrayD));
    u8x8.display();

    // Signal Strength
    if (Signal_Strength){
        u8x8.drawString(12, 4, "Good");
    } else {
        u8x8.drawString(12, 4, "Bad");
    }

    // Mode
    u8x8.clearLine(6);
    if (Mode == 0){
        u8x8.drawString(0, 6, "Acro");
    } else if (Mode == 1){
        u8x8.drawString(0, 6, "Angle");
    } else if (Mode == 2){
        u8x8.drawString(0, 6, "Landing");
    } else if (Mode == 3){
        u8x8.drawString(0, 6, "Homing");
    } else if (Mode == 4){
        u8x8.drawString(0, 6, "Kill");
    } else {
        u8x8.drawString(0, 6, "Disabled");
    }

    // GPS_Status
    u8x8.drawString(9, 6, "PACK: ");
    u8x8.drawString(14, 6, dtostrf(connLoss, 0, 0, arrayD));

    // Temp
    u8x8.clearLine(7);
    u8x8.drawString(0, 7, "Temp: ");
    u8x8.drawString(6, 7, dtostrf(MPU_Temp, 0, 0, arrayD));
    
}
