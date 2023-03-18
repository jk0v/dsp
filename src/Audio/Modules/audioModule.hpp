#pragma once
#ifndef AUDIOMODULE_HPP
#define AUDIOMODULE_HPP
#include <Arduino.h>
#include "conf.h"

// TODO: think about order of propagation, audioSystem implementing ordered update, linearize signal chain(simplification)
//       determine order from output to input once and save hierarchy


namespace Audio
{
    typedef struct Block
    {
        int32_t data[2][AUDIO_BLOCK_SAMPLES];
        bool used;

        void cpyTo(Block* dest)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                dest->data[0][i] = data[0][i]; 
                dest->data[1][i] = data[1][i]; 
            }
        }
        void cpyTo(int32_t** dest)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                dest[0][i] = data[0][i]; 
                dest[1][i] = data[1][i]; 
            }
        }
        void cpyToMono(Block* dest)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                dest->data[0][i] = data[0][i];
                dest->data[1][i] = data[0][i];
            }
        }
        void cpyToMono(int32_t** dest)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                dest[0][i] = data[0][i];
                dest[1][i] = data[0][i];
            }
        }

        void setFrom(Block* src)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src->data[0][i]; 
                data[1][i] = src->data[1][i]; 
            }
        }
        void setFrom(int32_t** src)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src[0][i]; 
                data[1][i] = src[1][i]; 
            }
        }
        void setFromMono(Block* src)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src->data[0][i]; 
                data[1][i] = src->data[0][i]; 
            }
        }
        void setFromMono(int32_t** src)
        {
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src[0][i]; 
                data[1][i] = src[0][i]; 
            }
        }
        void setFromWithGain(float gain, Block* src)
        {
            if(gain==1.f)
            {
                setFrom(src);
                return;
            }
            if(gain==0.f)
            {
                memset(src->data, 0, AUDIO_BLOCK_SAMPLES*2);
                return;
            }
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src->data[0][i] * gain; 
                data[1][i] = src->data[1][i] * gain; 
            }
        }
        void setFromWithGain(float gain, int32_t** src)
        {
            if(gain==1.f)
            {
                setFrom(src);
                return;
            }
            if(gain==0.f)
            {
                memset(src, 0, AUDIO_BLOCK_SAMPLES*2);
                return;
            }
            for(auto i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                data[0][i] = src[0][i] * gain; 
                data[1][i] = src[1][i] * gain; 
            }
        }

        void setUsed(bool val)
        {
            used = val;
        }
        bool getUsed()
        {
            return used;
        }
        
        // // maybe unsafe
        // void cpyTo(Block* dest)
        // {
        //     memcpy(dest->data, &data, sizeof(data));
        // }
        // void cpyTo(int32_t** dest)
        // {
        //     memcpy(dest, &data, sizeof(data));
        // }
        
        // // maybe unsafe
        // void setFrom(Block* src)
        // {
        //     memcpy(&data, src->data, sizeof(data));
        // }
        // void setFrom(int32_t** src)
        // {
        //     memcpy(&data, src, sizeof(data));
        // }

        void add(Block* src, int32_t normalize = 1)
        {
            for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
            {

                this->data[0][i] += src->data[0][i] / normalize;
                this->data[1][i] += src->data[1][i] / normalize;
            }
        }
        void add(int32_t** src, int32_t normalize = 1)
        {
            for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; ++i)
            {
                this->data[0][i] += src[0][i] / normalize;
                this->data[1][i] += src[1][i] / normalize;

            }
        }
    } Block;

    namespace Modules
    {
        class ModuleChain;
        enum ModuleType
        {
            IN_I2S,
            OUT_I2S,
            MODULETYPE_LAST
        };
        enum UpdateStatus
        {
            FINISHED,
            UNFINISHED,
            PENDING,
            PASSING
        };
        class Module
        {
            public:
            Module() {}
            virtual ~Module() {}

            virtual void init() {};

            ModuleType getType(){return type;}
            UpdateStatus getStatus(){return status;}
            // int16_t getIndex(){return index;}
            
            virtual void update() {}

            friend class ModuleChain;

            protected:
            // Block data;

            Block* inputBuffers[MAX_MODULE_IO];
            Block outputBuffers[MAX_MODULE_IO];

            // Module* inputs[MAX_MODULE_IO];
            // Module* outputs[MAX_MODULE_IO];
            
            ModuleType type;
            UpdateStatus status;
            int16_t updateCount;
        };
    }
}
#endif