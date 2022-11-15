#pragma once
#include "output_i2s.h"
#include "audioModule.hpp"
#include "inputModule.hpp"

namespace Audio::Modules
{
    void audioCallback(int32_t** in, int32_t** out);

    class OutputI2S : protected Module
    {
        public:
        OutputI2S();
        ~OutputI2S();

        friend void audioCallback(int32_t** in, int32_t** out);

        protected:
        static Block output;
        float gains[MAX_MODULE_IO];

        void update();

        private:
        AudioOutputI2S i2sOut;
    };
}