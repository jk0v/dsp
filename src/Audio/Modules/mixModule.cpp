#include "mixModule.hpp"
#include <arm_math.h>

namespace Audio
{
    namespace Modules
    {
        MixModule::MixModule()
        {
            for(int i=0; i<MAX_MODULE_IO; ++i)
            {
                gains[i] = 1.f;
            }
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                outputBuffers[0].data[0][i] = 0;
                outputBuffers[0].data[1][i] = 0;
            }
        }
        MixModule::MixModule(float gain)
        {
            for(int i=0; i<MAX_MODULE_IO; ++i)
            {
                gains[i] = gain;
            }
        }

        void MixModule::setGain(int16_t index, float val)
        {
            if(index > MAX_MODULE_IO) return;
            gains[index] = val;
        }
        
        void MixModule::update()
        {
            // digitalToggleFast(34);
            // __disable_irq();
            // for(int i=0; i<MAX_MODULE_IO; ++i)
            // {
            //     // if(inputBuffers[i]->getUsed())
            //     // {
            //         outputBuffers[i].setFromWithGain(gains[i], inputBuffers[i]);
            //     // }
            // }
            // __disable_irq();

            outputBuffers[0].setFromWithGain(gains[0], inputBuffers[0]);

            // for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            // {
            //     float normL = (float)inputBuffers[0]->data[0][i] / F32_NORM_MAX;
            //     float normR = (float)inputBuffers[0]->data[1][i] / F32_NORM_MAX;
            //     outputBuffers[0].data[0][i] = (int32_t)(normL * F32_NORM_MAX * gains[0]);
            //     outputBuffers[0].data[1][i] = (int32_t)(normR * F32_NORM_MAX * gains[0]);
            // }

            // arm_scale_q31((q31_t*)inputBuffers[0]->data, (q31_t)25, 0, (q31_t *)inputBuffers[0]->data, AUDIO_BLOCK_SAMPLES*2);
            // arm_scale_f32((float*)inputBuffers[0]->data, gains[0], (float*)inputBuffers[0]->data, AUDIO_BLOCK_SAMPLES*2);
            // outputBuffers[0].setFrom(inputBuffers[0]);
            // __enable_irq();
        }
    }
}