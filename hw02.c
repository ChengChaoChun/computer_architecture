#include <stdio.h>
#include <stdint.h>
extern uint64_t get_cycles();
//Define a union for the conversion between floating-point numbers and integers
typedef union {
    float f;          // The floating-point part
    unsigned int i;   // The integer part
} float_to_int;

//Define a function to convert single-precision floating-point numbers to the bfloat16 format.
float fp32_to_bf16(float x) {
    float y = x;                  // Copy the input value to y
    int *p = (int *) &y;          // Create an integer pointer to access the binary representation of a floating-point number
    unsigned int exp = *p & 0x7F800000; //  Extract the exponent part of the floating-point number
    unsigned int man = *p & 0x007FFFFF;  // Extract the mantissa part of the floating-point number
    // If it is zero or infinity/NaN, simply return the input value x
    if (exp == 0 && man == 0)
        return x;
    if (exp == 0x7F800000)
        return x;

    // Perform rounding on normalized numbers.
    // First, create a new floating-point number "r" for rounding.
    float r = x;
    int *pr = (int *) &r;
    *pr &= 0xFF800000;  // The exponent part of "r" is the same as that of "x"
    r /= 0x100;         // Right-shift by 8 bits, which is equivalent to dividing by 256, to implement rounding
    y = x + r;          // Add the rounded value back to "x"
    *p &= 0xFFFF0000;   // Clear the fractional part of "y" and retain the integer part
    return y;           // Return the bfloat16 value
}

int main() {
    uint64_t oldcount = get_cycles();
    // Define two 2x2 matrices, A and B, as well as a result matrix C, all initialized to zero
    float A[2][2] = {
        {1.2345, 2.3456},
        {3.4567, 4.5678}
    };

    float B[2][2] = {
        {0.1234, 0.2345},
        {0.3456, 0.4567}
    };

    float C[2][2] = {
        {0.0, 0.0},
        {0.0, 0.0}
    };
    
    C[0][0] += fp32_to_bf16(A[0][0]) * fp32_to_bf16(B[0][0]);
    C[0][0] += fp32_to_bf16(A[0][1]) * fp32_to_bf16(B[1][0]);
    
    C[0][1] += fp32_to_bf16(A[0][0]) * fp32_to_bf16(B[0][1]);
    C[0][1] += fp32_to_bf16(A[0][1]) * fp32_to_bf16(B[1][1]);
    
    C[1][0] += fp32_to_bf16(A[1][0]) * fp32_to_bf16(B[0][0]);
    C[1][0] += fp32_to_bf16(A[1][1]) * fp32_to_bf16(B[1][0]);
    
    C[1][1] += fp32_to_bf16(A[1][0]) * fp32_to_bf16(B[0][1]);
    C[1][1] += fp32_to_bf16(A[1][1]) * fp32_to_bf16(B[1][1]);
    
    
    printf("Result Matrix C:\n");
    printf("%f ", C[0][0]);
    printf("%f \n", C[0][1]);
    printf("%f ", C[1][0]);
    printf("%f ", C[1][1]);
    uint64_t cyclecount = get_cycles() - oldcount;
    printf("cycle count: %u\n", (unsigned int) cyclecount);
    return 0;
}
