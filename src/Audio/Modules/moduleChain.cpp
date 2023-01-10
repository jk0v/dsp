#include "moduleChain.hpp"

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
            } else
            {
                throwError("Maximum amount of modules reached.", 0);
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
                throwError("Maximum amount of connections reached.", 0);
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
            throwError("Could not remove module/Module not in moduleChain.", 0);
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
            throwError("Could not remove connection/Connection not in moduleChain.", 0);
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



// #include "moduleChain.hpp"

// namespace Audio::Modules
// {
//     void deserializeModFile(char* filePath, char* cachePath)
//     {
//         File modFile;
//         char modBuffer[MAX_FILELENGTH];

//         if(!SD.exists(filePath))
//         {
//             throwError("Module file not found.", 0);
//             return;
//         } else
//         {
//             modFile = SD.open(filePath, FILE_READ);
//             if(modFile)
//             {
//                 deserializeModFile(filePath, modBuffer);
//             }
//         }

//         if(modFile)
//         {
//             modFile.readBytes(modBuffer, MAX_FILELENGTH);
//             for(uint16_t i = 0; i < MAX_FILELENGTH; i++)
//             {
//                 switch(modBuffer[i])
//                 {
//                     case ModuleType::IN_I2S:

//                 }
//             }
//         }
//     }

//     void loadModulesFromFile(char* filePath = "default.mod")
//     {
//         char* cachePath = strncat(filePath, ".tmp", strlen(filePath)-4);
//         deserializeModFile(filePath, );
        
//     }

//     void loadModule(ModuleType type)
//     {

//     }
// }