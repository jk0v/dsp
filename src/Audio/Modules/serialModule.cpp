#include "serialModule.hpp"

namespace Audio
{
    namespace Modules
    {
        void SerialModule::init()
        {
            Serial.begin(115200);
            status = UpdateStatus::UNFINISHED;
            Serial.println("Begin");
        }

        void SerialModule::update()
        {
            // Serial.printf("Data");
            // return UNFINISHED;
        }
    }
}