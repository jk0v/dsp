#pragma once
#ifndef CONF_H
#define CONF_H

// Audio
#define SAMPLERATE 44100
#define BIT_DEPTH 24
#define AUDIO_BLOCK_SAMPLES 128

// modules
#define MAX_MODULE_IO 16
#define MAX_MODULES 16
#define MAX_CONNECTIONS MAX_MODULES*4
// #define MODULE_LOOP_DUR 5805
#define MODULE_LOOP_DUR abs((((float)AUDIO_BLOCK_SAMPLES/(float)SAMPLERATE)*1000000.f)) // 1000000us

// misc
#define MAX_FILELENGTH 8192
#define NN_HIDDEN_SIZE 15
#define F24_NORM_MAX (float) (0x7FFFFF) 
#define F24_NORM_MAX_INV 0.0000001192093037616359652f
#define ARDUINOJSON_USE_DOUBLE 0
#define JSON_MAX_MEM 300000

// i2c
#define I2C_ADDRESS 0x20

// UART
#define UART_BAUD 115200


// i2s
#define ADDA_I2S_INTERFACE 2
#define I2S_TDM_AUDIO_CHANNEL 8

// pins
// ADC: I2S1
// #define AD_MCLK_PIN 23
// #define AD_BCLK_PIN 21
// #define AD_LRCLK_PIN 20
// #define AD_DATA_PIN 7
// #define AD_MCLK_PIN_CONF CORE_PIN23_CONFIG
// #define AD_BCLK_PIN_CONF CORE_PIN21_CONFIG
// #define AD_LRCLK_PIN_CONF CORE_PIN20_CONFIG
// #define AD_DATA_PIN_CONF CORE_PIN7_CONFIG

#define ADDA_MCLK_PIN 33
#define ADDA_BCLK_PIN 4
#define ADDA_LRCLK_PIN 3
#define ADDA_MCLK_PIN_CONF CORE_PIN33_CONFIG
#define ADDA_BCLK_PIN_CONF CORE_PIN4_CONFIG
#define ADDA_LRCLK_PIN_CONF CORE_PIN3_CONFIG

#define AD_DATA_PIN 5
#define AD_DATA_PIN_CONF CORE_PIN5_CONFIG
#define DA_DATA_PIN 2
#define DA_DATA_PIN_CONF CORE_PIN2_CONFIG

// #define AD_MCLK_PIN 33
// #define AD_BCLK_PIN 4
// #define AD_LRCLK_PIN 3
// #define AD_DATA_PIN 8
// #define AD_MCLK_PIN_CONF CORE_PIN33_CONFIG
// #define AD_BCLK_PIN_CONF CORE_PIN4_CONFIG
// #define AD_LRCLK_PIN_CONF CORE_PIN3_CONFIG
// #define AD_DATA_PIN_CONF CORE_PIN8_CONFIG
// #define DA_MCLK_PIN 23
// #define DA_BCLK_PIN 21
// #define DA_LRCLK_PIN 20
// #define DA_DATA_PIN 7
// #define DA_MCLK_PIN_CONF CORE_PIN23_CONFIG
// #define DA_BCLK_PIN_CONF CORE_PIN21_CONFIG
// #define DA_LRCLK_PIN_CONF CORE_PIN20_CONFIG
// #define DA_DATA_PIN_CONF CORE_PIN7_CONFIG

// debug
// #define AD_MCLK_PIN 23
// #define AD_BCLK_PIN 21
// #define AD_LRCLK_PIN 20
// #define AD_DATA_PIN 7
// #define AD_MCLK_PIN_CONF CORE_PIN23_CONFIG
// #define AD_BCLK_PIN_CONF CORE_PIN21_CONFIG
// #define AD_LRCLK_PIN_CONF CORE_PIN20_CONFIG
// #define AD_DATA_PIN_CONF CORE_PIN7_CONFIG

// DAC: I2S2
// #define DA_MCLK_PIN 33
// #define DA_BCLK_PIN 4
// #define DA_LRCLK_PIN 3
// #define DA_DATA_PIN 8
// #define DA_MCLK_PIN_CONF CORE_PIN33_CONFIG
// #define DA_BCLK_PIN_CONF CORE_PIN4_CONFIG
// #define DA_LRCLK_PIN_CONF CORE_PIN3_CONFIG
// #define DA_DATA_PIN_CONF CORE_PIN8_CONFIG


#define SPI_MOSI_PIN PIN_SPI_MOSI
#define SPI_MISO_PIN PIN_SPI_MISO
#define SPI_SCLK_PIN PIN_SPI_SCK /*27*/ // SCK1
#define DA_CS_PIN 28 // active low
#define AD_CS_PIN 29 // active low
#define PGA_CS_PIN 30 // active low

// misc pins
#define ADDA_RST_PIN 31 // active low
#define STATUS_PIN LED_BUILTIN

#endif