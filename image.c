
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>

#include "tools.h"
#include "image.h"

image_hsl *image_hsl_new(int width, int height) {
    image_hsl* im = NEW(image_hsl);
    im->width = width;
    im->height = height;
    int wh = width * height;
    im->h = NEWA(float, wh);
    im->s = NEWA(float, wh);
    im->l = NEWA(float, wh);
    return im;
}

image_hsv *image_hsv_new(int width, int height) {
    image_hsv* im = NEW(image_hsv);
    im->width = width;
    im->height = height;
    int wh = width * height;
    im->h = NEWA(float, wh);
    im->s = NEWA(float, wh);
    im->v = NEWA(float, wh);
    return im;
}

image_gray* image_gray_new(int width, int height) {
    image_gray* im = NEW(image_gray);
    im->width = width;
    im->height = height;
    int wh = width * height;
    im->data = NEWA(unsigned char, wh);
    return im;
}

image_rgb *image_rgb_new(int width, int height) {
    image_rgb* im = NEW(image_rgb);
    im->width = width;
    im->height = height;
    int wh = width * height;
    im->r = NEWA(unsigned char, wh);
    im->g = NEWA(unsigned char, wh);
    im->b = NEWA(unsigned char, wh);
    return im;
}

void image_hsl_delete(image_hsl* im) {
    free(im->h);
    free(im->s);
    free(im->l);
    free(im);
}

void image_hsv_delete(image_hsv* im) {
    free(im->h);
    free(im->s);
    free(im->v);
    free(im);
}

void image_rgb_delete(image_rgb* im) {
    free(im->r);
    free(im->g);
    free(im->b);
    free(im);
}

void image_gray_delete(image_gray* im) {
    free(im->data);
    free(im);
}

image_rgb* load_image(const char *fname, int iscolor) {
  int i, j, width, height, widthStep;
  IplImage *image = cvLoadImage(fname, iscolor);
  if (!image) {
	  printf("load image err!\n");
	  exit(1);
  }
  width = image->width;
  height = image->height;
  widthStep = image->widthStep;
  
  image_rgb *im = image_rgb_new(width, height);
  
  if (iscolor) { // RGB
      for (i = 0; i < height; ++i) {
          for (j = 0; j < width; ++j) {
              im->r[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3 + 2];  // R
              im->g[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3 + 1];  // G
              im->b[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3];      // B
          }
      }
  } else { // gray level
      for (i = 0; i < height; ++i) {
          for (j = 0; j < width; ++j) {
              im->r[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
              im->g[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
              im->b[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
          }
      }
  }
  
  cvReleaseImage(&image);
  
  return im;
}

image_rgb* load_cv_image(IplImage *image) {
  int i, j, width, height, widthStep;
  if (!image) {
	  printf("load image err!\n");
	  exit(1);
  }
  width = image->width;
  height = image->height;
  widthStep = image->widthStep;
  
  image_rgb *im = image_rgb_new(width, height);
  
  if (image->nChannels > 1) { // RGB
      for (i = 0; i < height; ++i) {
          for (j = 0; j < width; ++j) {
              im->r[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3 + 2];  // R
              im->g[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3 + 1];  // G
              im->b[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j*3];      // B
          }
      }
  } else { // gray level
      for (i = 0; i < height; ++i) {
          for (j = 0; j < width; ++j) {
              im->r[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
              im->g[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
              im->b[i*width+j] = ((unsigned char*)image->imageData)[i*widthStep+j];
          }
      }
  }
  
  return im;
}

image_hsl* rgb2hsl(image_rgb* rgb) {
    int i, j;
    int width = rgb->width, height = rgb->height;
    float h, s, l,\
          r, g, b,\
          maxVal, minVal;

    image_hsl *hsl = image_hsl_new(width, height);
    // normalizes red-green-blue values
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            h = 0;
            s = 0; 
            l = 0;
            r = rgb->r[i*width+j]/255.f;
            g = rgb->g[i*width+j]/255.f;
            b = rgb->b[i*width+j]/255.f;
            maxVal = fmax3(r, g, b);
            minVal = fmin3(r, g, b);
            // hue
            if(maxVal == minVal) {
                h = 0; // undefined
            }
            else if(maxVal==r && g>=b) {
                h = 60.0f*(g-b)/(maxVal-minVal);
            }
            else if(maxVal==r && g<b) {
                h = 60.0f*(g-b)/(maxVal-minVal) + 360.0f;
            }
            else if(maxVal==g) {
                h = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;
            }
            else if(maxVal==b) {
                h = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;
            }
            // luminance
            l = (maxVal+minVal)/2.0f;
            // saturation
            if(l == 0 || maxVal == minVal) {
                s = 0;
            }
            else if(0<l && l<=0.5f) {
                s = (maxVal-minVal)/(maxVal+minVal);
            }
            else if(l>0.5f) {
                s = (maxVal-minVal)/(2 - (maxVal+minVal)); //(maxVal-minVal > 0)?
            }
            hsl->h[i*width+j] = ((h>=360)? 0 : ((h<0)?0:h))/360.f; 
            hsl->s[i*width+j] = ((s>1)? 1 : ((s<0)?0:s));
            hsl->l[i*width+j] = ((l>1)? 1 : ((l<0)?0:l));
        }
    }
    
    return hsl;
} 

image_hsv* rgb2hsv(image_rgb *rgb) {
    int i, j;
    int width = rgb->width, height = rgb->height;
    float h, s, v,\
          r, g, b,\
          maxVal, minVal;

    image_hsv *hsv = image_hsv_new(width, height);
    // normalizes red-green-blue values
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            h = 0;
            s = 0; 
            v = 0;
            r = rgb->r[i*width+j]/255.f;
            g = rgb->g[i*width+j]/255.f;
            b = rgb->b[i*width+j]/255.f;
            maxVal = fmax3(r, g, b);
            minVal = fmin3(r, g, b);
            // hue
            if(maxVal == minVal) {
                h = 0; // undefined
            }
            else if(maxVal==r && g>=b) {
                h = 60.0f*(g-b)/(maxVal-minVal);
            }
            else if(maxVal==r && g<b) {
                h = 60.0f*(g-b)/(maxVal-minVal) + 360.0f;
            }
            else if(maxVal==g) {
                h = 60.0f*(b-r)/(maxVal-minVal) + 120.0f;
            }
            else if(maxVal==b) {
                h = 60.0f*(r-g)/(maxVal-minVal) + 240.0f;
            }
            // luminance
            v = maxVal;
            // saturation
            if(maxVal == 0) {
                s = 0;
            }
            else {
                s = 1 - minVal/maxVal;
            }
            hsv->h[i*width+j] = ((h>=360)? 0 : ((h<0)?0:h))/360.f; 
            hsv->s[i*width+j] = ((s>1)? 1 : ((s<0)?0:s));
            hsv->v[i*width+j] = ((v>1)? 1 : ((v<0)?0:v));
        }
    }
    
    return hsv;
}

image_gray* rgb2gray(image_rgb* rgb) {
    int i, j;
    int width = rgb->width, height = rgb->height;
    int data;
    
    image_gray *gray = image_gray_new(width, height);
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            data = (int)(0.299*rgb->r[i*width+j] + 0.587*rgb->g[i*width+j] + 0.114*rgb->b[i*width+j] + 0.5);
            if (data > 255)
                data = 255;
            gray->data[i*width+j] = (unsigned char)data;
        }
    }
    
    return gray;
}

int* get_gray_histogram(image_gray* gray) {
    int i, j;
    int width = gray->width, height = gray->height;
    int *histogram = NEWA(int, 256);
    
    memset(histogram, 0, 256*sizeof(int));
    
    for (i = 0; i < height; ++i) { // get the gray histogram
        for (j = 0; j < width; ++j) {
            ++histogram[gray->data[i*width+j]];
        }
    }
    
    return histogram;
}

int* get_rgb_histogram(image_rgb* rgb) {
    int i, j;
    int width = rgb->width, height = rgb->height;
    int *histogram = NEWA(int, 512);
    int bins[3];
    
    memset(histogram, 0, 512*sizeof(int));
    
    for (i = 0; i < height; ++i) { 
        for (j = 0; j < width; ++j) {
            bins[0] = rgb->r[i*width+j] / 32;
            bins[1] = rgb->g[i*width+j] / 32;
            bins[2] = rgb->b[i*width+j] / 32;
            ++histogram[bins[0]*64 + bins[1]*8 + bins[2]];
        }
    }
    
    return histogram;
}

int* get_hsv_histogram(image_hsv* hsv) {
    int i, j;
    int width = hsv->width, height = hsv->height;
    int *histogram = NEWA(int, 512);
    int bins[3];
    
    memset(histogram, 0, 512*sizeof(int));
    
    for (i = 0; i < height; ++i) { 
        for (j = 0; j < width; ++j) {
            bins[0] = (int)(hsv->h[i*width+j] / 0.125);
            if (bins[0] > 7)
                bins[0] = 7;
            bins[1] = (int)(hsv->s[i*width+j] / 0.125);
            if (bins[1] > 7)
                bins[1] = 7;
            bins[2] = (int)(hsv->v[i*width+j] / 0.125);
            if (bins[2] > 7)
                bins[2] = 7;
            
            ++histogram[bins[0]*64 + bins[1]*8 + bins[2]];
        }
    }
    
    return histogram;
}

int* get_hue_histogram(image_hsv* hsv) {
    int i, j;
    int width = hsv->width, height = hsv->height;
    int *histogram = NEWA(int, 20);
    int index;
    memset(histogram, 0, 20*sizeof(int));
    
    for (i = 0; i < height; ++i) { 
        for (j = 0; j < width; ++j) {
            if (hsv->s[i*width+j] > 0.2 && hsv->v[i*width+j] > 0.2) {
                index = (int)(hsv->h[i*width+j] / 0.05);
                if (index > 19)
                    index = 19;
                ++histogram[index];
            }
        }
    }
    
    return histogram;
}

float *get_saliency_map(image_rgb* rgb) { 
    int i, j;
    int width = rgb->width, height = rgb->height;
    int wh = width*height;
    
    float gray[wh];
    fftw_complex *in1 = (fftw_complex *) fftw_malloc(width*height*sizeof(fftw_complex));
    fftw_complex *in2 = (fftw_complex *) fftw_malloc(width*height*sizeof(fftw_complex));
    fftw_complex *out = (fftw_complex *) fftw_malloc(width*height*sizeof(fftw_complex));
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            gray[i*width+j] = (0.299*rgb->r[i*width+j] + 0.587*rgb->g[i*width+j] + 0.114*rgb->b[i*width+j]) / 255.f;
        }
    }
    
    for(j = 0; j < height; j++) {
        for(i = 0; i < width; i++) {
            in1[j*width + i][0] = gray[j*width+i];
            in1[j*width + i][1] = 0.0f;
        }
    }
    
    fftw_plan fft = fftw_plan_dft_2d(height, width, in1, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(fft);
    
    double* myLogAmplitude = get_log_amplitude(out, width, height);
    double* myPhase = get_phase_angle(out, width, height);
    double* img_avg_filtered = mean_filter(myLogAmplitude, width, height);
    
    double mySpectralResidual;
    for (i = 0; i < wh; ++i) {
        mySpectralResidual = myLogAmplitude[i] - img_avg_filtered[i];
        in2[i][0] = exp(mySpectralResidual)*cos(myPhase[i]); 
        in2[i][1] = exp(mySpectralResidual)*sin(myPhase[i]);
    }
    
    fftw_plan ifft = fftw_plan_dft_2d(height, width, in2, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(ifft);
    
    double tmp_saliency[wh];
    for (i = 0; i < wh; ++i) {
        tmp_saliency[i] = (out[i][0]*out[i][0] + out[i][1]*out[i][1])/wh/wh;
    }
    
    float* saliencyMap = gaussian_filter(tmp_saliency, width, height);
    
    float max = amax(saliencyMap, wh), min = amin(saliencyMap, wh);
    float dif = max - min;
    
    for (i = 0; i < wh; ++i) {
        saliencyMap[i] = (saliencyMap[i] - min) / dif;
    }
    
    free(myLogAmplitude);
    free(myPhase);
    free(img_avg_filtered);
    fftw_destroy_plan(fft);
    fftw_destroy_plan(ifft);
    fftw_free(in1);
    fftw_free(in2);
    fftw_free(out);

    return saliencyMap;
}

int count_segarea(int*seglable,int size,int lable){
    int lablesize=0,i=0;
    for(i=0;i<size;++i){
        if(seglable[i]==lable)
            lablesize++;
    }
    return lablesize;
}

int* segment_hue_histogram(int*seglable,image_hsv* imagehsv,int nr,int nc,int lablenum){
    int* seghistogram=NEWA(int,20*lablenum);
    int i=0,j=0,k=0,m=0;
    
    for(i=0;i<20*lablenum;++i){
        seghistogram[i]=0;    
    }
    
    for(i=0;i<nr;++i){
        for(j=0;j<nc;++j){
            for(k=0;k<lablenum;++k){
                if(seglable[i*nc+j]==k+1){
                    if(imagehsv->s[i*nc+j]>0.2||imagehsv->v[i*nc+j]>0.2){
                        m=imagehsv->h[i*nc+j]/18;
                        seghistogram[k*20+m]++;
                    }    
                }
            }           
        }
    }
    
    return seghistogram;
}

int* maxarea_segment(int*seglable,int size,int lablenum){
    int mlable=0,lablesize=-1;
    int* maxseg=NEWA(int,2);
    int i=0;
    for(i=0;i<lablenum;++i){
        if(lablesize<count_segarea(seglable,size,i+1)){
            lablesize=count_segarea(seglable,size,i+1);
            mlable=i+1;           
        }
    }
    maxseg[0]=mlable;
    maxseg[1]=lablesize;
    return maxseg;
}

//
ncut_seg ncut_main_seg(char* filename,int nr,int nc){
    int size[5]={0};
    int imagesize=nr*nc;
    int*seglable=NEWA(int,imagesize);
    int i=0,j=0;
    int initial_lablenum=5;
    int lablenum=initial_lablenum;
    ncut_seg nseg;
    int initial_seglable[nr*nc];
    int noise_lable=0;
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Can't open %s!\n", filename);
        exit(1);
    }
    for (i = 0; i < nr; ++i) {
        fscanf(fp, "%d", &initial_seglable[i*nc]);
        for (j = 1; j < nc; ++j) {
            fscanf(fp, ",%d", &initial_seglable[i*nc+j]);
        }
    }
    fclose(fp);
    for(i=0;i<imagesize;++i){
        seglable[i]=initial_seglable[i];
    }
    
    for(i=0;i<initial_lablenum;++i){
        size[i]=count_segarea(initial_seglable,imagesize,i+1);
        if(size[i]<5*imagesize/100){
            lablenum=lablenum-1;
            for(j=0;j<imagesize;++j){
                if(initial_seglable[j]==i+1){
                    noise_lable=seglable[j];
                    seglable[j]=0;
                }
            }
            
            for(j=0;j<imagesize;++j){
                if(seglable[j]>noise_lable)
                    seglable[j]=seglable[j]-1;
            }
        }
    }
    
    nseg.nc=nc;
    nseg.nr=nr;
    nseg.lablenum=lablenum;
    nseg.seglable=seglable;   
    return nseg;
}