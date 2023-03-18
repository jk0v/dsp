#include "NNMath.h"


/*
 * @brief Neural network activation function using direct table look-up
 *
 * @note  Refer header file for details.
 *
 */

void arm_nn_activation_s16(const int16_t *input,
                           int16_t *output,
                           const uint16_t size,
                           const uint16_t left_shift,
                           const arm_nn_activation_type type)
{
    uint32_t abs_input_shift, max_saturation;
    switch (type)
    {
    case ARM_SIGMOID:
        abs_input_shift = 9;
        max_saturation = 0x7FFF << 10;
        break;
    case ARM_TANH:
    default:
        abs_input_shift = 8;
        max_saturation = 0xFFFF << 8;
        break;
    }

    // Use the LUT for sigmoid and take into account, that
    // tanh(x) = 2*sigmoid(2*x) - 1
    int32_t input_multiplier = ((int32_t)3) << left_shift;

    for (int i = 0; i < size; ++i, input++, output++)
    {
        int32_t input_data = ((*input) * input_multiplier);

        uint32_t abs_input_data = input_data > 0 ? input_data : -input_data;

        uint32_t uh = abs_input_data >> abs_input_shift;

        uint32_t result;

        if (uh >= 255)
        {
            result = max_saturation;
        }
        else
        {
            uint32_t ua = sigmoid_table_uint16[uh];
            uint32_t ub = sigmoid_table_uint16[uh + 1];
            uint32_t ut;
            if (type == ARM_SIGMOID)
            {
                ut = abs_input_data & 0x1ff;
            }
            else
            {
                ut = abs_input_data & 0x0ff;
            }
            result = (ua << abs_input_shift) + ut * (ub - ua);
        }
        if (type == ARM_SIGMOID)
        {
            result = (input_data >= 0) ? (result + (1 << 9)) : ((1 << 25) - result + (1 << 9) - 1);
            result >>= 10;
        }
        else
        {
            result = (input_data >= 0) ? (result - (1 << 23)) + (1 << 7) : ((-result + (1 << 23)) + (1 << 7) - 1);
            result >>= 8;
        }
        *output = (int16_t)result;
    }
}
