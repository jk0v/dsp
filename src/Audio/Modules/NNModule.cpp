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
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                inBuffer[i] = (float)inputBuffers[0]->data[0][i];
            }

            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                lstm.forward(&inBuffer[i]);
                fc.forward(lstm.outState);
            }
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                outputBuffers[0].data[0][i] = (float)fc.outState[i];
            }
        }
    }
}