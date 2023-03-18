#pragma once
#ifndef NN_MATH_H
#define NN_MATH_H
#include <stdint.h>
#include "NNMathUtil.h"

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
void arm_nn_activation_s16(const int16_t *input,
                           int16_t *output,
                           const uint16_t size,
                           const uint16_t left_shift,
                           const arm_nn_activation_type type);
                        
#endif