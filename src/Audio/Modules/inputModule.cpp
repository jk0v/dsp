#include "inputModule.hpp"
// #include "util.h"

namespace Audio
{
    namespace Modules
    {
        void InputI2S::init()
        {
            i2sAudioCallback = audioCallback;
            i2sIn.begin();

            type = ModuleType::IN_I2S;
        }

        UpdateStatus InputI2S::update()
        {
            data.cpyTo(&outputBuffers[0]);
            return FINISHED;
        }
    }
}