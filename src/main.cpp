#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <arm_math.h>
#include <Wire.h>
#include "conf.h"
#include "util.h"
#include "Audio/Modules/outputModule.hpp"
#include "Audio/Modules/inputModule.hpp"
#include "Audio/Modules/moduleChain.hpp"
#include "Audio/Modules/mixModule.hpp"
#include "Audio/Modules/NNModule.hpp"
#include "i2s_timers.h"
// #include "IO/i2cComm.hpp"


Audio::Modules::ModuleChain modChain;
Audio::Modules::InputI2S inI2S;
Audio::Modules::OutputI2S outI2S;
// Audio::Modules::MixModule mixer;
Audio::Modules::NNModule nnMod;

int acc = 0;


void init()
{
    // Serial.begin(115200);
    
    // i2c
    // Wire.begin(I2C_ADDRESS);
    // Wire.onReceive(IO::i2cRecCallback);
    // Wire.onRequest(IO::i2cReqCallback);

    // configure pins
    pinMode(AD_CS_PIN, OUTPUT);
    pinMode(DA_CS_PIN, OUTPUT);
    pinMode(PGA_CS_PIN, OUTPUT);

    pinMode(ADDA_RST_PIN, OUTPUT);
    pinMode(STATUS_PIN, OUTPUT);

    // debug (callbacks)
    pinMode(35, OUTPUT);
    pinMode(34, OUTPUT);
    // digitalToggle(35);
    // digitalToggle(37);

    // put ADC and DAC in reset mode
    digitalWrite(ADDA_RST_PIN, 0);
    digitalWrite(STATUS_PIN, 1);
    
    // I2S init
    outI2S.init();
    inI2S.init();

    // serMod.init();

    // SD init
    // if(!SD.begin(BUILTIN_SDCARD))
    // {
    //     throwError("SD initialization failed.", 0);
    // }

    // SPI init
    SPI.begin();
    SPI.setMOSI(SPI_MOSI_PIN);
    SPI.setMISO(SPI_MISO_PIN);
    SPI.setSCK(SPI_SCLK_PIN);
    
    // release ADDA reset
    digitalWrite(ADDA_RST_PIN, 1);

    // ADC, PGA config
    writeADCRegister(0x01, 0b11100001); // ADC config: 1:CP-EN = true, 000:MCLKDIV = /1, 01:DIF = I2S, 11:MODE = slave
    writePGAGain((uint8_t)255, (uint8_t)255); // set initial channel gain (192 = 0dB)

    digitalToggle(STATUS_PIN);
}

void modChainTest()
{
    modChain.addModule(&inI2S);
    modChain.addModule(&outI2S);
    // modChain.addModule(&mixer);
    modChain.addModule(&nnMod);
    // mixer.setGain(0, 25.f);
    

    modChain.addConnection(&inI2S, 0, &nnMod, 0);
    // modChain.addConnection(&inI2S, 0, &mixer, 0);
    modChain.addConnection(&nnMod, 0, &outI2S, 0);
    
    // modChain.addConnection(&inI2S, 0, &outI2S, 0);
}

void setup()
{
    init();
    modChainTest();
}

void loop()
{
    
}
