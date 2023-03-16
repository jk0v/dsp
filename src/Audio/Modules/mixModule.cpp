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
            digitalToggleFast(35);
            for(int i=0; i<MAX_MODULE_IO; i++)
            {
                if(inputBuffers[i]->getUsed())
                {
                    outputBuffers[i].setFromWithGain(gains[i], inputBuffers[i]);
                }
            }
        }
    }
}