#include "conf.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <arm_math.h>
#include <Wire.h>
#include "util.h"
#include "i2s_timers.h"
#include "Audio/Modules/outputModule.hpp"
#include "Audio/Modules/inputModule.hpp"
#include "Audio/Modules/moduleChain.hpp"
#include "Audio/Modules/mixModule.hpp"
#include "Audio/Modules/NNModule.hpp"

Audio::Modules::ModuleChain modChain;
Audio::Modules::InputI2S inI2S;
Audio::Modules::OutputI2S outI2S;
// Audio::Modules::MixModule mixer;
Audio::Modules::NNModule nnMod;


void init()
{
    Serial.begin(9600);
    while(!Serial) continue;
        
    // SD
    if(!SD.begin(BUILTIN_SDCARD))
    {
        // throwError("SD initialization failed.", 0);
        // Serial.println("SD Card init failed.");
    }

    // UART
    Serial1.begin(UART_BAUD, SERIAL_8N1);

    // configure pins
    pinMode(AD_CS_PIN, OUTPUT);
    pinMode(DA_CS_PIN, OUTPUT);
    pinMode(PGA_CS_PIN, OUTPUT);
    digitalWrite(AD_CS_PIN, 1);
    digitalWrite(DA_CS_PIN, 1);
    digitalWrite(PGA_CS_PIN, 1);


    pinMode(ADDA_RST_PIN, OUTPUT);

    // debug (callbacks)
    pinMode(35, OUTPUT);
    pinMode(34, OUTPUT);
    pinMode(37, OUTPUT);   


    // release ADDA reset
    digitalWrite(ADDA_RST_PIN, 1);
    delayMicroseconds(100);

    // SPI init
    SPI.setSCK(SPI_SCLK_PIN);
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.setMISO(SPI_MISO_PIN);
    SPI.begin();
    digitalWrite(AD_CS_PIN, 1);
    digitalWrite(DA_CS_PIN, 1);
    digitalWrite(PGA_CS_PIN, 1);

    // ADC, PGA config
    writeADCRegister(0x01, 0b11100001); // ADC config: 1:CP-EN = true, 000:MCLKDIV = /1, 01:DIF = I2S, 11:MODE = slave
    delayMicroseconds(100);

    // put ADC and DAC in reset mode
    digitalWrite(ADDA_RST_PIN, 0);

    // I2S init
    outI2S.init();
    inI2S.init();
    delayMicroseconds(100);

    // release ADDA reset
    digitalWrite(ADDA_RST_PIN, 1);
    delayMicroseconds(100);
}

void modChainTest()
{ 
    modChain.addModule(&inI2S);
    modChain.addModule(&outI2S);
    modChain.addModule(&nnMod);
    
    nnMod.loadWeights("/Amps/6505_med_gain_4266.json");

    modChain.addConnection(&inI2S, 0, &nnMod, 0);

    modChain.addConnection(&nnMod, 0, &outI2S, 0);
}

void setup()
{
    init();
    modChainTest();
}

void loop()
{
}
