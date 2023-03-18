#include "outputModule.hpp"
#include "mixModule.hpp"
#include <arm_math.h>
// #include "util.h"

extern Audio::Modules::OutputI2S outI2S;
extern Audio::Modules::InputI2S inI2S;
extern Audio::Modules::MixModule mixer;
extern int acc;

namespace Audio
{
    namespace Modules
    {
        void audioCallback(int32_t** in, int32_t** out)
        {
            // static uint16_t t = 0;
            // for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     // int32_t sig = (int32_t)arm_sin_f32(t * 0.01f * 2.0f * M_PI) * 1000000000.0f;
            //     // out[0][i] = in[0][i] + sig;
            //     // out[1][i] = in[1][i] + sig;
            //     out[0][i] = in[0][i];
            //     out[1][i] = in[0][i];

            //     t++;
            //     if(t>=100) t=0;
            // }

            // for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     InputI2S::input.data[0][i] = in[0][i];
            //     InputI2S::input.data[1][i] = in[1][i];

            //     out[0][i] = OutputI2S::output.data[0][i];
            //     out[1][i] = OutputI2S::output.data[1][i];
            // }

            // Block inB, outB;
            // inB.setFrom(in);
            // outB.cpyTo(out);
            // memcpy(out, in, sizeof(int32_t)*AUDIO_BLOCK_SAMPLES*2);

            // inI2S.input.setFromMono(in);
                inI2S.outputBuffers[0].setFromMono(in);
                // mixer.update();
            modUpdateCallback();
            // outI2S.output.cpyToMono(out);
                outI2S.inputBuffers[0]->cpyToMono(out);
            // NVIC_SET_PENDING(IRQ_SOFTWARE);
            // digitalToggleFast(35);

            // for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     out[0][i] = in[0][i]<<6;
            //     out[1][i] = in[0][i]<<6;
            // }
            // for (size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     int sig = (int)(arm_sin_f32(acc * 0.01f * 2.0f * M_PI) * 1000000000.0f);
            //     out[0][i] = in[0][i] + sig;
            //     out[1][i] = in[0][i] + sig;
            //     acc++;
            //     if (acc >= 100)
            //     acc -= 100;
            // }
        }


        void OutputI2S::init()
        {
            i2sAudioCallback = audioCallback;
            i2sOut.begin();

            type = ModuleType::OUT_I2S;
        }

        void OutputI2S::update()
        {
            // TODO: module chain keeps track of how many mods
            
            // maybe later
            // for(auto i = 0; i < MAX_MODULE_IO; i++)
            // {
            //     data.add(&inputBuffers[i], MAX_MODULE_IO);
            // }
            // digitalToggleFast(35);

            // data.setFrom(inputBuffers[0]);
            // digitalToggleFast(35);
            // output.setFrom(inputBuffers[0]);

            // status = UpdateStatus::UNFINISHED;
        }
    }
}