#include "outputModule.hpp"
#include "mixModule.hpp"
#include <arm_math.h>
// #include "util.h"

extern Audio::Modules::OutputI2S outI2S;
extern Audio::Modules::InputI2S inI2S;
extern Audio::Modules::MixModule mixer;

namespace Audio
{
    namespace Modules
    {
        void audioCallback(int32_t** in, int32_t** out)
        {
            inI2S.outputBuffers[0].setFromMono(in, 1);
            modUpdateCallback();
            outI2S.inputBuffers[0]->cpyTo(out);
        }


        void OutputI2S::init()
        {
            i2sAudioCallback = audioCallback;
            i2sOut.begin();

            type = ModuleType::OUT_I2S;
            strcpy(name, "OutputI2S");
        }

        void OutputI2S::update()
        {
        }

        void OutputI2S::disableI2S()
        {
            NVIC_DISABLE_IRQ(IRQ_DMA_CH0);
            NVIC_DISABLE_IRQ(IRQ_DMA_CH1);
        }
        void OutputI2S::enableI2S()
        {
            NVIC_ENABLE_IRQ(IRQ_DMA_CH0);
            NVIC_ENABLE_IRQ(IRQ_DMA_CH1);
        }
    }
}