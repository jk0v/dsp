#include "inputModule.hpp"

namespace Audio::Modules
{
    InputI2S::InputI2S()
    {
        i2sIn.begin();
        i2sAudioCallback = audioCallback;
    }
}