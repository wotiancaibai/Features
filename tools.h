
#ifndef TOOLS_H
#define	TOOLS_H

// find the max among 3 numbers
float fmax3(float a, float b, float c); // tested

// find the min among 3 numbers
float fmin3(float a, float b, float c); // tested

// find the max number in an array
float amax(float* a, int n); // tested
int intamax(int* a, int n); // tested

// find the min number in an array
float amin(float* a, int n); // tested
int intamin(int* a, int n); // tested

// get the mean value of the array
float amean(float* a, int n); // tested

// easy implemented "selection sort"(descend) for integer
void isort(int* data, int* index, int n);  // tested

// easy implemented "selection sort"(descend) for float
void fsort(float* data, int* index, int n);  // tested

// get the standard deviation of the array
float get_stdev(float*a, int n);  // tested

// return an arc length normalized in [0, 0.5]
float get_arc_length(float a, float b); // tested

void fft1D(float *arrayBuf, int n); // tested

float* fft2D(float* imgBuf, int width, int height); // tested

void* fftshift(float* fftBuf, int width, int height); // tested

void ifft1D(float *arrayBuf, int n);

float* ifft2D(float* fftBuf, int width, int height);

float* get_phase_angle(float* fftBuf, int width, int height); // tested

float* get_log_amplitude(float* fftBuf, int width, int height); // tested

float* mean_filter(float* data, int width, int height); // tested

float* gaussian_filter(float* data, int width, int height); // tested

#endif	/* TOOLS_H */

