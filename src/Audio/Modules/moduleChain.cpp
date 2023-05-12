#include "moduleChain.hpp"
#include "output_i2s.h"

extern Audio::Modules::ModuleChain modChain;

namespace Audio
{
    namespace Modules
    {
        void updateCallback()
        {
            for(int i=0; i<modChain.connIndex; ++i)
            {               
                modChain.connections[i].src->update();
            }
            asm("DSB");
        }

        void ModuleChain::addModule(Module* mod)
        {
            if(modIndex < MAX_MODULES)
            {
                modules[modIndex] = mod;
                modIndex++;
            } else
            {
                // throwError("Maximum amount of modules reached.", 0);
            }
        }
        void ModuleChain::addConnection(Module* src, int16_t srcIndex, Module* dest, int16_t destIndex)
        {
            if(srcIndex <= MAX_MODULE_IO && destIndex <= MAX_MODULE_IO)
            {
                dest->inputBuffers[destIndex] = &src->outputBuffers[srcIndex];
                connections[connIndex].init(src, dest, srcIndex, destIndex);

                dest->inputBuffers[destIndex]->setUsed(true);

                connIndex++;
            } else
            {
                // throwError("Maximum amount of connections reached.", 0);
            }
        }
        
        void ModuleChain::removeModule(Module* mod)
        {
            for(auto i=0; i<MAX_MODULES; ++i)
            {
                if(modules[i] == mod)
                {
                    modules[i] = nullptr;
                    delete(mod);
                    modIndex--;
                    return;
                }
            }
            // throwError("Could not remove module/Module not in moduleChain.", 0);
        }
        void ModuleChain::removeConnection(Connection* conn)
        {
            for(auto i=0; i<MAX_MODULES; ++i)
            {
                if(&connections[i] == conn)
                {
                    conn->dest->inputBuffers[conn->srcIndex]->setUsed(false);

                    conn->dest->inputBuffers[conn->srcIndex] = conn->dest->inputBuffers[conn->destIndex];
                    conn->destr();

                    connIndex--;
                    return;
                }
            }
            // throwError("Could not remove connection/Connection not in moduleChain.", 0);
        }

        void ModuleChain::loadChainFromFile(char* filePath)
        {
            
        }

        ModuleChain::ModuleChain()
        {
            modIndex = 0;
            connIndex = 0;

            modUpdateCallback = updateCallback;
        }
        ModuleChain::ModuleChain(ModuleType* preset)
        {
            modIndex = 0;
            connIndex = 0;
        }
    }
}