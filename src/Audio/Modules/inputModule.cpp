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
            status = UpdateStatus::UNFINISHED;
        }

        UpdateStatus InputI2S::update()
        {
            if(status != UNFINISHED && updateCount > 0) { return UpdateStatus::PASSING; }
            data.cpyTo(&outputBuffers[0]);

            status = UpdateStatus::UNFINISHED;
            return UpdateStatus::FINISHED;
        }
    }
}