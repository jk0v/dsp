#pragma once
#ifndef IO_HPP
#define IO_HPP
#include <Arduino.h>

namespace IO
{
    class IO
    {
        public:
        IO(){}
        ~IO(){}

        void init()
        {
            Serial1.setTX(ESP_UART_TX_PIN)
        }
    }
}

#endif