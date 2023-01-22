#include "moduleChain.hpp"
// #include "util.h"

namespace Audio
{
    namespace Modules
    {
        void ModuleChain::addModule(Module* mod)
        {
            if(modIndex < MAX_MODULES)
            {
                modules[modIndex] = mod;
                modIndex++;
                digitalToggle(35);
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
                digitalToggle(35);
            } else
            {
                // throwError("Maximum amount of connections reached.", 0);
                digitalToggle(35);
            }
        }
        
        void ModuleChain::removeModule(Module* mod)
        {
            for(int i=0; i<MAX_MODULES; i++)
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
            for(int i=0; i<MAX_MODULES; i++)
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
        
        
        ModuleChain::ModuleChain()
        {
            modIndex = 0;
            connIndex = 0;
        }
        ModuleChain::ModuleChain(ModuleType* preset)
        {
            modIndex = 0;
            connIndex = 0;
        }
    }
}