#pragma once
#ifndef MODULECHAIN_HPP
#define MODULECHAIN_HPP
#include <TeensyTimerTool.h>
#include "audioModule.hpp"

void updateCallback();

namespace Audio
{
    namespace Modules
    {
        

        typedef struct Connection
        {
            Module* src;
            Module* dest;
            int16_t srcIndex, destIndex;

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
        } Connection;
        class ModuleChain
        {
            public:
            ModuleChain();
            ModuleChain(ModuleType* preset);
            ~ModuleChain(){}

            void addModule(Module* mod);
            void addConnection(Module* src, int16_t srcIndex, Module* dest, int16_t destIndex);

            void removeModule(Module* mod);
            void removeConnection(Connection* conn);

            void loadChainFromFile(const char* filePath = "default.chn");
            
            friend void updateCallback();

            private:
            Module* modules[MAX_MODULES];
            Connection connections[MAX_CONNECTIONS];
            int16_t modIndex, connIndex;
            // TeensyTimerTool::PeriodicTimer updateClock;
            // IntervalTimer updateClock;

        };
        
        // void deserializeModFile(char* filePath);
        // void loadModule(ModuleType type);
    }
}
#endif