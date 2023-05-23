#include "moduleChain.hpp"
#include "output_i2s.h"
#include "NNModule.hpp"
#include "outputModule.hpp"
#include "inputModule.hpp"
#include <ArduinoJson.h>
#include <SD.h>

extern Audio::Modules::ModuleChain modChain;
extern Audio::Modules::InputI2S inI2S; // very hideous
extern Audio::Modules::OutputI2S outI2S;
extern Audio::Modules::NNModule nnMod;

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

        void ModuleChain::reset()
        {
            for(int i=0; i<modIndex; ++i)
            {
                removeModule(modules[i]);
            }
            for(int i=0; i<connIndex; ++i)
            {
                removeConnection(&connections[i]);
            }
        }
        Module* ModuleChain::getStaticModuleFromType(ModuleType type)
        {
            switch(type)
            {
                case IN_I2S:
                    return &inI2S;
                break;
                case OUT_I2S:
                    return &outI2S;
                break;
                case NN_MOD:
                    return &nnMod;
                break;
                default:
                    return nullptr;
                break;
            }
        }

        void ModuleChain::loadFromFile(const char* name)
        {
            //Serial.println("1");
            snprintf(path, sizeof(path), "%s%s.json", MODCHAINS_PATH, name);
            File file = SD.open(path);
            //Serial.println("2");
            if (file)
            {
                //Serial.println("3");
                // set name
                strncpy(this->name, name, IO_MAX_STR_LEN);

                // load json file
                DynamicJsonDocument conf(JSON_ARRAY_SIZE(MAX_MODULES*MAX_MODULE_ATTR));
                //Serial.println("4");
                DeserializationError err = deserializeJson(conf, file);
                //Serial.println("5");
                if(err){/*Serial.printf("Deserialzation failed: %s", err.c_str());*/}

                // reset module chain
                // reset();
                //Serial.println("6");
                // load modules
                for(int i=0; i<MAX_MODULES; ++i)
                {
                    //Serial.println("7");
                    String key = String("mod")+String(i);
                    if(conf.containsKey(key))
                    {
                        //Serial.println("8");
                        addModule(getStaticModuleFromType((ModuleType)conf[key]["type"]));
                        //Serial.printf("9: %d\n", (int)conf[key]["type"]);
                        modules[i]->setConf(conf[key]);
                    } else {
                        break;
                    }
                }
                for(int i=0; i<MAX_CONNECTIONS; ++i)
                {
                    //Serial.println("10");
                    String key = String("conn")+String(i);
                    if(conf.containsKey(key))
                    {
                        //Serial.println("11");
                        addConnection(modules[(int)conf[key]["srcModIndex"]], (int)conf[key]["srcIndex"], modules[(int)conf[key]["destModIndex"]], (int)conf[key]["destIndex"]);
                        //Serial.printf("12: %d, %d, %d, %d\n", (int)conf[key]["srcModIndex"], (int)conf[key]["srcIndex"], (int)conf[key]["destModIndex"], (int)conf[key]["destIndex"]);
                    } else {
                        break;
                    }
                }
            }
        }
        void ModuleChain::saveToFile(const char* name)
        {
            snprintf(path, sizeof(path), "%s%s.json", MODCHAINS_PATH, name);
            if(SD.exists(path))
            {
                SD.remove(path);
            }

            File file = SD.open(path, FILE_WRITE);
            if(file)
            {
                DynamicJsonDocument conf(JSON_ARRAY_SIZE(MAX_MODULES*MAX_MODULE_ATTR));
                
                // load name
                conf["name"] = name;

                // load modules
                for(int i=0; i<modIndex; ++i)
                {
                    JsonObject mod = conf.createNestedObject(String("mod")+String(i));
                    modules[i]->getConf(mod);
                }

                // load connections
                for(int i=0; i<connIndex; ++i)
                {
                    JsonObject conn = conf.createNestedObject(String("conn")+String(i));

                    for(int j=0; j<modIndex; ++j)
                    {
                        if(connections[i].src == modules[j])
                        {
                            conn["srcModIndex"] = j;
                            conn["srcIndex"] = connections[i].srcIndex;
                        }
                        if(connections[i].dest == modules[j])
                        {
                            conn["destModIndex"] = j;
                            conn["destIndex"] = connections[i].destIndex;
                        }
                    }
                }

                // write to file
                serializeJsonPretty(conf, file);
                // serializeJsonPretty(conf, Serial);
                conf.clear();
            }
            file.close();
        }

        ModuleChain::ModuleChain()
        {
            modIndex = 0;
            connIndex = 0;
            strcpy(name, "ModChain1");

            modUpdateCallback = updateCallback;
        }
        ModuleChain::ModuleChain(ModuleType* preset)
        {
            modIndex = 0;
            connIndex = 0;
        }
    }
}