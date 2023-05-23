#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "conf.h"
#include <ESP32Encoder.h>
#include <HardwareSerial.h>
#include "IO/IO.hpp"

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
ESP32Encoder enc;
HardwareSerial serPort(2);

const char* modNames[] = {
    "marshall_1390",
    "marshall_1152",
    "marshall_300",
    "6505_4266",
    "6505_600",
    "clean"
};
int numMod = 6;
int actMod = 0;

void setup()
{
    serPort.begin(UART_BAUD, SERIAL_8N1, 16, 17);
    pinMode(ENC_PIN_SW, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();

    enc.attachHalfQuad(ENC_PIN_DT, ENC_PIN_CLK);
    enc.setCount(0);
}

void loop()
{
    if(actMod != abs(enc.getCount())%numMod)
    {
        actMod = abs(enc.getCount())%numMod;

        serPort.write(actMod);
        // serPort.printf("%d,", gain);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Preset:");
        lcd.setCursor(0,1);
        lcd.printf("%s", modNames[actMod]);
    }
    delay(100);
}