#pragma once
#ifndef CONF_H
#define CONF_H

// Audio
#define SAMPLERATE 44100
#define BIT_DEPTH 24
#define AUDIO_BLOCK_SAMPLES 64

// misc
#define MAX_MODULE_IO 16
#define MAX_FILELENGTH 1024

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
#define AD_MCLK_PIN 33
#define AD_BCLK_PIN 4
#define AD_LRCLK_PIN 3
#define AD_DATA_PIN 7
#define AD_MCLK_PIN_CONF CORE_PIN33_CONFIG
#define AD_BCLK_PIN_CONF CORE_PIN4_CONFIG
#define AD_LRCLK_PIN_CONF CORE_PIN3_CONFIG
#define AD_DATA_PIN_CONF CORE_PIN5_CONFIG

// DAC: I2S2
// #define DA_MCLK_PIN 33
// #define DA_BCLK_PIN 4
// #define DA_LRCLK_PIN 3
// #define DA_DATA_PIN 8
// #define DA_MCLK_PIN_CONF CORE_PIN33_CONFIG
// #define DA_BCLK_PIN_CONF CORE_PIN4_CONFIG
// #define DA_LRCLK_PIN_CONF CORE_PIN3_CONFIG
// #define DA_DATA_PIN_CONF CORE_PIN8_CONFIG
//for debug
// #define DA_MCLK_PIN 23
// #define DA_BCLK_PIN 21
// #define DA_LRCLK_PIN 20
// #define DA_DATA_PIN 8

#define DA_MCLK_PIN 33
#define DA_BCLK_PIN 4
#define DA_LRCLK_PIN 3
#define DA_DATA_PIN 7

// #define DA_MCLK_PIN_CONF CORE_PIN23_CONFIG
// #define DA_BCLK_PIN_CONF CORE_PIN21_CONFIG
// #define DA_LRCLK_PIN_CONF CORE_PIN20_CONFIG
// #define DA_DATA_PIN_CONF CORE_PIN8_CONFIG

#define DA_MCLK_PIN_CONF CORE_PIN33_CONFIG
#define DA_BCLK_PIN_CONF CORE_PIN4_CONFIG
#define DA_LRCLK_PIN_CONF CORE_PIN3_CONFIG
#define DA_DATA_PIN_CONF CORE_PIN7_CONFIG

#define SPI_MOSI_PIN PIN_SPI_MOSI
#define SPI_MISO_PIN PIN_SPI_MISO
#define SPI_SCLK_PIN 27 // SCK1
#define DA_CS_PIN 28 // active low
#define AD_CS_PIN 29 // active low
#define PGA_CS_PIN 30 // active low

// misc pins
#define ADDA_RST_PIN 31 // active low
#define STATUS_PIN LED_BUILTIN

#endif