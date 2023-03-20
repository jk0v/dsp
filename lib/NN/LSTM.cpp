#include "LSTM.hpp"
#include <arm_math.h>
#include "NNMath/NNMath.h"

namespace NN
{
    namespace LSTM
    {
        template <int32_t inSize, int32_t outSize>
        LSTMLayerF32<inSize, outSize>::LSTMLayerF32()
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
            
            this.reset();
        }

        template <int32_t inSize, int32_t outSize>
        void LSTMLayerF32<inSize, outSize>::reset()
        {
            for(int i=0; i< outSize; ++i)
            {
                ct[i] = (float) 0;
                outState[i] = (float) 0;
            }
        }

        template <int32_t inSize, int32_t outSize>
        void LSTMLayerF32<inSize, outSize>::setWWeights(const float (&wVals)[inSize][4*outSize])
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
        template <int32_t inSize, int32_t outSize>
        void LSTMLayerF32<inSize, outSize>::setUWeights(const float (&uVals)[outSize][4*outSize])
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
        template <int32_t inSize, int32_t outSize>
        void LSTMLayerF32<inSize, outSize>::setBWeights(const float (&bVals)[4*outSize])
        {
            for(int i=0; i<outSize; ++i)
            {
                bi[i] = bVals[i];
                bf[i] = bVals[i + outSize];
                bc[i] = bVals[i + 2 * outSize];
                bo[i] = bVals[i + 3 * outSize];
            }
        }
        
        // for inSize==1
        // template <int32_t inSize, int32_t outSize>
        // inline void LSTMLayerF32<inSize, outSize>::forward(float &inState)
        // {
        //     // compute ft
        //     recurrMatMult(outState, Uf, ft);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         ft[i] = NN::Math::sigmoidF32(ft[i] + bf[i] + (Wf_1[i] * &inState));
        //     }
        //     // compute it
        //     recurrMatMult(outState, Ui, it);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         it[i] = NN::Math::sigmoidF32(it[i] + bi[i] + (Wi_1[i] * &inState));
        //     }
        //     // compute ot
        //     recurrMatMult(outState, Uo, ot);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         ot[i] = NN::Math::sigmoidF32(ot[i] + bo[i] + (Wo_1[i] * &inState));
        //     }
            
        //     computeOutputs(inState);
        // }

        // template <int32_t inSize, int32_t outSize>
        // inline void LSTMLayerF32<inSize, outSize>::recurrMatMult(const float (&vec)[outSize], const float (&mat)[outSize][outSize], float (&out)[outSize]) noexcept
        // {
        //     // // const arm_matrix_instance_f32 tmpMat;
        //     // // const arm_matrix_instance_f32 tmpVec;
        //     // // arm_matrix_instance_f32 tmpOut;
        //     // // arm_mat_init_f32(*tmpMat, outSize, outSize, mat);
        //     // // arm_mat_init_f32(*tmpVec, outSize, 1, vec);            
        //     // // arm_mat_init_f32(*tmpOut, outSize, 1, out);    
        //     // digitalToggle(35);
        //     // const arm_matrix_instance_f32 tmpMat = {outSize, 1, mat};        
        //     // const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
        //     // arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

        //     // arm_mat_mult_f32(tmpMat, tmpVec, tmpOut);
        //     // digitalToggle(35);
        //     // NN::Math::matVecMult(vec, mat, outSize, outSize, out, outSize);

        //     digitalToggleFast(35);
        //     const arm_matrix_instance_f32 tmpMat = {outSize, 1, mat};        
        //     const arm_matrix_instance_f32 tmpVec = {outSize, 1, vec};
        //     arm_matrix_instance_f32 tmpOut = {outSize, 1, out};

        //     arm_mat_mult_f32(&tmpMat, &tmpVec, &tmpOut);
        //     digitalToggleFast(35);
        // }

        // template <int32_t inSize, int32_t outSize>
        // inline void LSTMLayerF32<inSize, outSize>::computeOutputs(const float (&inState)[inSize]) noexcept
        // {
        //     // compute ct
        //     recurrMatMult(outState, Uc, ht);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         ct[i] = it[i] * tanhf(ht[i] + bc[i] + (Wc_1[i] * inState[0])) + ft[i] * ct[i];
        //     }
        //     // compute output
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         outState[i] = ot[i] * tanhf(ct[i]);
        //     }
        // }
        /*
        // template <int inSize, int outSize>
        // LSTMLayer<float, inSize, outSize>::LSTMLayer()
        // {
        //     for(int i = 0; i < outSize; ++i)
        //     {
        //         // single-input kernel weights
        //         Wf_1[i] = (float) 0;
        //         Wi_1[i] = (float) 0;
        //         Wo_1[i] = (float) 0;
        //         Wc_1[i] = (float) 0;

        //         // biases
        //         bf[i] = (float) 0;
        //         bi[i] = (float) 0;
        //         bo[i] = (float) 0;
        //         bc[i] = (float) 0;

        //         // intermediate vars
        //         ft[i] = (float) 0;
        //         it[i] = (float) 0;
        //         ot[i] = (float) 0;
        //         ht[i] = (float) 0;
        //     }

        //     for(int i = 0; i < out_size; ++i)
        //     {
        //         // recurrent weights
        //         for(int j = 0; j < out_size; ++j)
        //         {
        //             Uf[i][j] = (float) 0;
        //             Ui[i][j] = (float) 0;
        //             Uo[i][j] = (float) 0;
        //             Uc[i][j] = (float) 0;
        //         }

        //         // kernel weights
        //         for(int j = 0; j < in_size; ++j)
        //         {
        //             Wf[i][j] = (float) 0;
        //             Wi[i][j] = (float) 0;
        //             Wo[i][j] = (float) 0;
        //             Wc[i][j] = (float) 0;
        //         }
        //     }
            
        //     this.reset();
        // }

        // template <int inSize, int outSize>
        // void LSTMLayer<float, inSize, outSize>::reset()
        // {
        //     for(int i=0; i< outSize; ++i)
        //     {
        //         ct[i] = (float) 0;
        //         outState[i] = (float) 0;
        //     }
        // }

        // template <int inSize, int outSize>
        // void LSTMLayer<float, inSize, outSize>::setWWeights(const float (&wVals)[inSize][4*outSize])
        // {
        //     for(int i=0; i<inSize; ++i)
        //     {
        //         for(int j=0; j<outSize; ++j)
        //         {
        //             Wi[j][i] = wVals[i][j];
        //             Wf[j][i] = wVals[i][j + outSize];
        //             Wc[j][i] = wVals[i][j + 2 * outSize];
        //             Wo[j][i] = wVals[i][j + 3 * outSize];
        //         }
        //     }
        // }
        // template <int inSize, int outSize>
        // void LSTMLayer<float, inSize, outSize>::setUWeights(const float (&uVals)[outSize][4*outSize])
        // {
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         for(int j=0; j<outSize; ++j)
        //         {
        //             Ui[j][i] = uVals[i][j];
        //             Uf[j][i] = uVals[i][j + outSize];
        //             Uc[j][i] = uVals[i][j + 2 * outSize];
        //             Uo[j][i] = uVals[i][j + 3 * outSize];
        //         }
        //     }
        // }
        // template <int inSize, int outSize>
        // void LSTMLayer<float, inSize, outSize>::setBWeights(const float (&bVals)[4*outSize])
        // {
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         bi[i] = bVals[i];
        //         bf[i] = bVals[i + outSize];
        //         bc[i] = bVals[i + 2 * outSize];
        //         bo[i] = bVals[i + 3 * outSize];
        //     }
        // }
        
        // template <int inSize, int outSize>
        // inline void LSTMLayer<float, inSize, outSize>::forward(const float (&inState)[inSize])
        // {
        //     // arm_mat_mult_fast_q15()
        //     recurrMatMult(outState, Uf, ft);
        //     kernelMatMult(inState, Wf, kernelOutState);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         ft[i] = arm_nn_activation_s16
        //     }
        // }
        
        
        // template <int inSize, int outSize>
        // LSTMLayer<T, inSize, outSize>::LSTMLayer()
        // {
        //     for(int i = 0; i < outSize; ++i)
        //     {
        //         // single-input kernel weights
        //         Wf_1[i] = (T) 0;
        //         Wi_1[i] = (T) 0;
        //         Wo_1[i] = (T) 0;
        //         Wc_1[i] = (T) 0;

        //         // biases
        //         bf[i] = (T) 0;
        //         bi[i] = (T) 0;
        //         bo[i] = (T) 0;
        //         bc[i] = (T) 0;

        //         // intermediate vars
        //         ft[i] = (T) 0;
        //         it[i] = (T) 0;
        //         ot[i] = (T) 0;
        //         ht[i] = (T) 0;
        //     }

        //     for(int i = 0; i < out_size; ++i)
        //     {
        //         // recurrent weights
        //         for(int j = 0; j < out_size; ++j)
        //         {
        //             Uf[i][j] = (T) 0;
        //             Ui[i][j] = (T) 0;
        //             Uo[i][j] = (T) 0;
        //             Uc[i][j] = (T) 0;
        //         }

        //         // kernel weights
        //         for(int j = 0; j < in_size; ++j)
        //         {
        //             Wf[i][j] = (T) 0;
        //             Wi[i][j] = (T) 0;
        //             Wo[i][j] = (T) 0;
        //             Wc[i][j] = (T) 0;
        //         }
        //     }
            
        //     this.reset();
        // }

        // template <int inSize, int outSize>
        // void LSTMLayer<T, inSize, outSize>::reset()
        // {
        //     for(int i=0; i< outSize; ++i)
        //     {
        //         ct[i] = (T) 0;
        //         outState[i] = (T) 0;
        //     }
        // }

        // template <int inSize, int outSize>
        // void LSTMLayer<T, inSize, outSize>::setWWeights(const T (&wVals)[inSize][4*outSize])
        // {
        //     for(int i=0; i<inSize; ++i)
        //     {
        //         for(int j=0; j<outSize; ++j)
        //         {
        //             Wi[j][i] = wVals[i][j];
        //             Wf[j][i] = wVals[i][j + outSize];
        //             Wc[j][i] = wVals[i][j + 2 * outSize];
        //             Wo[j][i] = wVals[i][j + 3 * outSize];
        //         }
        //     }
        // }
        // template <int inSize, int outSize>
        // void LSTMLayer<T, inSize, outSize>::setUWeights(const T (&uVals)[outSize][4*outSize])
        // {
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         for(int j=0; j<outSize; ++j)
        //         {
        //             Ui[j][i] = uVals[i][j];
        //             Uf[j][i] = uVals[i][j + outSize];
        //             Uc[j][i] = uVals[i][j + 2 * outSize];
        //             Uo[j][i] = uVals[i][j + 3 * outSize];
        //         }
        //     }
        // }
        // template <int inSize, int outSize>
        // void LSTMLayer<T, inSize, outSize>::setBWeights(const T (&bVals)[4*outSize])
        // {
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         bi[i] = bVals[i];
        //         bf[i] = bVals[i + outSize];
        //         bc[i] = bVals[i + 2 * outSize];
        //         bo[i] = bVals[i + 3 * outSize];
        //     }
        // }
        
        // template <int inSize, int outSize>
        // inline void LSTMLayer<inSize, outSize>::forward(const T (&inState)[inSize])
        // {
        //     // arm_mat_mult_fast_q15()
        //     recurrMatMult(outState, Uf, ft);
        //     kernelMatMult(inState, Wf, kernelOutState);
        //     for(int i=0; i<outSize; ++i)
        //     {
        //         ft[i] = arm_nn_activation_s16
        //     }
        // }
        template <int inSize, int outSize>
        LSTMLayer<inSize, outSize>::LSTMLayer()
        {
            for(int i = 0; i < outSize; ++i)
            {
                // single-input kernel weights
                Wf_1[i] = 0;
                Wi_1[i] = 0;
                Wo_1[i] = 0;
                Wc_1[i] = 0;

                // biases
                bf[i] = 0;
                bi[i] = 0;
                bo[i] = 0;
                bc[i] = 0;

                // intermediate vars
                ft[i] = 0;
                it[i] = 0;
                ot[i] = 0;
                ht[i] = 0;
            }

            for(int i = 0; i < outSize; ++i)
            {
                // recurrent weights
                for(int j = 0; j < outSize; ++j)
                {
                    Uf[i][j] = 0;
                    Ui[i][j] = 0;
                    Uo[i][j] = 0;
                    Uc[i][j] = 0;
                }

                // kernel weights
                for(int j = 0; j < in_size; ++j)
                {
                    Wf[i][j] = 0;
                    Wi[i][j] = 0;
                    Wo[i][j] = 0;
                    Wc[i][j] = 0;
                }
            }
            
            this.reset();
        }

        template <int inSize, int outSize>
        void LSTMLayer<inSize, outSize>::reset()
        {
            for(int i=0; i< outSize; ++i)
            {
                ct[i] = 0;
                outState[i] = 0;
            }
        }

        template <int inSize, int outSize>
        void LSTMLayer<inSize, outSize>::setWWeights(const int32_t (&wVals)[inSize][4*outSize])
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
        template <int inSize, int outSize>
        void LSTMLayer<inSize, outSize>::setUWeights(const int32_t (&uVals)[outSize][4*outSize])
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
        template <int inSize, int outSize>
        void LSTMLayer<inSize, outSize>::setBWeights(const int32_t (&bVals)[4*outSize])
        {
            for(int i=0; i<outSize; ++i)
            {
                bi[i] = bVals[i];
                bf[i] = bVals[i + outSize];
                bc[i] = bVals[i + 2 * outSize];
                bo[i] = bVals[i + 3 * outSize];
            }
        }
        
        template <int inSize, int outSize>
        inline void LSTMLayer<inSize, outSize>::forward(const int32_t (&inState)[inSize])
        {
            // arm_mat_mult_fast_q15()
            recurrMatMult(outState, Uf, ft);
            kernelMatMult(inState, Wf, kernelOutState);
            for(int i=0; i<outSize; ++i)
            {
                ft[i] = arm_nn_activation_s16
            }
        }
        */
    }
}