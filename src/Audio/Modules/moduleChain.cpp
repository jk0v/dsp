#include "moduleChain.hpp"

namespace Audio::Modules
{
    void deserializeModFile(char* filePath, char* cachePath)
    {
        File modFile;
        char modBuffer[MAX_FILELENGTH];

        if(!SD.exists(filePath))
        {
            throwError("Module file not found.", 0);
            return;
        } else
        {
            modFile = SD.open(filePath, FILE_READ);
            if(modFile)
            {
                deserializeModFile(filePath, modBuffer);
            }
        }

        if(modFile)
        {
            modFile.readBytes(modBuffer, MAX_FILELENGTH);
            for(uint16_t i = 0; i < MAX_FILELENGTH; i++)
            {
                switch(modBuffer[i])
                {
                    case ModuleType::IN_I2S:

                }
            }
        }
    }

    void loadModulesFromFile(char* filePath = "default.mod")
    {
        char* cachePath = strncat(filePath, ".tmp", strlen(filePath)-4);
        deserializeModFile(filePath, );
        
    }

    void loadModule(ModuleType type)
    {

    }
}