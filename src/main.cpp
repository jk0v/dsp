#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <arm_math.h>
#include "output_i2s.h"
#include "input_i2s.h"
#include "conf.h"
#include "util.h"
// #include "Audio/Modules/moduleChain.hpp"

AudioOutputI2S i2sOut;
AudioInputI2S i2sIn;
//Audio::Modules::ModuleChain mChain;

void init()
{
    // configure pins
    pinMode(AD_CS_PIN, OUTPUT);
    pinMode(DA_CS_PIN, OUTPUT);
    pinMode(PGA_CS_PIN, OUTPUT);

    pinMode(ADDA_RST_PIN, OUTPUT);
    pinMode(STATUS_PIN, OUTPUT);

    // put ADC and DAC in reset mode
    digitalWrite(ADDA_RST_PIN, 0);
    digitalWrite(STATUS_PIN, 0);
    
    // I2S init
    // i2sOut->begin();
    // i2sIn->begin();
    i2sOut.begin();
    i2sIn.begin();

    // SD init
    if(!SD.begin(BUILTIN_SDCARD))
    {
        throwError("SD initialization failed.", 0);
    }

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

void test(int32_t** in, int32_t** out)
{
    static uint16_t t = 0;
    for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
    {
        int32_t sig = (int32_t)arm_sin_f32(t * 0.01f * 200.0f * M_PI) * 1000000000.0f;
        out[0][i] = in[0][i] + sig;
        out[1][i] = in[1][i] + sig;

        t++;
        if(t>=100) t=0;
    }
}

void setup()
{
    i2sAudioCallback = test;

    init();
}

void loop()
{
}
