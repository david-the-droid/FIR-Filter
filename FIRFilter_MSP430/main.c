//******************************************************************************
//!
//!   Example of using fixed point arithmetic in a FIR
//!
//******************************************************************************

#include <stdio.h>
#include <stdbool.h>
#include "driverlib.h"
#include "main.h"

#define INPUT_LEN 61
#define OUTPUT_LEN 200
#define COEFFICIENT_LEN 10

static float coefficients[10] =
{
    0.0110631440768700,
    0.0313470712799202,
    0.0877380341023896,
    0.159724142609811,
    0.210127607931010,
    0.210127607931010,
    0.159724142609811,
    0.0877380341023896,
    0.0313470712799202,
    0.0110631440768700
};

// SINE
static float input_samples[INPUT_LEN] =
{
    14.80832428,
    42.15658097,
    54.37874427,
    71.24064984,
    94.21531206,
    85.08679701,
    92.67667474,
    77.14190311,
    63.03911208,
    78.62527575,
    58.88416197,
    9.317883017,
    -6.374913347,
    -23.76350564,
    -88.43588138,
    -119.484247,
    -79.21710758,
    -104.5797391,
    -129.0581992,
    -93.03536544,
    -82.41902389,
    -49.67963569,
    -61.58488894,
    -38.57576301,
    -27.89967505,
    -24.76952345,
    26.85836588,
    75.26664532,
    58.59559254,
    89.78325949,
    69.72546444,
    113.1787841,
    103.9343685,
    59.41997255,
    39.12395792,
    48.21284805,
    25.8576457,
    -1.319899986,
    -39.05203522,
    -57.21640262,
    -73.95098846,
    -101.9209554,
    -75.17773477,
    -121.5056421,
    -115.3564221,
    -94.60469757,
    -79.1437481,
    -77.40692263,
    -57.7821318,
    -25.14193898,
    5.227710357,
    34.16750645,
    36.1600397,
    62.27624838,
    77.97514617,
    75.56158547,
    106.7823722,
    93.715181,
    69.05951092,
    81.31218981
};

/* STEP
static double input_samples[INPUT_LEN] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
    500,
};
*/
/*
static double input_samples[INPUT_LEN] =
{
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    500,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
*/

static float sample_storage[200] = {0};
static int sample_index = 0;

/* Fixed point arithmetic */
const int scale = 8;
const uint32_t ONE = 0x01;
#define FLOATFIXED(A) ((A) * (float)(ONE<<scale))
#define FIXEDFLOAT(A) ((float)(A) / (float)(ONE<<scale))
#define FIXEDMULT(x, y) ((((x)>>2)*((y)>>6))>>0)
#define FIXEDMULTFIR(x, y) ((((x)>>3)*((y)>>1))>>4)

int main() {

    configure_device();
    run_test();
    return 0;
}

static void run_test(void)
{
    P1OUT ^= BIT0;  // toggle pin

    float output_array[OUTPUT_LEN] = {0};
    int index;


    for(index = 0; index < INPUT_LEN; index++)
    {
        output_array[index] = calculate_filtered_value(input_samples[index]);
    }

    P1OUT ^= BIT0;
}

float calculate_filtered_value(float input_sample)
{
    float sum = 0;
    int fix_val_1 = 0;
    int fix_val_2 = 0;
    int mult_out = 0;
    float result = 0;
    int fir_index = 0;

    sample_storage[sample_index] = input_sample;

    for (fir_index = 0; fir_index < COEFFICIENT_LEN; fir_index++)
    {
        if (sample_index-fir_index < 0) // prevents accessing undefined memory
        {
            break;
        }

        fix_val_1 = FLOATFIXED(sample_storage[sample_index-fir_index]);
        fix_val_2 = FLOATFIXED(coefficients[fir_index]);
        mult_out = FIXEDMULTFIR(fix_val_1, fix_val_2);
        result = FIXEDFLOAT(mult_out);

        sum = sum + result;

        //sum = sum + (coefficients[fir_index] * sample_storage[sample_index-fir_index]);
    }

    sample_index++;

    return sum;
}

void configure_device(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog
    P1OUT &= ~BIT0;
    P1DIR |= BIT0;
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
}

