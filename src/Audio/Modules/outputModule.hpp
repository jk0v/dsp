#pragma once
#include "output_i2s.h"
#include "audioModule.hpp"
#include "inputModule.hpp"

namespace Audio
{
    namespace Modules
    {
        void audioCallback(int32_t** in, int32_t** out);

        class OutputI2S : protected Module
        {
            public:
            OutputI2S();
            ~OutputI2S() {}

            void init(void* args) override;
            
            friend void audioCallback(int32_t** in, int32_t** out);

            protected:
            Block output;
            float gains[MAX_MODULE_IO];

            void update() override;


            private:
            AudioOutputI2S i2sOut;
        };
    }
}