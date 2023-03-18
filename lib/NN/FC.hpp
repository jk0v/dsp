#pragma once
#ifndef FC_HPP
#define FC_HPP
#include <stdint.h>
#include <arm_math.h>
#include "NNMath/NNMath.h"

namespace NN
{
    namespace FC
    {
        template <int32_t inSize, int32_t outSize>
        class FCLayerF32
        {
            public:
            FCLayerF32()
            {
                for(int i=0; i<outSize*inSize; ++i)
                {
                    weights[i] = (float) 0;
                }
                for(int i=0; i<outSize; ++i)
                {
                    bias[i] = (float) 0;
                    outState[i] = (float) 0;
                }
            }
            ~FCLayerF32(){}

            void reset(){}

            inline void forward(float (&inState)[inSize]) noexcept
            {
                NN::Math::matVecMultF32(inState, inSize, weights, outSize, inSize, outState); // probably wrong
                for(int i=0; i<outSize; ++i)
                {
                    outState[i] += bias[i];
                }
            }

            void setWeights(float** newWeights)
            {
                for(int i=0; i<outSize; ++i)
                {
                    for(int j=0; j<inSize; ++i)
                    {
                        weights[i*inSize+j] = newWeights[i][j];
                    }
                }
            }
            void setBias(float* b)
            {
                for(int i=0; i<outSize; ++i)
                {
                    bias[i] = b[i];
                }
            }

            float outState[outSize];

            private:
            float bias[outSize];
            float weights[outSize*inSize];
        };

    }
}

#endif