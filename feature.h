
#ifndef FEATURE_H
#define	FEATURE_H
#include "image.h"

#define PI 3.1415927
#define VALVE 0.01

typedef struct {
    float mean;        // mean value
    float stdev;       // standard deviation
    float max;
    float min;
} brightness;
// the saturation feature shares the same structure as brightness
typedef brightness saturation; 

typedef struct {
    int contrast;
    int sig_pixels_num;
    float stdev;
} gray_simplicity;

typedef struct {
    int sig_pixels_num;
    float ratio;
} rgb_simplicity;
// the HSV simplicity feature shares the same structure as RGB simplicity
typedef rgb_simplicity hsv_simplicity;

typedef struct {
    int sig_pixels_num;
    float contrast;
    float stdev;
} hue_hist;

typedef struct {
    float bestfit;
    float fisrt_two_dev;
    float avg_dev;
} color_harmony;

typedef struct {
    int n_ccc;    // the number of connected coherent components
    float r_lc;   // the ratio of the largest component
    float r_slc;  // the ratio of the second largest component
    int rank;     // the rank of the bin which maximizes the quantity of the CCC
    int s_rank;   // the second rank 
} color_coherence;

// Get brightness feature - (1)
brightness* get_brightness_feature(image_hsl* hsl); // tested

// Get saturation feature - (2)
saturation* get_saturation_feature(image_hsl* hsl); // tested 

// Get colorfulness feature - (3)
float get_colorfulness_feature(image_rgb* hsl); // tested 

// Get naturalness feature - (4)
float get_naturalness_feature(image_hsl* hsl); // tested

// Get contrast feature - (5)
float get_contrast_feature(image_hsl* hsl); // teseted

// Get sharpness feature - (6)
float get_sharpness_feature(image_hsl* hsl); // tested

// Get texture feature - (7)
// miss

// Get gray scale feature - (8)
gray_simplicity* get_grayscale_simplicity_feature(image_gray* gray); // tested

// Get RGB simplicity feature - (9-1)
rgb_simplicity* get_rgb_simplicity_feature(image_rgb* rgb); // tested

// Get HSV simplicity feature - (9-2)
hsv_simplicity* get_hsv_simplicity_feature(image_hsv* hsv); ////////////////

// Get color harmony feature - (10)
color_harmony* get_color_harmony_feature(image_hsl* hsl); // tested

// Get hue histogram feature - (11)
hue_hist* get_hue_histogram_feature(image_hsv* hsv); // tested

color_coherence* get_color_coherence_feature(image_hsv* hsv); ///////////////////

#endif	/* FEATURES_H */