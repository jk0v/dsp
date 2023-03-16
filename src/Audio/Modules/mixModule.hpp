#pragma once
#ifndef MIX_MODULE_HPP
#define MIX_MODULE_HPP
#include "audioModule.hpp"

namespace Audio
{
    namespace Modules
    {
        class MixModule : public Module
        {
            public:
            MixModule();
            MixModule(float gain);
            ~MixModule(){};

            void setGain(int16_t index, float val);

            void update() override;

            private:
            float gains[MAX_MODULE_IO];
        };
    }
}

#endif