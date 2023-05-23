#pragma once
#ifndef NN_LSTM_HPP
#define NN_LSTM_HPP
#define ARDUINOJSON_USE_DOUBLE 0

#include <stdint.h>
#include <Arduino.h>
#include <arm_math.h>
#include <ArduinoJson.h>
#include "NNMath/NNMath.h"
#include <vector>

namespace NN
{
    namespace LSTM
    { 
        // float impl (using FPU) (int32_t -> float32_t: ok: float32_t ca. 24bit prec.), 
        // inSize must be 1 (TODO: kernel impl for inSize > 1)
        template <int32_t inSize, int32_t outSize>
        class LSTMLayerF32
        {
            public:
            void reset()
            {
                for(int i=0; i< outSize; ++i)
                {
                    ct[i] = (float) 0;
                    outState[i] = (float) 0;
                }
            }
            LSTMLayerF32()
            {
                for(int i = 0; i < outSize; ++i)
                {
                    // single-input kernel weights
                    Wf_1[i] = (float) 0;
                    Wi_1[i] = (float) 0;
                    Wo_1[i] = (float) 0;
                    Wc_1[i] = (float) 0;

                    // biases
                    bf[i] = (float) 0;
                    bi[i] = (float) 0;
                    bo[i] = (float) 0;
                    bc[i] = (float) 0;

                    // intermediate vars
                    ft[i] = (float) 0;
                    it[i] = (float) 0;
                    ot[i] = (float) 0;
                    ht[i] = (float) 0;
                }

                for(int i = 0; i < outSize; ++i)
                {
                    // recurrent weights
                    for(int j = 0; j < outSize; ++j)
                    {
                        Uf[i][j] = (float) 0;
                        Ui[i][j] = (float) 0;
                        Uo[i][j] = (float) 0;
                        Uc[i][j] = (float) 0;
                    }

                    // kernel weights
                    for(int j = 0; j < inSize; ++j)
                    {
                        Wf[i][j] = (float) 0;
                        Wi[i][j] = (float) 0;
                        Wo[i][j] = (float) 0;
                        Wc[i][j] = (float) 0;
                    }
                }
                
                this->reset();
            }
            ~LSTMLayerF32() {}


            static inline void recurrMatMult(float (&vec)[outSize], float (&mat)[outSize][outSize], float (&out)[outSize]) noexcept
            {
                const arm_matrix_instance_f32 tmpMat = {outSize, outSize, (float*)mat};        
                const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
                arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

                arm_mat_mult_f32(&tmpVec, &tmpMat, &tmpOut); // flipped
            }
            static inline void kernelMatMult(const float (&vec)[inSize], const float (&mat)[outSize][inSize], float (&out)[outSize]) noexcept{}
            inline void computeOutputs(float (&inState)) noexcept
            {
                // compute ct
                recurrMatMult(outState, Uc, ht);
                for(int i=0; i<outSize; ++i)
                {
                    ct[i] = it[i] * tanhf(ht[i] + bc[i] + (Wc_1[i] * inState)) + ft[i] * ct[i];
                }
                // compute output
                for(int i=0; i<outSize; ++i)
                {
                    outState[i] = ot[i] * tanhf(ct[i]);
                }
            }

            inline void forward(float &inState)
            {
                // compute ft
                recurrMatMult(outState, Uf, ft);
                for(int i=0; i<outSize; ++i)
                {
                    NN::Math::sigmoidF32(ft[i] + bf[i] + (Wf_1[i] * inState), &ft[i]);
                }
                // compute it
                recurrMatMult(outState, Ui, it);
                for(int i=0; i<outSize; ++i)
                {
                    NN::Math::sigmoidF32(it[i] + bi[i] + (Wi_1[i] * inState), &it[i]);
                }
                // compute ot
                recurrMatMult(outState, Uo, ot);
                for(int i=0; i<outSize; ++i)
                {
                    NN::Math::sigmoidF32(ot[i] + bo[i] + (Wo_1[i] * inState), &ot[i]);
                }
                
                computeOutputs(inState);
            }
            // sets the layer kernel weights (transpose in here)
            void setWWeights(JsonArray wVals)
            {
                for(int i=0; i<outSize; ++i)
                {
                    Wi_1[i] = (float)wVals[i][0];
                    Wf_1[i] = (float)wVals[i + outSize][0];
                    Wc_1[i] = (float)wVals[i + 2 * outSize][0];
                    Wo_1[i] = (float)wVals[i + 3 * outSize][0];
                }
            }
            // sets the layer recurrent weights (transpose in here)
            void setUWeights(JsonArray uVals)
            {
                for(int i=0; i<outSize; ++i)
                {
                    for(int j=0; j<outSize; ++j)
                    {
                        Ui[j][i] = (float)uVals[j][i];
                        Uf[j][i] = (float)uVals[j + outSize][i];
                        Uc[j][i] = (float)uVals[j + 2 * outSize][i];
                        Uo[j][i] = (float)uVals[j + 3 * outSize][i];
                    }
                }

            }
            // sets the layer bias
            void setBWeights(JsonArray bVals)
            {
                for(int i=0; i<outSize; ++i)
                {
                    bi[i] = (float)bVals[i];
                    bf[i] = (float)bVals[i + outSize];
                    bc[i] = (float)bVals[i + 2 * outSize];
                    bo[i] = (float)bVals[i + 3 * outSize];
                }
            }

            float outState[outSize];
            private:

            // kernel weights
            float Wf[outSize][inSize];
            float Wi[outSize][inSize];
            float Wo[outSize][inSize];
            float Wc[outSize][inSize];
            float kernelOutState[outSize];

            // single-input kernel weights
            float Wf_1[outSize];
            float Wi_1[outSize];
            float Wo_1[outSize];
            float Wc_1[outSize];

            // recurrent weights
            float Uf[outSize][outSize];
            float Ui[outSize][outSize];
            float Uo[outSize][outSize];
            float Uc[outSize][outSize];

            // biases
            float bf[outSize];
            float bi[outSize];
            float bo[outSize];
            float bc[outSize];

            // intermediate vars
            float ft[outSize];
            float it[outSize];
            float ot[outSize];
            float ht[outSize];
            float ct[outSize];
        };
    }
}
#endif