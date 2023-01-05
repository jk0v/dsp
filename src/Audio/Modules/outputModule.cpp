#include "outputModule.hpp"
#include <arm_math.h>
// #include "util.h"

namespace Audio
{
    namespace Modules
    {
        void audioCallback(int32_t** in, int32_t** out)
        {
            static uint16_t t = 0;
            for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            {
                int32_t sig = (int32_t)arm_sin_f32(t * 0.01f * 2.0f * M_PI) * 1000000000.0f;
                out[0][i] = in[0][i] + sig;
                out[1][i] = in[1][i] + sig;

                t++;
                if(t>=100) t=0;
            }

            // for(size_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++)
            // {
            //     InputI2S::input.data[0][i] = in[0][i];
            //     InputI2S::input.data[1][i] = in[1][i];

            //     out[0][i] = OutputI2S::output.data[0][i];
            //     out[1][i] = OutputI2S::output.data[1][i];
            // }
            // // probably unsafe
            // memcpy(&InputI2S::input.data, &in, sizeof(&in));
            // memcpy(&out, &OutputI2S::output.data, sizeof(OutputI2S::output.data));

            // not sure if not global instances needed
            // InputI2S::input.setFrom(in);
            // OutputI2S::output.cpyTo(out);        
        }



        OutputI2S::OutputI2S()
        {
        }

        void OutputI2S::init(void* args)
        {
            i2sAudioCallback = audioCallback;
            i2sOut.begin();
        }

        void OutputI2S::update()
        {
            for(size_t i = 0; i < MAX_MODULE_IO; i++)
            {
                output.add(&inputBuffers[i], MAX_MODULE_IO);
            }
        }
    }
}