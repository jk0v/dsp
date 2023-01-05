#include "inputModule.hpp"
// #include "util.h"

namespace Audio
{
    namespace Modules
    {
        InputI2S::InputI2S()
        {
        }

        void InputI2S::init(void* args)
        {
            i2sAudioCallback = audioCallback;
            i2sIn.begin();
        }
    }
}