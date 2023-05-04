#include "NNModule.hpp"

namespace Audio
{
    namespace Modules
    {
        NNModule::NNModule()
        {
        }

        void NNModule::update()
        {
            // digitalToggleFast(35);
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                // inBuffer[i] = (float)inputBuffers[0]->data[0][i]; // not normalized
                inBuffer[i] = (float)inputBuffers[0]->data[0][i] / F24_NORM_MAX; 
            }
            // digitalToggleFast(35);

            // digitalToggleFast(34);
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                // digitalToggleFast(34);
                lstm.forward(inBuffer[i]);
                // digitalToggleFast(34);
                // digitalWriteFast(34, 1);
                fc.forward(lstm.outState);
                // digitalWriteFast(34, 0);
                outputBuffers[0].data[0][i] = (float)fc.outState[0];
            }
            // digitalToggleFast(34);
        }
    }
}