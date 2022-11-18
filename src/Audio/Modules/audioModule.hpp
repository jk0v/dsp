#pragma once
#include <Arduino.h>
#include "output_i2s.h"
#include "input_i2s.h"
#include "conf.h"

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

        void add(Block* src, bool normalize)
        {
            for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            {
                this->data[0][i] += src->data[0][i];
                this->data[1][i] += src->data[1][i];

                if(normalize)
                {
                    this->data[0][i] / 2;
                    this->data[1][i] / 2;
                }
            }
        }
        void add(int32_t** src, bool normalize)
        {
            for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            {
                this->data[0][i] += src[0][i];
                this->data[1][i] += src[1][i];

                if(normalize)
                {
                    this->data[0][i] / 2;
                    this->data[1][i] / 2;
                }
            }
        }
    } Block;
}

namespace Audio::Modules
{
    class Module
    {
        public:
        Module();
        ~Module();

        virtual uint8_t init(void* args);

        ModuleType getType(){return type;}

        // friend class AudioInputI2S;
        // friend class AudioOutputI2S;

        protected:
        Block data;

        Block inputBuffers[MAX_MODULE_IO];
        Block outputBuffers[MAX_MODULE_IO];

        Module* inputs[MAX_MODULE_IO];
        Module* outputs[MAX_MODULE_IO];
        
        ModuleType type;

        virtual void update();
        virtual uint8_t publish();
    };

    enum ModuleType
    {
        IN_I2S,
        OUT_I2S,
        MODULETYPE_LAST
    };
}