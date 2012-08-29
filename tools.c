
#include <math.h>
#include "tools.h"

float fmax3(float a, float b, float c) {
    float r = a;
    if (b > r)
        r = b;
    if (c > r)
        r = c;
    
    return r;
}

float fmin3(float a, float b, float c) {
    return -fmax3(-a, -b, -c);
}

float amax(float* a, int n) {
    int i;
    float r = a[0];
    for (i = 1; i < n; ++i) {
        if (a[i] > r)
            r = a[i];
    }
    
    return r;
}

float amin(float* a, int n) {
    int i;
    float r = a[0];
    for (i = 1; i < n; ++i) {
        if (a[i] < r)
            r = a[i];
    }
    
    return r;
}

float amean(float* a, int n) {
    int i;
    float s = 0;
    
    for (i = 0; i < n; ++i) {
        s += a[i];
    }
    s /= n;
    
    return s;
}

void isort(int* data, int* index /* = NULL */, int n) {
    int i, j, k, temp;
    
    if (index) { // if we want to record the original index
        for (i = 0; i < n; ++i) {
            index[i] = i;
        }
        for (i = 0; i < n-1; ++i) {
            k = i;
            for (j = i+1; j < n; ++j) {
                if (data[k] < data[j]) 
                    k = j;
            }
            if (i != k) {
                temp = data[i];
                data[i] = data[k];
                data[k] = temp;
                temp = index[i];
                index[i] = index[k];
                index[k] = temp;
            }
        }
    } else {
        for (i = 0; i < n-1; ++i) {
            k = i;
            for (j = i+1; j < n; ++j) {
                if (data[k] < data[j]) 
                    k = j;
            }
            if (i != k) {
                temp = data[i];
                data[i] = data[k];
                data[k] = temp;
            }
        }
    }
}

void fsort(float* data, int* index /* = NULL */, int n) {
    int i, j, k;
	float temp;
    
    if (index) { // if we want to record the original index
        for (i = 0; i < n; ++i) {
            index[i] = i;
        }
        for (i = 0; i < n-1; ++i) {
            k = i;
            for (j = i+1; j < n; ++j) {
                if (data[k] < data[j]) 
                    k = j;
            }
            if (i != k) {
                temp = data[i];
                data[i] = data[k];
                data[k] = temp;
                temp = index[i];
                index[i] = index[k];
                index[k] = temp;
            }
        }
    } else {
        for (i = 0; i < n-1; ++i) {
            k = i;
            for (j = i+1; j < n; ++j) {
                if (data[k] < data[j]) 
                    k = j;
            }
            if (i != k) {
                temp = data[i];
                data[i] = data[k];
                data[k] = temp;
            }
        }
    }
}

float get_stdev(float*a, int n) {
    int i;
    float mean, stdev = 0;
    
    mean = amean(a, n);
    for (i = 0; i < n; ++i) {
        stdev += (a[i] - mean) * (a[i] - mean);
    }
    
    return sqrt(stdev/n);
}

float get_arc_length(float a, float b) {
    float r = fabs(a - b);
    if (r > 0.5)
        r = 1 - r;
    
    return r;
}
