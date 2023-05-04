#pragma once
#ifndef NN_MATH_H
#define NN_MATH_H
#include <stdint.h>
#include <arm_math.h>
#include <dspinst.h>
#include <Arduino.h>
#include "NNMathUtil.h"

namespace NN
{
    namespace Math
    {
        /**
         * @brief s16 neural network activation function using direct table look-up
         * @param[in]       input        pointer to input data
         * @param[out]      output      pointer to output
         * @param[in]       size        number of elements
         * @param[in]       left_shift  bit-width of the integer part, assume to be smaller than 3
         * @param[in]       type        type of activation functions
         *
         * @details Supported framework: TensorFlow Lite for Microcontrollers.
         * This activation function must be bit precise congruent with the corresponding TFLM tanh and sigmoid actication
         * functions
         */
        void arm_nn_activation_s16(const int16_t *input, int16_t *output, const uint16_t size, const uint16_t left_shift, const arm_nn_activation_type type);

        // sigmoid/tanh activation for 24bit values using 1024 entry lut
        // void activationI24(const int32_t *input, int32_t *output, const uint16_t size, const uint16_t left_shift,const arm_nn_activation_type type);
        void activationI24(int32_t input, float *output, const uint16_t left_shift, const arm_nn_activation_type type);

        // self
        // inline float sigmoidF32(float in) noexcept
        // {
        //     return 1.f/(1.f+expf(-(in))); // slow af (264ns)
        //     // return (in <= )0.5f+(in/(3+((in <= 0) ? in : -in))); // alot faster (ca. 36ns), but only approx. (1-o. tayl.s.)
        //     // return 0.5f*tanhf(0.5f*in) + 0.5f; // even slower
        // }
        inline void sigmoidF32(float in, float* out) noexcept
        {
            // *out = 1.f/(1.f+expf(-(in))); // slow af (264ns)
            // *out = /*(in >= 4.f) ? 1.f : */0.5f+(in/(3.f+((in <= 0.f) ? in : -in))); // alot faster (ca. 36ns), but only approx. (1-o. tayl.s.)
            *out = /*(in >= 4.f) ? 1.f :*/ 0.5f+(in/(3.f+fabsf(in))); // alot faster (ca. 52ns), but only approx. (1-o. tayl.s.)
            // return 0.5f*tanhf(0.5f*in) + 0.5f; // even slower
        }
        inline void tanhF32(float in, float* out) noexcept
        {
            *out = 2.f/(1+expf(-2.f*in))-1;
        }
        // inline void matVecMultF32(const float vec[], const uint16_t vecSize, const float (**mat), const uint16_t matSizeR, const uint16_t matSizeC, float (&out)[], const uint16_t outSize) noexcept;
    }
}
#endif