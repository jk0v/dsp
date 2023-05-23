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
            strcpy(name, "InputI2S");
            // status = UpdateStatus::UNFINISHED;
        }

        void InputI2S::update()
        {
            // if(status != UNFINISHED && updateCount > 0) { return UpdateStatus::PASSING; }
            // input.cpyTo(&outputBuffers[0]);
            // // digitalToggleFast(35);

            // status = UpdateStatus::UNFINISHED;
        }
    }
}