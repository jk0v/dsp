#pragma once
#include "input_i2s.h"
#include "audioModule.hpp"

namespace Audio::Modules
{
    class InputI2S : protected Module
    {
        public:
        InputI2S();
        ~InputI2S();

        private:
        AudioInputI2S i2sIn;
        AudioOutputI2S i2sOut;
    };
}