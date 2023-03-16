#include "LSTM.h"
#include <arm_math.h>
#include <string.h>

namespace NN
{
    namespace LSTM
    {
        template <typename T, int inSize, int outSize>
        LSTMLayer<T, inSize, outSize>::LSTMLayer()
        {
            memset(wWeights, 0, outSize);
            memset(wWeights, 0, outSize);
            memset(wWeights, 0, outSize);
            memset(wWeights, 0, outSize);
            this.reset();
        }

        template <typename T, int inSize, int outSize>
        inline void LSTMLayer<T, inSize, outSize>::forward(const T (&inState)[inSize])
        {
            
        }
    }
}