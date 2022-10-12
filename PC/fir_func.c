#include <stdio.h>
#include <stdbool.h>

#define INPUT_LEN 61
#define OUTPUT_LEN 200
#define COEFFICIENT_LEN 10

static double coefficients[10] = 
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

static double input_samples[INPUT_LEN] = 
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

static double sample_storage[200] = {0};
static int index = 0;

double calculate_filtered_value(double input_sample)
{
    double sum = 0;
    int fir_index = 0;
    double coef = 0;
    double sample = 0;

    sample_storage[index] = input_sample;

    for (fir_index = 0; fir_index < COEFFICIENT_LEN; fir_index++)
    {
        if (index-fir_index < 0) // bad things will happen if we don't break here
        {
            break; 
        }
        sum = sum + (coefficients[fir_index] * sample_storage[index-fir_index]);   
    }

    index++;
    
    return sum; 
}


int main() {
   // printf() displays the string inside quotation
   printf("Starting FIR Filter program!\n");

    int for_loop_count = 0;
    double output_array[OUTPUT_LEN] = {0};
    double sum = 0;
    char *filename = "test.txt";

    for(int index = 0; index < INPUT_LEN; index++) // outer loop fine
    {
        output_array[index] = calculate_filtered_value(input_samples[index]);
    }

    printf("Samples:\n");

    for(int index = 0; index < OUTPUT_LEN; index++)
    {
        printf(" %f\n", output_array[index]);
    }    

    // open the file for writing
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Error opening the file %s", filename);
        return -1;
    }
    // write to the text file
    for (int i = 0; i < OUTPUT_LEN; i++)
        fprintf(fp, "%f\n", output_array[i]);

    // close the file
    fclose(fp);

   return 0;
}