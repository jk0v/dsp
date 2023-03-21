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
                // NN::Math::matVecMultF32(inState, inSize, weights, outSize, inSize, outState); // probably wrong
                matMult(inState, weights, outState);
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

            inline void matMult(float (&vec)[inSize], float (&mat)[inSize]/*[outSize]*/, float (&out)[outSize])
            {
                // digitalToggleFast(35);
                const arm_matrix_instance_f32 tmpMat = {outSize, 1, mat};        
                const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
                arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

                arm_mat_mult_f32(&tmpMat, &tmpVec, &tmpOut);
                // digitalToggleFast(35);
            }

            float outState[outSize];

            private:
            float bias[outSize];
            float weights[outSize*inSize];
        };

    }
}

#endif