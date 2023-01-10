#pragma once
#include <SD.h>
#include "util.h"
#include "audioModule.hpp"

namespace Audio
{
    namespace Modules
    {
        typedef struct Connection
        {
            void init(Module* src, Module* dest, int16_t srcIndex, int16_t destIndex)
            {
                this->src = src;
                this->dest = dest;
                this->srcIndex = srcIndex;
                this->destIndex = destIndex;
            }
            void destr()
            {
                this->src = nullptr;
                this->dest = nullptr;
                this->srcIndex = MAX_MODULES+1;
                this->destIndex = MAX_MODULES+1;
            }

            Module* src;
            Module* dest;
            int16_t srcIndex, destIndex;
        } Connection;

        class ModuleChain
        {
            public:
            ModuleChain();
            ModuleChain(ModuleType* preset);
            ~ModuleChain(){}

            void addModule(Module* mod);
            void addConnection(Module* src, Module* dest);

            void removeModule(Module* mod);
            void removeConnection(Connection* conn);

            void loadModulesFromFile(char* filePath = "default.ch");
            

            private:
            Module* modules[MAX_MODULES];
            Connection connections[MAX_MODULES];
            int16_t modIndex, connIndex;
        };
        
        // void deserializeModFile(char* filePath);
        // void loadModule(ModuleType type);
    }
}