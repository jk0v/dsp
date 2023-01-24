#pragma once
#ifndef NN_LSTM_H
#define NN_LSTM_H
#include "Layer.h"

// inspired by https://github.com/jatinchowdhury18/RTNeural/blob/main/RTNeural/lstm/lstm.h
namespace NN
{
    namespace LSTM
    {
        template <typename T, int inSize, int outSize>
        class LSTMLayer : 
        {
            public:
            LSTMLayer();
            ~LSTMLayer() {}

            T forward(const T (&inState)[inSize])
            {
                recurrent_mat_mul(outState)
            }

        };
    }
}
#endif