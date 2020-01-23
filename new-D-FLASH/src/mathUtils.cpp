#include "mathUtils.h"

unsigned int getLog2(unsigned int x) {
    if (x < 2) {
        return 0;
    }

    unsigned int y = 1;
    do {
        x /= 2;
        y++;
    } while (x > 1);

    return y;
}

// http://stackoverflow.com/questions/364985/algorithm-for-finding-the-smallest-power-of-two-thats-greater-or-equal-to-a-giv
int smallestPow2(int x) {
    if (x < 0) {
        return 0;
    }

    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;

    return x + 1;
}

float mean(float values[], int n) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += values[i];
    }
    return sum / n;
}

float var(float values[], int n, float valuesMean) {
    float sum = 0;
    for (int i = 0; i < n; i++) {
        sum += (values[i] - valuesMean) * (values[i] - valuesMean);
    }
    return sum / (n - 1);
}

/*
Z-score standardize the result.
Minus mean, then divide by std.
*/
void zCentering(float *values, int n) {
    float valuesMean = mean(values, n);
    float valuesStd = sqrtf(var(values, n, valuesMean));
    for (int i = 0; i < n; i++) {
        values[i] = (values[i] - valuesMean) / valuesStd;
    }
}

float cosineDist(float *A, float *B, unsigned int n) {

    float up = 0;
    for (int i = 0; i < n; i++)
        up += A[i] * B[i];

    float a = 0;
    for (int i = 0; i < n; i++)
        a += A[i] * A[i];

    float b = 0;
    for (int i = 0; i < n; i++)
        b += B[i] * B[i];
    a = sqrtf(a);
    b = sqrtf(b);

    return up / (a * b);
}

float cosineDist(unsigned int *indiceA, float *valA, int nonzerosA, unsigned int *indiceB,
                 float *valB, int nonzerosB) {

    float up = 0;
    float a = 0;
    float b = 0;
    unsigned int startA, endA, startB, endB;

    up = SparseVecMul(indiceA, valA, nonzerosA, indiceB, valB, nonzerosB);
    a = SparseVecMul(indiceA, valA, nonzerosA, indiceA, valA, nonzerosA);
    b = SparseVecMul(indiceB, valB, nonzerosB, indiceB, valB, nonzerosB);
    a = sqrtf(a);
    b = sqrtf(b);
    if (a == 0 || b == 0) {
        return 0;
    }
    return up / (a * b);
}

float SparseVecMul(unsigned int *indicesA, float *valuesA, unsigned int sizeA, float *B) {

    float result = 0;
    unsigned int ctA = 0;
    unsigned int iA;

    while (ctA < sizeA) {
        iA = indicesA[ctA];
        result += valuesA[ctA] * B[iA];
        ctA++;
    }
    return result;
}

float SparseVecMul(unsigned int *indicesA, float *valuesA, unsigned int sizeA,
                   unsigned int *indicesB, float *valuesB, unsigned int sizeB) {

    float result = 0;
    unsigned int ctA = 0;
    unsigned int ctB = 0;
    unsigned int iA, iB;

    /* Maximum iteration: nonzerosA + nonzerosB.*/
    while (ctA < sizeA && ctB < sizeB) {
        iA = indicesA[ctA];
        iB = indicesB[ctB];

        if (iA == iB) {
            result += valuesA[ctA] * valuesB[ctB];
            ctA++;
            ctB++;
        } else if (iA < iB) {
            ctA++;
        } else if (iA > iB) {
            ctB++;
        }
    }
    return result;
}
