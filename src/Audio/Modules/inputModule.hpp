#pragma once
#ifndef INPUTMODULE_HPP
#define INPUTMODULE_HPP
#include "input_i2s.h"
#include "audioModule.hpp"
#include "outputModule.hpp"

extern void loop();
namespace Audio
{
    namespace Modules
    {
        class InputI2S : public Module
        {
            public:
            InputI2S(){}
            ~InputI2S() {}

            void init() override;
            
            friend void audioCallback(int32_t** in, int32_t** out);
            
            void update() override;
            
            protected:
            Block input;


            private:
            AudioInputI2S i2sIn;
        };
    }
}
#endif