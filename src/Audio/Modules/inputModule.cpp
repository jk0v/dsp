#include "inputModule.hpp"

namespace Audio::Modules
{
    InputI2S::InputI2S()
    {
        i2sAudioCallback = audioCallback;
        i2sIn.begin();
    }
}