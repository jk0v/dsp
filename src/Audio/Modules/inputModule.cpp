#include "inputModule.hpp"
// #include "util.h"

namespace Audio
{
    namespace Modules
    {
        void InputI2S::init(void* args)
        {
            i2sAudioCallback = audioCallback;
            i2sIn.begin();

            type = ModuleType::IN_I2S;
        }

        void InputI2S::update()
        {
            data.cpyTo(&outputBuffers[0]);
        }
    }
}