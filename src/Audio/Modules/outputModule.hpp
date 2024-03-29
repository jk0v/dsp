#pragma once
#ifndef OUTPUTMODULE_HPP
#define OUTPUTMODULE_HPP
#include "output_i2s.h"
#include "audioModule.hpp"
#include "inputModule.hpp"

namespace Audio
{
    namespace Modules
    {
        void audioCallback(int32_t** in, int32_t** out);

        class OutputI2S : public Module
        {
            public:
            OutputI2S() {}
            ~OutputI2S() {}

            void init() override;
            
            friend void audioCallback(int32_t** in, int32_t** out);

            void update() override;

            void disableI2S();
            void enableI2S();

            Block output;

            private:
            AudioOutputI2S i2sOut;
        };
    }
}
#endif