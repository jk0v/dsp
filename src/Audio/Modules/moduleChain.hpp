#pragma once
#ifndef MODULECHAIN_HPP
#define MODULECHAIN_HPP
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

            void loadFromFile(const char* name);
            void saveToFile(const char* name);

            void reset();
            Module* getStaticModuleFromType(ModuleType type); // temporary: better go to dynamic allocation
            
            friend void updateCallback();

            private:
            Module* modules[MAX_MODULES];
            Connection connections[MAX_CONNECTIONS];
            int16_t modIndex, connIndex;

            char name[IO_MAX_STR_LEN];
            char path[MAX_PATH_LEN];
            // TeensyTimerTool::PeriodicTimer updateClock;
            // IntervalTimer updateClock;

        };
        
        // void deserializeModFile(char* filePath);
        // void loadModule(ModuleType type);
    }
}
#endif