#pragma once
#ifndef NN_LSTM_H
#define NN_LSTM_H
#include "Layer.h"

// inspired by https://github.com/jatinchowdhury18/RTNeural/blob/main/RTNeural/lstm/lstm.h and https://github.com/GuitarML/Proteus/blob/main/src/RTNeuralLSTM.cpp
// gucken ob float weights kompatibel mit int daten (vorkonvertierung)
namespace NN
{
    namespace LSTM
    {
        template <typename T, int inSize, int outSize>
        class LSTMLayer
        {
            public:
            LSTMLayer();
            ~LSTMLayer() {}

            void reset();

            inline void forward(const T (&inState)[inSize]);

            void setWWeights();
            void setUWeights();
            void setBWeights();

            // inline void recurrentMatMult(const (&vec)[outSize], const T (&mat)[outSize][outSize], T (&out)[outSize])
            // {
            //     for(int i = 0; i < outSize; ++i)
            //     {
            //         out[i] = 
            //     }
            // }

            private:
            T[outSize] wWeights;
            T[outSize] uWeights;
            T[outSize] bWeights;

        };
    }
}
#endif