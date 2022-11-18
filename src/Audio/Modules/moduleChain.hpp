#pragma once
#include <SD.h>
#include "util.h"
#include "audioModule.hpp"

namespace Audio::Modules
{
    class ModuleChain
    {
        public:
        ModuleChain();
        ~ModuleChain();

        uint8_t loadModulesFromFile(char* filePath = "default.mod");

        private:
        Module* modules;

    };
    
    void deserializeModFile(char* filePath);
    void loadModule(ModuleType type);
}