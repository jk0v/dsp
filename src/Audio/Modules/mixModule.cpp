#include "mixModule.hpp"

namespace Audio
{
    namespace Modules
    {
        MixModule::MixModule()
        {
            for(int i=0; i<MAX_MODULE_IO; i++)
            {
                gains[i] = 1.f;
            }
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; i++)
            {
                outputBuffers[0].data[0][i] = 0;
                outputBuffers[0].data[1][i] = 0;
            }
        }
        MixModule::MixModule(float gain)
        {
            for(int i=0; i<MAX_MODULE_IO; i++)
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
            digitalToggleFast(34);
            // __disable_irq();
            // for(int i=0; i<MAX_MODULE_IO; i++)
            // {
            //     // if(inputBuffers[i]->getUsed())
            //     // {
            //         outputBuffers[i].setFromWithGain(gains[i], inputBuffers[i]);
            //     // }
            // }
            // __disable_irq();
            outputBuffers[0].setFromWithGain(gains[0], inputBuffers[0]);
            // outputBuffers[0].setFrom(inputBuffers[0]);
            // __enable_irq();
        }
    }
}