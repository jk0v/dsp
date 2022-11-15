#pragma once
#include "input_i2s.h"
#include "audioModule.hpp"
#include "outputModule.hpp"

namespace Audio::Modules
{
    class InputI2S : protected Module
    {
        public:
        InputI2S();
        ~InputI2S();
        
        friend void audioCallback(int32_t** in, int32_t** out);
        
        protected:
        static Block input;

        private:
        AudioInputI2S i2sIn;
    };
}