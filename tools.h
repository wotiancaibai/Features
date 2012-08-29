
#ifndef TOOLS_H
#define	TOOLS_H

// find the max among 3 numbers
float fmax3(float a, float b, float c); // tested

// find the min among 3 numbers
float fmin3(float a, float b, float c); // tested

// find the max number in an array
float amax(float* a, int n); // tested

// find the min number in an array
float amin(float* a, int n); // tested

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

#endif	/* TOOLS_H */

