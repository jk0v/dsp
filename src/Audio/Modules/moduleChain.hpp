#pragma once
#include <SD.h>
#include "util.h"
#include "audioModule.hpp"

namespace Audio
{
    namespace Modules
    {
        class ModuleChain
        {
            public:
            ModuleChain();
            ~ModuleChain();

            uint8_t loadModulesFromFile(char* filePath = "default.stp");

            private:
            Module* modules;

        };
        
        void deserializeModFile(char* filePath);
        void loadModule(ModuleType type);
    }
}