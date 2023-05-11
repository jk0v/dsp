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
    // Serial.begin(9600);
    // while(!Serial) continue;
        
    // SD
    if(!SD.begin(BUILTIN_SDCARD))
    {
        // throwError("SD initialization failed.", 0);
        // Serial.println("SD Card init failed.");
    }

    // i2c
    // Wire.begin(I2C_ADDRESS);
    // Wire.onReceive(IO::i2cRecCallback);
    // Wire.onRequest(IO::i2cReqCallback);

    // UART
    // Serial1.begin(UART_BAUD, SERIAL_8N1);

    // configure pins
    pinMode(AD_CS_PIN, OUTPUT);
    pinMode(DA_CS_PIN, OUTPUT);
    pinMode(PGA_CS_PIN, OUTPUT);
    digitalWrite(AD_CS_PIN, 1);
    digitalWrite(DA_CS_PIN, 1);
    digitalWrite(PGA_CS_PIN, 1);


    pinMode(ADDA_RST_PIN, OUTPUT);
    // pinMode(STATUS_PIN, OUTPUT);

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
    // writeADCRegister(0x01, 0b11010001); // TDM: ADC config: 1:CP-EN = true, 000:MCLKDIV = /1, 10:DIF = TDM, 11:MODE = slave
    // writePGAGain((uint8_t)255, (uint8_t)255); // set initial channel gain (192 = 0dB)
    delayMicroseconds(100);

    // put ADC and DAC in reset mode
    digitalWrite(ADDA_RST_PIN, 0);
    // digitalWrite(STATUS_PIN, 1);

    // I2S init
    outI2S.init();
    inI2S.init();
    delayMicroseconds(100);

    // release ADDA reset
    digitalWrite(ADDA_RST_PIN, 1);
    delayMicroseconds(100);

    // digitalToggle(STATUS_PIN);
}

void modChainTest()
{
    modChain.addModule(&inI2S);
    modChain.addModule(&outI2S);
    // modChain.addModule(&mixer);
    modChain.addModule(&nnMod);
    
    nnMod.loadWeights("/Amps/6505_med_gain_4266.json");
    // float test = 0.5f;
    // nnMod.test(test);
    // test = 0.75f;
    // nnMod.test(test);
    // nnMod.update();
    // mixer.setGain(0, 25.f);
    

    modChain.addConnection(&inI2S, 0, &nnMod, 0);
    // modChain.addConnection(&inI2S, 0, &mixer, 0);

    modChain.addConnection(&nnMod, 0, &outI2S, 0);
    // modChain.addConnection(&mixer, 0, &outI2S, 0);
    
    // modChain.addConnection(&inI2S, 0, &outI2S, 0);
}

void setup()
{
    init();
    modChainTest();
}

float inc = 0.f;
float gain = 10.f;

void loop()
{
    // if(Serial1.available() > 0)
    // {
    //     gain = (float)Serial1.read();
    //     // Serial.println(gain);
    //     mixer.setGain(0, gain);
    // }
}
