
#ifndef IMAGE_H
#define	IMAGE_H

#define NEWA(type,n) (type*)malloc(sizeof(type)*(n))
#define NEW(type) NEWA(type,1)

typedef struct {
    int width, height;
    unsigned char* r;
    unsigned char* g;
    unsigned char* b;
} image_rgb;

typedef struct {
    int width, height;
    unsigned char* data;
} image_gray;

typedef struct {
    int width, height;
    float* h;  // [0,1]
    float* s;  // [0,1]
    float* l;  // [0,1]
} image_hsl;

typedef struct {
    int width, height;
    float* h;  // [0,1]
    float* s;  // [0,1]
    float* v;  // [0,1]
} image_hsv;

/* ************************************************** */
/* a group of functions which create or delete images */
image_hsl* image_hsl_new(int width, int height);
image_hsv* image_hsv_new(int width, int height);
image_gray* image_gray_new(int width, int height);
image_rgb* image_rgb_new(int width, int height);
void image_hsl_delete(image_hsl* image);
void image_hsv_delete(image_hsv* image);
void image_gray_delete(image_gray* image);
void image_rgb_delete(image_rgb* image);
/* ************************************************** */

// read a RGB image from a common compressed image format(such as 'JPG', 'BMP' etc), using OpenCV
image_rgb* load_rgb_image(const char *fname, int iscolor); // tested JPG & BMP format, and it should also works for other common format

// convert RGB color space to HSL color space
image_hsl* rgb2hsl(image_rgb *rgb); // tested

// convert RGB color space to HSV color space
image_hsv* rgb2hsv(image_rgb *rgb); // tested

// convert RGB color space to gray level
image_gray* rgb2gray(image_rgb *rgb); // tested

// get gray level histogram
int* get_gray_histogram(image_gray* gray); // tested

// get RGB histogram
int* get_rgb_histogram(image_rgb* rgb); // tested

// get HSV histogram
int* get_hsv_histogram(image_hsv* hsv); ///////////////////////

// get hue histogram
int* get_hue_histogram(image_hsv* hsv); // tested

float *get_saliency_map(image_rgb* rgb);

//normalized cut related
int count_segarea(int*seglable,int size,int lable);//tested
int* segment_hue_histogram(int*seglable,image_hsv* imagehsv,int nr,int nc,int lablenum);//tested
int* maxarea_segment(int*seglable,int size,int lablenum);//tested
#endif	/* IMAGE_H */

