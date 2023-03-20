#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include "conf.h"

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
ESP32Encoder enc;
int gain = 89;
int pressed = 00;

void setup()
{
    pinMode(ENC_PIN_SW, INPUT_PULLUP);
    lcd.init();
    lcd.backlight();

    enc.attachHalfQuad(ENC_PIN_DT, ENC_PIN_CLK);
    enc.setCount(0);
}

void loop()
{
    lcd.clear();
    lcd.setCursor(0,1);
    delay(200);
}