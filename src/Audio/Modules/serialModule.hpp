#pragma once
#ifndef SERIALMODULE_HPP
#define SERIALMODULE_HPP
#include "audioModule.hpp"

namespace Audio
{
    namespace Modules
    {
        class SerialModule : public Module
        {
            public:
            SerialModule() {}
            ~SerialModule() {}

            void init() override;

            void update() override;
        };
    }
}

#endif