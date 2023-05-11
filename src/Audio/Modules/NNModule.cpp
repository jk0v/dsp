#include "NNModule.hpp"
#include <ArduinoJson.h>
#include <SD.h>
#include "outputModule.hpp"

extern Audio::Modules::OutputI2S outI2S;
extern Audio::Modules::InputI2S inI2S;

namespace Audio
{
    namespace Modules
    {
        NNModule::NNModule()
        {
            type = NN_MOD;
        }

        void NNModule::update()
        {
            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                // inBuffer[i] = (float)inputBuffers[0]->data[0][i]; // not normalized
                inBuffer[i] = (float)inputBuffers[0]->data[0][i] * F24_NORM_MAX_INV; 
                // Serial.printf("Norm data %i: %.15f", i, inBuffer[i]);
            }     

            for(int i=0; i<AUDIO_BLOCK_SAMPLES; ++i)
            {
                lstm.forward(inBuffer[i]);
                fc.forward(lstm.outState);
                outputBuffers[0].data[0][i] = (int32_t)((fc.outState[0]) * F24_NORM_MAX);
                // Serial.printf("Norm data %i: %.15f", i, inBuffer[i]);
                // outputBuffers[0].data[0][i] = (int32_t)(inBuffer[i] * F24_NORM_MAX);
            }
        }

        void NNModule::loadWeights(const char* path)
        {
            // disable audio interupts
            outI2S.disableI2S();
            // load json file
            File weightsFile = SD.open(path);
            if (weightsFile)
            {
                DynamicJsonDocument weights(weightsFile.size());
                DeserializationError err = deserializeJson(weights, weightsFile);
                if(err)
                {
                    // Serial.printf("Weights serialzation failed: %s", err.c_str());
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

                // reenable audio interupts
                outI2S.enableI2S();

                // // deserialize json
                // DynamicJsonDocument weights(weightsFile.size()/2);
                // DeserializationError err = deserializeJson(weights, weightsFile);
                // if(err)
                // {
                //     // Serial.printf("Weights serialization failed: %s", err.c_str());
                // }

                // // set lstm kernel & recurrent weights
                // JsonArray lstm_weights_ih = weights["state_dict"]["rec.weight_ih_l0"];
                // JsonArray lstm_weights_hh = weights["state_dict"]["rec.weight_hh_l0"];
                // lstm.setWWeights(lstm_weights_ih);
                // lstm.setUWeights(lstm_weights_hh);

                // // set lstm bias
                // JsonArray lstm_bias_ih = weights["state_dict"]["rec.bias_ih_l0"];
                // JsonArray lstm_bias_hh = weights["state_dict"]["rec.bias_hh_l0"];
                // for(int i=0; i<NN_HIDDEN_SIZE*4; ++i)
                // {
                //     float hh = lstm_bias_hh[i]; 
                //     float ih = lstm_bias_ih[i];
                //     lstm_bias_hh[i] = hh + ih;
                // }
                // lstm.setBWeights(lstm_bias_hh);

                // // set fc weights & bias
                // JsonArray fc_weights = weights["state_dict"]["lin.weight"];
                // JsonArray fc_bias = weights["state_dict"]["lin.bias"];
                // fc.setWeights(fc_weights);
                // fc.setBias(fc_bias);

                // // release json memory
                // weights.clear();
                // weightsFile.close();
            }
        }
        void NNModule::test(float test)
        {
            lstm.forward(test);
            for(int i=0; i<NN_HIDDEN_SIZE; ++i)
            {
                Serial.printf("%d: %.10f,", i, lstm.outState[i]);
            }
            fc.forward(lstm.outState);
            Serial.printf("%.10f", fc.outState[0]);

        }
    }
}