#pragma once
#ifndef NN_LSTM_HPP
#define NN_LSTM_HPP
#include <stdint.h>
#include <Arduino.h>
#include <arm_math.h>
#include "NNMath/NNMath.h"

// inspired by https://github.com/jatinchowdhury18/RTNeural/blob/main/RTNeural/lstm/lstm.h and https://github.com/GuitarML/Proteus/blob/main/src/RTNeuralLSTM.cpp
// TODO: gucken ob float weights kompatibel mit int daten (vorkonvertierung)
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
                const arm_matrix_instance_f32 tmpMat = {outSize, 1, (float32_t*)mat};        
                const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
                arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

                arm_mat_mult_f32(&tmpMat, &tmpVec, &tmpOut);
            }
            static inline void kernelMatMult(const float (&vec)[inSize], const float (&mat)[outSize][inSize], float (&out)[outSize]) noexcept{}
            inline void computeOutputs(float (&inState)) noexcept
            {
                // // compute ct
                // recurrMatMult(outState, Uc, ht);
                // for(int i=0; i<outSize; ++i)
                // {
                //     ct[i] = it[i] * tanhf(ht[i] + bc[i] + (Wc_1[i] * inState)) + ft[i] * ct[i];
                // }
                // // compute output
                // for(int i=0; i<outSize; ++i)
                // {
                //     outState[i] = ot[i] * tanhf(ct[i]);
                // }
                // compute ct
                recurrMatMult(outState, Uc, ht);
                for(int i=0; i<outSize; ++i)
                {
                    // NN::Math::activationI24((int32_t)(ht[i] + bc[i] + (Wc_1[i] * inState) + ft[i] * ct[i]), &ct[i], 0, 1);
                    // ct[i] *= it[i];
                    
                    digitalToggleFast(35);
                    // ct[i] = it[i] * tanhf(ht[i] + bc[i] + (Wc_1[i] * inState)) + ft[i] * ct[i];
                    NN::Math::tanhF32(ht[i] + bc[i] + (Wc_1[i] * inState), &ct[i]);
                    digitalToggleFast(35);
                    ct[i] = ct[i] * it[i] + ft[i]*ct[i];
                }
                // compute output
                for(int i=0; i<outSize; ++i)
                {
                    // outState[i] = ot[i] * tanhf(ct[i]);
                    // NN::Math::activationI24((int32_t)ct[i], &outState[i], 0, 1);
                    // outState[i] *= ot[i];
                    outState[i] = ot[i] * tanhf(ct[i]);
                }
            }

            inline void forward(float &inState) // inSize temporär == 1
            {
                // compute ft
                recurrMatMult(outState, Uf, ft);
                for(int i=0; i<outSize; ++i)
                {
                    // ft[i] = NN::Math::sigmoidF32(ft[i] + bf[i] + (Wf_1[i] * inState));
                    NN::Math::sigmoidF32(ft[i] + bf[i] + (Wf_1[i] * inState), &ft[i]);
                }
                // compute it
                recurrMatMult(outState, Ui, it);
                for(int i=0; i<outSize; ++i)
                {
                    // it[i] = NN::Math::sigmoidF32(it[i] + bi[i] + (Wi_1[i] * inState));
                    NN::Math::sigmoidF32(it[i] + bi[i] + (Wi_1[i] * inState), &it[i]);
                }
                // compute ot
                recurrMatMult(outState, Uo, ot);
                for(int i=0; i<outSize; ++i)
                {
                    // ot[i] = NN::Math::sigmoidF32(ot[i] + bo[i] + (Wo_1[i] * inState));
                    NN::Math::sigmoidF32(ot[i] + bo[i] + (Wo_1[i] * inState), &ot[i]);
                }

                // // compute ft
                // recurrMatMult(outState, Uf, ft);
                // for(int i=0; i<outSize; ++i)
                // {
                //     NN::Math::activationI24((int32_t)(ft[i] + bf[i] + (Wf_1[i] * inState)), &ft[i], 0, 0);
                // }
                // // compute it
                // recurrMatMult(outState, Ui, it);
                // for(int i=0; i<outSize; ++i)
                // {
                //     NN::Math::activationI24((int32_t)(it[i] + bi[i] + (Wi_1[i] * inState)), &it[i], 0, 0);
                // }
                // // compute ot
                // recurrMatMult(outState, Uo, ot);
                // for(int i=0; i<outSize; ++i)
                // {
                //     NN::Math::activationI24((int32_t)(ot[i] + bo[i] + (Wo_1[i] * inState)), &ot[i], 0, 0);
                // }
                
                computeOutputs(inState);
            }
            // sets the layer kernel weights
            void setWWeights(const float (&wVals)[inSize][4*outSize])
            {
                for(int i=0; i<inSize; ++i)
                {
                    for(int j=0; j<outSize; ++j)
                    {
                        Wi[j][i] = wVals[i][j];
                        Wf[j][i] = wVals[i][j + outSize];
                        Wc[j][i] = wVals[i][j + 2 * outSize];
                        Wo[j][i] = wVals[i][j + 3 * outSize];
                    }
                }
            }
            // sets the layer recurrent weights
            void setUWeights(const float (&uVals)[outSize][4*outSize])
            {
                for(int i=0; i<outSize; ++i)
                {
                    for(int j=0; j<outSize; ++j)
                    {
                        Ui[j][i] = uVals[i][j];
                        Uf[j][i] = uVals[i][j + outSize];
                        Uc[j][i] = uVals[i][j + 2 * outSize];
                        Uo[j][i] = uVals[i][j + 3 * outSize];
                    }
                }
            }
            // sets the layer bias
            void setBWeights(const float (&bVals)[4*outSize])
            {
                for(int i=0; i<outSize; ++i)
                {
                    bi[i] = bVals[i];
                    bf[i] = bVals[i + outSize];
                    bc[i] = bVals[i + 2 * outSize];
                    bo[i] = bVals[i + 3 * outSize];
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
        
        /*
        template <typename T, int32_t inSize, int32_t outSize>
        class LSTMLayer
        {
            public:
            LSTMLayer();
            ~LSTMLayer() {}

            void reset();

            inline void forward(const T (&inState)[inSize]);

            // sets the layer kernel weights
            void setWWeights(const T (&wVals)[inSize][4*outSize]);
            // sets the layer recurrent weights
            void setUWeights(const T (&uVals)[outSize][4*outSize]);
            // sets the layer bias
            void setBWeights(const T (&bVals)[4*outSize]);

            private:
            T outState[outSize];

            // kernel weights
            T Wf[outSize][inSize];
            T Wi[outSize][inSize];
            T Wo[outSize][inSize];
            T Wc[outSize][inSize];
            T kernelOutState[outSize];

            // single-input kernel weights
            T Wf_1[outSize];
            T Wi_1[outSize];
            T Wo_1[outSize];
            T Wc_1[outSize];

            // recurrent weights
            T Uf[outSize][outSize];
            T Ui[outSize][outSize];
            T Uo[outSize][outSize];
            T Uc[outSize][outSize];

            // biases
            T bf[outSize];
            T bi[outSize];
            T bo[outSize];
            T bc[outSize];

            // intermediate vars
            T ft[outSize];
            T it[outSize];
            T ot[outSize];
            T ht[outSize];
            T ct[outSize];


            static inline void recurrMatMult(const T (&vec)[outSize], const T (&mat)[outSize][outSize], T (&out)[outSize]) noexcept;
            static inline void kernelMatMult(const T (&vec)[inSize], const T (&mat)[outSize][inSize], T (&out)[outSize]) noexcept;
        };
        template <int32_t inSize, int32_t outSize>
        class LSTMLayer<int32_t, inSize, outSize>
        {
            public:
            LSTMLayer();
            ~LSTMLayer() {}

            void reset();

            inline void forward(const int32_t (&inState)[inSize]);

            // sets the layer kernel weights
            void setWWeights(const int32_t (&wVals)[inSize][4*outSize]);
            // sets the layer recurrent weights
            void setUWeights(const int32_t (&uVals)[outSize][4*outSize]);
            // sets the layer bias
            void setBWeights(const int32_t (&bVals)[4*outSize]);

            private:
            int32_t outState[outSize];

            // kernel weights
            int32_t Wf[outSize][inSize];
            int32_t Wi[outSize][inSize];
            int32_t Wo[outSize][inSize];
            int32_t Wc[outSize][inSize];
            int32_t kernelOutState[outSize];

            // single-input kernel weights
            int32_t Wf_1[outSize];
            int32_t Wi_1[outSize];
            int32_t Wo_1[outSize];
            int32_t Wc_1[outSize];

            // recurrent weights
            int32_t Uf[outSize][outSize];
            int32_t Ui[outSize][outSize];
            int32_t Uo[outSize][outSize];
            int32_t Uc[outSize][outSize];

            // biases
            int32_t bf[outSize];
            int32_t bi[outSize];
            int32_t bo[outSize];
            int32_t bc[outSize];

            // intermediate vars
            int32_t ft[outSize];
            int32_t it[outSize];
            int32_t ot[outSize];
            int32_t ht[outSize];
            int32_t ct[outSize];


            static inline void recurrMatMult(const int32_t (&vec)[outSize], const int32_t (&mat)[outSize][outSize], int32_t (&out)[outSize]) noexcept;
            static inline void kernelMatMult(const int32_t (&vec)[inSize], const int32_t (&mat)[outSize][inSize], int32_t (&out)[outSize]) noexcept;
        };
        */

//         template <int32_t inSize, int32_t outSize>
//         class LSTMLayerI32
//         {
//             public:
//             void reset()
//             {
//                 for(int i=0; i< outSize; ++i)
//                 {
//                     ct[i] = 0;
//                     outState[i] = 0;
//                 }
//             }
//             LSTMLayerI32()
//             {
//                 for(int i = 0; i < outSize; ++i)
//                 {
//                     // single-input kernel weights
//                     Wf_1[i] = 0;
//                     Wi_1[i] = 0;
//                     Wo_1[i] = 0;
//                     Wc_1[i] = 0;

//                     // biases
//                     bf[i] = 0;
//                     bi[i] = 0;
//                     bo[i] = 0;
//                     bc[i] = 0;

//                     // intermediate vars
//                     ft[i] = 0;
//                     it[i] = 0;
//                     ot[i] = 0;
//                     ht[i] = 0;
//                 }

//                 for(int i = 0; i < outSize; ++i)
//                 {
//                     // recurrent weights
//                     for(int j = 0; j < outSize; ++j)
//                     {
//                         Uf[i][j] = 0;
//                         Ui[i][j] = 0;
//                         Uo[i][j] = 0;
//                         Uc[i][j] = 0;
//                     }

//                     // kernel weights
//                     for(int j = 0; j < inSize; ++j)
//                     {
//                         Wf[i][j] = 0;
//                         Wi[i][j] = 0;
//                         Wo[i][j] = 0;
//                         Wc[i][j] = 0;
//                     }
//                 }
                
//                 this->reset();
//             }
//             ~LSTMLayerI32() {}


//             static inline void recurrMatMult(int32_t (&vec)[outSize], int32_t (&mat)[outSize][outSize], int32_t (&out)[outSize]) noexcept
//             {
//                 // const arm_matrix_instance_f32 tmpMat = {outSize, 1, (float32_t*)mat};        
//                 // const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
//                 // arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

//                 // arm_mat_mult_f32(&tmpMat, &tmpVec, &tmpOut);
//             }
//             static inline void kernelMatMult(const int32_t (&vec)[inSize], const int32_t (&mat)[outSize][inSize], int32_t (&out)[outSize]) noexcept{}
//             inline void computeOutputs(int32_t (&inState)) noexcept
//             {
//                 // compute ct
//                 recurrMatMult(outState, Uc, ht);
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     NN::Math::activationI24(ht[i] + bc[i] + (Wc_1[i] * inState) + ft[i] * ct[i], ct[i], 0, 1);
//                     ct[i] *= it[i];
//                 }
//                 // compute output
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     // outState[i] = ot[i] * tanhf(ct[i]);
//                     NN::Math::activationI24(ct[i], outState[i], 0, 1);
//                     outState[i] *= ot[i];
//                 }
//             }

//             inline void forward(int32_t &inState) // inSize temporär == 1
//             {
//                 // compute ft
//                 recurrMatMult(outState, Uf, ft);
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     NN::Math::activationI24(ft[i] + bf[i] + (Wf_1[i] * inState), ft[i], 0, 0);
//                 }
//                 // compute it
//                 recurrMatMult(outState, Ui, it);
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     NN::Math::activationI24(it[i] + bi[i] + (Wi_1[i] * inState), it[i], 0, 0);
//                 }
//                 // compute ot
//                 recurrMatMult(outState, Uo, ot);
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     NN::Math::activationI24(ot[i] + bo[i] + (Wo_1[i] * inState), ot[i], 0, 0);
//                 }
                
//                 computeOutputs(inState);
//             }
//             // sets the layer kernel weights
//             void setWWeights(const int32_t (&wVals)[inSize][4*outSize])
//             {
//                 for(int i=0; i<inSize; ++i)
//                 {
//                     for(int j=0; j<outSize; ++j)
//                     {
//                         Wi[j][i] = wVals[i][j];
//                         Wf[j][i] = wVals[i][j + outSize];
//                         Wc[j][i] = wVals[i][j + 2 * outSize];
//                         Wo[j][i] = wVals[i][j + 3 * outSize];
//                     }
//                 }
//             }
//             // sets the layer recurrent weights
//             void setUWeights(const int32_t (&uVals)[outSize][4*outSize])
//             {
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     for(int j=0; j<outSize; ++j)
//                     {
//                         Ui[j][i] = uVals[i][j];
//                         Uf[j][i] = uVals[i][j + outSize];
//                         Uc[j][i] = uVals[i][j + 2 * outSize];
//                         Uo[j][i] = uVals[i][j + 3 * outSize];
//                     }
//                 }
//             }
//             // sets the layer bias
//             void setBWeights(const int32_t (&bVals)[4*outSize])
//             {
//                 for(int i=0; i<outSize; ++i)
//                 {
//                     bi[i] = bVals[i];
//                     bf[i] = bVals[i + outSize];
//                     bc[i] = bVals[i + 2 * outSize];
//                     bo[i] = bVals[i + 3 * outSize];
//                 }
//             }

//             int32_t outState[outSize];
//             private:

//             // kernel weights
//             float Wf[outSize][inSize];
//             float Wi[outSize][inSize];
//             float Wo[outSize][inSize];
//             float Wc[outSize][inSize];
//             float kernelOutState[outSize];

//             // single-input kernel weights
//             float Wf_1[outSize];
//             float Wi_1[outSize];
//             float Wo_1[outSize];
//             float Wc_1[outSize];

//             // recurrent weights
//             float Uf[outSize][outSize];
//             float Ui[outSize][outSize];
//             float Uo[outSize][outSize];
//             float Uc[outSize][outSize];

//             // biases
//             float bf[outSize];
//             float bi[outSize];
//             float bo[outSize];
//             float bc[outSize];

//             // intermediate vars
//             float ft[outSize];
//             float it[outSize];
//             float ot[outSize];
//             float ht[outSize];
//             float ct[outSize];
//         };
    }
}
#endif