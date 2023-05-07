#include "NNModule.hpp"
#include <ArduinoJson.h>
#include <SD.h>
#include <dspinst.h>
#include <arm_math.h>

namespace Audio
{
    namespace Modules
    {
        NNModule::NNModule()
        {
        }

        void NNModule::update()
        {
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                // inBuffer[i] = (float)inputBuffers[0]->data[0][i]; // not normalized
                inBuffer[i] = (float)inputBuffers[0]->data[0][i] * F24_NORM_MAX_INV; 
            }            

            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                lstm.forward(inBuffer[i]);
                fc.forward(lstm.outState);
                outputBuffers[0].data[0][i] = (float)fc.outState[0] * F24_NORM_MAX;
            }
        }

        void NNModule::loadWeights(const char* path)
        {
            // load json file
            File weightsFile = SD.open("/AmpPack1/BlackstarHT40_AmpHighGain.json", FILE_READ);
            if (weightsFile)
            {
                // deserialize json
                DynamicJsonDocument weights(weightsFile.size()/2);
                DeserializationError err = deserializeJson(weights, weightsFile);
                if(err)
                {
                    // Serial.printf("Weights serialization failed: %s", err.c_str());
                }

                // set lstm kernel & recurrent weights
                JsonArray lstm_weights_ih = weights["state_dict"]["rec.weight_ih_l0"];
                JsonArray lstm_weights_hh = weights["state_dict"]["rec.weight_hh_l0"];
                lstm.setWWeights(lstm_weights_ih);
                lstm.setUWeights(lstm_weights_hh);

                // set lstm bias
                JsonArray lstm_bias_ih = weights["state_dict"]["rec.bias_ih_l0"];
                JsonArray lstm_bias_hh = weights["state_dict"]["rec.bias_hh_l0"];
                for(int i=0; i<NN_HIDDEN_SIZE*4; ++i)
                {
                    float hh = lstm_bias_hh[i]; 
                    float ih = lstm_bias_ih[i];
                    lstm_bias_hh[i] = hh + ih;
                }
                lstm.setBWeights(lstm_bias_hh);

                // set fc weights & bias
                JsonArray fc_weights = weights["state_dict"]["lin.weight"];
                JsonArray fc_bias = weights["state_dict"]["lin.bias"];
                fc.setWeights(fc_weights);
                fc.setBias(fc_bias);

                // release json memory
                weights.clear();
                weightsFile.close();
            }
        }
    }
}