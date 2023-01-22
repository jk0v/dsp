#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_

#include <Arduino.h>
#include <cstdint>
#include <SPI.h>
#include "conf.h"


// writes data to a register of the ADC 
void writeADCRegister(uint8_t regAddr, uint8_t data)
{
    digitalWrite(PGA_CS_PIN, 1);
    digitalWrite(DA_CS_PIN, 1);
    digitalWrite(AD_CS_PIN, 0); // pull CS low to start communication

    SPI.transfer(0b0111001); // chip address in write mode
    SPI.transfer(regAddr); // send MAP address
    SPI.transfer(data); // send data

    digitalWrite(AD_CS_PIN, 1); // pull CS high for end of communication
}

// sets the gain of the left and right channel of the PGA; formula: gain[dB]=31.5-(0.5-(255-n))
void writePGAGain(uint8_t lGain, uint8_t rGain)
{
    uint16_t rlGain = (rGain<<8) | lGain; // combine left, right gain into 1 16bit variable

    digitalWrite(DA_CS_PIN, 1);
    digitalWrite(AD_CS_PIN, 1);
    digitalWrite(PGA_CS_PIN, 0); // pull CS low to start communication

    SPI.transfer16(rlGain);

    digitalWrite(PGA_CS_PIN, 1); // pull CS high for end of communication
}

// Throw error: msg: message to display, return code != 0: halts execution 
void throwError(const char* msg, uint8_t retCode)
{
    digitalWrite(STATUS_PIN, 1);

    #if defined(SERIAL_COMM)
    Serial.println(strcat("ERROR: ", msg));
    #endif
    if(retCode != 0)
    {
        while(1)
        {
            #if defined(SERIAL_COMM)
            Serial.println("Halting execution.");
            #endif
            digitalToggle(STATUS_PIN);
            delay(500);
        }
    }
    delay(500);
    digitalWrite(STATUS_PIN, 0);
}
#endif