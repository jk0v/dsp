// #pragma once
// #ifndef NN_MODULE_HPP
// #define NN_MODULE_HPP
// #include "conf.h"
// #include "audioModule.hpp"
// #include "FC.hpp"
// #include "LSTM.hpp"

// namespace Audio
// {
//     namespace Modules
//     {
//         class NNModule : public Module
//         {
//             public:
//             NNModule();
//             ~NNModule(){}

//             void update() override;
//             void loadWeights();

//             private:
//             float inBuffer[AUDIO_BLOCK_SAMPLES];
//             float outBuffer[AUDIO_BLOCK_SAMPLES];

//             NN::LSTM::LSTMLayerF32<1, NN_HIDDEN_SIZE> lstm;
//             NN::FC::FCLayerF32<NN_HIDDEN_SIZE, 1> fc;

//             char weightStringBuf[MAX_FILELENGTH];
//         };
//     }
// }

// #endif