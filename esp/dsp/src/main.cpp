#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Encoder.h>
#include <HardwareSerial.h>
#include "conf.h"

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
ESP32Encoder enc;
HardwareSerial serPort(2);

int gain = 89;
int pressed = 00;


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
}