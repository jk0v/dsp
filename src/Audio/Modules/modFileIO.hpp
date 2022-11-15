#pragma once
#include <SD.h>
#include "util.h"
#include "audioModule.hpp"

namespace Audio::Modules
{
    void deserializeModFile(char* filePath)
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
        deserializeModFile(filePath, strncpy(strchr(filePath)))
        
    }

    void loadModule(ModuleType type)
    {

    }
}