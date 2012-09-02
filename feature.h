
#ifndef FEATURE_H
#define	FEATURE_H
#include <opencv/cv.h>
#include "image.h"

#ifndef PI
#define PI 3.1415927
#endif

#define VALVE 0.01

typedef struct {
    float mean;        // mean value
    float stdev;       // standard deviation
    float max;
    float min;
} fbrightness;
// the saturation feature shares the same structure as brightness
typedef fbrightness fsaturation; 

typedef struct {
    int contrast;
    int sig_pixels_num;
    float stdev;
} fgray_simplicity;

typedef struct {
    int sig_pixels_num;
    float ratio;
} frgb_simplicity;
// the HSV simplicity feature shares the same structure as RGB simplicity
typedef frgb_simplicity fhsv_simplicity;

typedef struct {
    int sig_pixels_num;
    float contrast;
    float stdev;
} fhue_hist;

typedef struct {
    float bestfit;
    float first_two_dev;
    float avg_dev;
} fcolor_harmony;

typedef struct {
    int n_ccc;    // the number of connected coherent components
    float r_lc;   // the ratio of the largest component
    float r_slc;  // the ratio of the second largest component
    int rank;     // the rank of the bin which maximizes the quantity of the CCC
    int s_rank;   // the second rank 
} fcolor_coherence;

typedef struct {
    float mseg_ave;
    float seg_ave_std;
    float seg_ave_contrast;
} fsegment_lightness;

typedef struct {
    float rmaxsize;
    float rcontrast;
} fSegment_size;

typedef struct {
    int fhues1;
    int fhues2;
    int fhues3;
    int fhues4;
    int fhues5;
    float fhues6;
} fSegment_hues;

typedef struct {
    float r_bg; // background size
	int n_cc;     // number of connected components in the binary map
	float r_lc;   // size of the largest components in the binary saliency map relative to the whole image
	float asv_lc; // average saliency value of the largest component in the binary saliency map
	int n_cc_bg;  // number of connected components in the image background.
	float r_lc_bg; // size of the largest connected component in the background relative to the whole image
	float d_cc;   // distance between connected components
	float d_rtp;  // distance from the rule of the third points
	float d_ci;   // distance from the center of image
} fsaliency_map;

// Get brightness feature - (1)
fbrightness get_brightness_feature(image_hsl* hsl); // tested

// Get saturation feature - (2)
fsaturation get_saturation_feature(image_hsl* hsl); // tested 

// Get colorfulness feature - (3)
float get_colorfulness_feature(image_rgb* rgb); // tested 

// Get naturalness feature - (4)
float get_naturalness_feature(image_hsl* hsl); // tested

// Get contrast feature - (5)
float get_contrast_feature(image_hsl* hsl); // teseted

// Get sharpness feature - (6)
float get_sharpness_feature(image_hsl* hsl); // tested

// Get texture feature - (7)
// miss

// Get gray scale feature - (8)
fgray_simplicity get_grayscale_simplicity_feature(image_gray* gray); // tested

// Get RGB simplicity feature - (9-1)
frgb_simplicity get_rgb_simplicity_feature(image_rgb* rgb); // tested

// Get HSV simplicity feature - (9-2)
fhsv_simplicity get_hsv_simplicity_feature(image_hsv* hsv); ////////////////

// Get color harmony feature - (10)
fcolor_harmony get_color_harmony_feature(image_hsl* hsl); // tested

// Get hue histogram feature - (11)
fhue_hist get_hue_histogram_feature(image_hsv* hsv); // tested

fcolor_coherence get_color_coherence_feature(image_hsv* hsv); ///////////////////

//基于分割的局部特征 分割后的图像正常情况是5部分 小于5%的部分舍去 下面的涉及到图像标签的部分均是取舍后的
//标签从1开始 舍去的部分标签为0 不算在分图像分割总个数内
//Get Segment size feature -(12)
//注意返回的指针指向的是动态分配的内存
fSegment_size get_segsize_feature(int*seglable,int nr,int nc,int lablenum);

//Get Segment hues feature-(13)
fSegment_hues get_seghues_feature(int*seglable,image_hsv* imagehsv,int nr,int nc,int lablenum);
int get_seghues_feature1(int* seghistogram,int imagesize,int mlable);
int get_seghues_feature2(int* seghistogram,int*seglable,int imagesize,int lablenum);
int get_seghues_feature3(int* seghistogram,int*seglable,int imagesize,int lablenum);
int get_seghues_feature4(int* seghistogram,int msize,int mlable);
float get_seghues_feature5(int* seghistogram,int*seglable,int imagesize,int lablenum);


//Get Segment color harmony feature-(14)
fcolor_harmony get_segcolor_harmony_feature(image_hsl* hsl,int* seglable,int lablenum);

//Get Segment lightness feature-(15)
fsegment_lightness get_seglightness_feature(image_hsl* hsl,int* seglable, int lablenum);
float get_seglightness_average(image_hsl* hsl,int* seglable, int lable);
float get_seglightness_std(image_hsl* hsl,int* seglable, int lablenum);
float get_segavelightness_contrast(image_hsl* hsl,int* seglable, int lablenum);



fsaliency_map get_saliency_map_feature(float* saliencyMap, int width, int height); // tested

int get_face_feature(IplImage* image_detect, CvHaarClassifierCascade* classifier, CvSize minSize); // tested

#endif	/* FEATURES_H */