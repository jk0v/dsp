#include "outputModule.hpp"
#include <arm_math.h>
// #include "util.h"

extern Audio::Modules::OutputI2S outI2S;
extern Audio::Modules::InputI2S inI2S;

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

            // inI2S.data.setFrom(in);
            inI2S.outputBuffers[0].setFromMono(in);
            // outI2S.data.cpyTo(out);
            outI2S.inputBuffers[0]->cpyToMono(out);

            // for(auto i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     out[0][i] = in[0][i]<<6;
            //     out[1][i] = in[0][i]<<6;
            // }
        }


        void OutputI2S::init()
        {
            i2sAudioCallback = audioCallback;
            i2sOut.begin();

            type = ModuleType::OUT_I2S;
        }

        UpdateStatus OutputI2S::update()
        {
            // TODO: module chain keeps track of how many mods
            
            // maybe later
            // for(auto i = 0; i < MAX_MODULE_IO; i++)
            // {
            //     data.add(&inputBuffers[i], MAX_MODULE_IO);
            // }

            if(status == UpdateStatus::FINISHED)
            {
                return UpdateStatus::FINISHED;
            }
            data.setFrom(inputBuffers[0]);

            status = UpdateStatus::FINISHED;
            return status;
        }
    }
}