#pragma once
#ifndef NN_LSTM_HPP
#define NN_LSTM_HPP
#include <stdint.h>

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
            LSTMLayerF32();
            ~LSTMLayerF32() {}

            void reset();

            inline void forward(const float (*inState));

            // sets the layer kernel weights
            void setWWeights(const float (&wVals)[inSize][4*outSize]);
            // sets the layer recurrent weights
            void setUWeights(const float (&uVals)[outSize][4*outSize]);
            // sets the layer bias
            void setBWeights(const float (&bVals)[4*outSize]);

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


            static inline void recurrMatMult(const float (&vec)[outSize], const float (&mat)[outSize][outSize], float (&out)[outSize]) noexcept;
            static inline void kernelMatMult(const float (&vec)[inSize], const float (&mat)[outSize][inSize], float (&out)[outSize]) noexcept{}

            inline void computeOutputs(const float (&inState)[inSize]) noexcept;
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
    }
}
#endif