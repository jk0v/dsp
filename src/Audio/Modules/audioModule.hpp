#pragma once
#include <Arduino.h>
#include "conf.h"
#include "moduleChain.hpp"

// TODO: think about order of propagation, audioSystem implementing ordered update, linearize signal chain(simplification)
//       determine order from output to input once and save hierarchy

namespace Audio
{
    typedef struct Block
    {
        int32_t data[2][AUDIO_BLOCK_SAMPLES];

        // maybe unsafe
        void cpyTo(Block* dest)
        {
            memcpy(dest->data, this->data, sizeof(data));
        }
        void cpyTo(int32_t** dest)
        {
            memcpy(dest, this->data, sizeof(data));
        }
        
        // maybe unsafe
        void setFrom(Block* src)
        {
            memcpy(src->data, this->data, sizeof(data));
        }
        void setFrom(int32_t** src)
        {
            memcpy(src, this->data, sizeof(data));
        }

        void add(Block* src, int32_t normalize = 1)
        {
            for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            {
                src->data[0][i] /= normalize;
                src->data[1][i] /= normalize;

                this->data[0][i] += src->data[0][i];
                this->data[1][i] += src->data[1][i];
            }
        }
        void add(int32_t** src, int32_t normalize = 1)
        {
            for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            {
                src[0][i] /= normalize;
                src[1][i] /= normalize;

                this->data[0][i] += src[0][i];
                this->data[1][i] += src[1][i];

            }
        }
    } Block;

    namespace Modules
    {
        enum ModuleType
        {
            IN_I2S,
            OUT_I2S,
            MODULETYPE_LAST
        };
        class Module
        {
            public:
            Module() {}
            ~Module() {}

            virtual void init(void* args) {};

            ModuleType getType(){return type;}
            int16_t getIndex(){return index;}

            friend class ModuleChain;

            protected:
            Block data;

            Block* inputBuffers[MAX_MODULE_IO];
            Block outputBuffers[MAX_MODULE_IO];

            // Module* inputs[MAX_MODULE_IO];
            // Module* outputs[MAX_MODULE_IO];
            
            ModuleType type;

            int16_t index;

            virtual void update() {}
            virtual void publish() {}
        };
    }
}