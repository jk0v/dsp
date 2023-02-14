#include "moduleChain.hpp"
// #include "util.h"
extern Audio::Modules::ModuleChain modChain;

namespace Audio
{
    namespace Modules
    {
        void updateCallback()
        {
            for(auto i=0; i<=modChain.connIndex; i++)
            {
                if(modChain.connections[i].src->update() == FINISHED)
                {
                    modChain.connections[i].dest->update();
                }
            }
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
        void ModuleChain::addConnection(Module* src, Module* dest)
        {
            if(src->index <= MAX_MODULE_IO && dest->index <= MAX_MODULE_IO)
            {
                dest->inputBuffers[dest->index] = &src->outputBuffers[src->index];
                connections[connIndex].init(src, dest, src->index, dest->index);
            } else
            {
                // throwError("Maximum amount of connections reached.", 0);
            }
        }
        
        void ModuleChain::removeModule(Module* mod)
        {
            for(auto i=0; i<MAX_MODULES; i++)
            {
                if(modules[i] == mod)
                {
                    modules[i] = nullptr;
                    delete(mod);
                    return;
                }
            }
            // throwError("Could not remove module/Module not in moduleChain.", 0);
        }
        void ModuleChain::removeConnection(Connection* conn)
        {
            for(auto i=0; i<MAX_MODULES; i++)
            {
                if(&connections[i] == conn)
                {
                    conn->dest->inputBuffers[conn->srcIndex] = conn->dest->inputBuffers[conn->destIndex];
                    conn->destr();
                    return;
                }
            }
            // throwError("Could not remove connection/Connection not in moduleChain.", 0);
        }

        void ModuleChain::loadChainFromFile(const char* filePath = "default.chn")
        {
            
        }



        ModuleChain::ModuleChain()
        {
            modIndex = 0;
            connIndex = 0;

            updateClock.begin(updateCallback, MODULE_LOOP_DUR);
        }
        ModuleChain::ModuleChain(ModuleType* preset)
        {
            modIndex = 0;
            connIndex = 0;
        }
    }
}