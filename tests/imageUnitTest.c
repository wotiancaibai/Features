/*
 * File:   imageUnitTest.c
 * Author: slee
 *
 * Created on Aug 26, 2012, 12:08:54 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "image.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void testRgb2gray() {
    image_rgb* rgb = load_image("tests/test.jpg", 1);
    image_gray *gray = rgb2gray(rgb);
    
    CU_ASSERT_EQUAL(gray->data[0], 3);
    CU_ASSERT_EQUAL(gray->data[1], 5);
    CU_ASSERT_EQUAL(gray->data[2], 7);
    CU_ASSERT_EQUAL(gray->data[3], 3);
    CU_ASSERT_EQUAL(gray->data[4], 5);
    CU_ASSERT_EQUAL(gray->data[5], 7);
    CU_ASSERT_EQUAL(gray->data[6], 3);
    CU_ASSERT_EQUAL(gray->data[7], 5);
    CU_ASSERT_EQUAL(gray->data[8], 7);
    
    image_rgb_delete(rgb);
    image_gray_delete(gray);
}

void testRgb2hsl() {
    image_rgb* rgb = load_image("tests/test.jpg", 1);
    image_hsl* hsl = rgb2hsl(rgb);
    image_rgb* rgb2 = image_rgb_new(2, 2);
    image_rgb* rgb3 = load_image("tests/test.bmp", 1);
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            CU_ASSERT_EQUAL(rgb->r[i*3+j], rgb3->r[i*3+j]);
            CU_ASSERT_EQUAL(rgb->g[i*3+j], rgb3->g[i*3+j]);
            CU_ASSERT_EQUAL(rgb->b[i*3+j], rgb3->b[i*3+j]);
        }
    }
    rgb2->r[0] = 255;
    rgb2->r[1] = 220;
    rgb2->r[2] = 100;
    rgb2->r[3] = 3;
    rgb2->g[0] = 245;
    rgb2->g[1] = 200;
    rgb2->g[2] = 155;
    rgb2->g[3] = 3;
    rgb2->b[0] = 2;
    rgb2->b[1] = 200;
    rgb2->b[2] = 100;
    rgb2->b[3] = 3;
    image_hsl* hsl2 = rgb2hsl(rgb2);
    
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[0], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[1], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[2], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[3], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[4], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[5], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[6], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[7], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->h[8], 0.1111, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->l[4], 0.01765, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->l[5], 0.02549, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->s[6], 0.60000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl->s[7], 0.33333, 0.0001);
    
    
    CU_ASSERT_DOUBLE_EQUAL(hsl2->h[0], 0.16008, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->h[1], 0.00000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->h[2], 0.33333, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->h[3], 0.00000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->s[0], 1.00000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->s[1], 0.22222, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->s[2], 0.21569, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->s[3], 0.00000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->l[0], 0.50392, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->l[1], 0.82353, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->l[2], 0.50000, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsl2->l[3], 0.01176, 0.0001);
    
    image_rgb_delete(rgb);
    image_rgb_delete(rgb2);
    image_rgb_delete(rgb3);
    image_hsl_delete(hsl);
    image_hsl_delete(hsl2);
}

void testRgb2hsv() {
    image_rgb* rgb = load_image("tests/r2.jpg", 1);
    image_hsv* hsv = rgb2hsv(rgb);
    int width = hsv->width;
    
    CU_ASSERT_DOUBLE_EQUAL(hsv->h[35*width + 15], 0.54808, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->h[73*width + 58], 0.61667, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->h[92*width + 99], 0.56250, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->h[23*width + 70], 0.56667, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->s[35*width + 15], 0.99048, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->s[73*width + 58], 0.04202, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->s[92*width + 99], 0.41558, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->s[23*width + 70], 0.12658, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->v[35*width + 15], 0.82353, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->v[73*width + 58], 0.93333, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->v[92*width + 99], 0.90588, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(hsv->v[23*width + 70], 0.92941, 0.0001);
    image_hsv_delete(hsv);
}

void get_histogram_test() { // gray_histogram & rgb_histogram & hue_histogram
    image_gray* gray = image_gray_new(2, 2);
    int* gray_hist;
    gray->data[0] = 1;
    gray->data[1] = 25;
    gray->data[2] = 25;
    gray->data[3] = 3;
    
    gray_hist = get_gray_histogram(gray);
    CU_ASSERT_EQUAL(gray_hist[1], 1);
    CU_ASSERT_EQUAL(gray_hist[25], 2);
    CU_ASSERT_EQUAL(gray_hist[3], 1);
    
    image_rgb* rgb = image_rgb_new(2, 2);
    int* rgb_hist;
    rgb->r[0] = 255;
    rgb->r[1] = 200;
    rgb->r[2] = 100;
    rgb->r[3] = 3;
    rgb->g[0] = 255;
    rgb->g[1] = 200;
    rgb->g[2] = 100;
    rgb->g[3] = 3;
    rgb->b[0] = 255;
    rgb->b[1] = 200;
    rgb->b[2] = 100;
    rgb->b[3] = 3;
    
    rgb_hist = get_rgb_histogram(rgb);
    
    CU_ASSERT_EQUAL(rgb_hist[0], 1);
    CU_ASSERT_EQUAL(rgb_hist[219], 1);
    CU_ASSERT_EQUAL(rgb_hist[438], 1);
    CU_ASSERT_EQUAL(rgb_hist[511], 1);
    
    image_hsv* hsv = image_hsv_new(2, 3);
    hsv->h[0] = 0.51;
    hsv->h[1] = 0.3;
    hsv->h[2] = 0.2;
    hsv->h[3] = 0.81;
    hsv->h[4] = 0.2;
    hsv->h[5] = 0.82;
    hsv->s[0] = 0.51;
    hsv->s[1] = 0.3;
    hsv->s[2] = 0.3;
    hsv->s[3] = 0.81;
    hsv->s[4] = 0.3;
    hsv->s[5] = 0.82;
    hsv->v[0] = 0.51;
    hsv->v[1] = 0.3;
    hsv->v[2] = 0.3;
    hsv->v[3] = 0.81;
    hsv->v[4] = 0.3;
    hsv->v[5] = 0.82;
    
    int *h_hist = get_hue_histogram(hsv);
    CU_ASSERT_EQUAL(h_hist[4], 2);
    CU_ASSERT_EQUAL(h_hist[6], 1);
    CU_ASSERT_EQUAL(h_hist[10], 1);
    CU_ASSERT_EQUAL(h_hist[16], 2);
    
    image_rgb_delete(rgb);
    image_gray_delete(gray);
    image_hsv_delete(hsv);
    free(gray_hist);
    free(rgb_hist);
    free(h_hist);
    
}

void get_saliency_map_test() {
    image_rgb* rgb = load_image("tests/2.jpg", 1);
    int i, j;
    int width = rgb->width, height = rgb->height;
    FILE *fp = fopen("tests/saliency_map.txt", "w");
    
    float *saliencyMap = get_saliency_map(rgb);
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            fprintf(fp, "%f\t", saliencyMap[i*width+j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

//normalized cut related
void count_segarea_test(){
    int size=100;
    int*seglable=NEWA(int,size);
    int lablenum=5;
    int i=0;
    int maxsegsize=0;
    int segsize[5]={20,20,30,20,10};
    
    for(i=0;i<size;++i){
        if(i<segsize[0])
            seglable[i]=1;
        else if(i<segsize[0]+segsize[1])
            seglable[i]=2;
        else if(i<segsize[0]+segsize[1]+segsize[2])
            seglable[i]=3;
        else if(i<segsize[0]+segsize[1]+segsize[2]+segsize[3])
            seglable[i]=4;
        else
            seglable[i]=5;       
    }
    for(i=0;i<5;++i){
        maxsegsize=count_segarea(seglable,size,i+1);
        CU_ASSERT_EQUAL(maxsegsize, segsize[i]);
    }
    
    free(seglable);
}

void segment_hue_histogram_test(){
    int nr=30,nc=30;
    int*seglable=NEWA(int,nr*nc);
    image_hsv* imagehsv=image_hsv_new(nr,nc);
    int lablenum=5;
    int* seghistogram=NEWA(int,20*lablenum);
    int i=0;
    int j=0;
    for(i=0;i<nr;++i){
        for(j=0;j<nc;++j){
            if(j<10)
                seglable[i*nc+j]=i/10+1;
            else
                seglable[i*nc+j]=i/10+1+3;
            if(seglable[i*nc+j]>5)
                seglable[i*nc+j]=3;            
        }
    }
    
    for(i=0;i<nr;++i){
        for(j=0;j<nc;++j){
            if(i<10&&j<20){             
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0); 
            }
            else if(i<10&&j<30){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+18.0;
            }
            
            if(i>=10&&i<20&&j<3){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0);
            }
            else if(i>=10&&i<20&&j<6){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+18.0;
            }
            else if(i>=10&&i<20&&j<10){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+36.0;
            }
            else if(i>=10&&i<20&&j<15){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0);
            }
            else if(i>=10&&i<20&&j<20){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+18.0;
            }
            else if(i>=10&&i<20&&j<25){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+36.0;
            }
            else if(i>=10&&i<20&&j<30){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+54.0;
            }
            
            if(i>=20&&i<30&&j<20){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+j*18.0;
            }
            else if(i>=20&&i<30&&j<30){
                imagehsv->h[i*nc+j]=18.0*rand()/(RAND_MAX+1.0)+19*18.0;
            }
            
            if(i%10<5){      
                imagehsv->s[i*nc+j]=0.2*rand()/(RAND_MAX+1.0);
                imagehsv->v[i*nc+j]=0.2*rand()/(RAND_MAX+1.0);                
            }
            else{
                imagehsv->s[i*nc+j]=0.2*rand()/(RAND_MAX+1.0)+0.2;
                imagehsv->v[i*nc+j]=0.2*rand()/(RAND_MAX+1.0)+0.1;
            }          
          }         
    }
   
    seghistogram=segment_hue_histogram(seglable,imagehsv,nr,nc,lablenum);
       
    for(i=0;i<20;++i){
        if(i==0){
            CU_ASSERT_EQUAL(seghistogram[i], 50);
        }
        else{
            CU_ASSERT_EQUAL(seghistogram[i],0);
        }
    }
    
    for(i=0;i<20;++i){
        if(i<2){
            CU_ASSERT_EQUAL(seghistogram[20+i],15);
        }
        else if(i==2){
            CU_ASSERT_EQUAL(seghistogram[20+i],20); 
        }
        else{
            CU_ASSERT_EQUAL(seghistogram[20+i],0);
        }
    }
    
    for(i=0;i<20;++i){
        if(i<19){
           CU_ASSERT_EQUAL(seghistogram[40+i],5);
        }
        else{
            CU_ASSERT_EQUAL(seghistogram[40+i],55);
        }           
    }
    
    for(i=0;i<20;++i){
        if(i<2){
            CU_ASSERT_EQUAL(seghistogram[60+i],50);
        }
        else{
            CU_ASSERT_EQUAL(seghistogram[60+i],0); 
        }
    }
    
    for(i=0;i<20;++i){
        if(i<4){
           CU_ASSERT_EQUAL(seghistogram[80+i],25);
        }
        else{
            CU_ASSERT_EQUAL(seghistogram[80+i],0);
        }
            
    }
    
    image_hsv_delete(imagehsv);
    free(seghistogram);
    free(seglable);
}

void maxarea_segment_test(){
    int size=100;
    int*seglable=NEWA(int,size);
    int lablenum=5;
    int* maxseg=NEWA(int,2);
    int i=0;
    
    for(i=0;i<size;++i){
        if(i<30)
            seglable[i]=3;
        else if(i<50)
            seglable[i]=2;
        else if(i<70)
            seglable[i]=1;
        else if(i<90)
            seglable[i]=4;
        else
            seglable[i]=5;       
    }
    
    maxseg=maxarea_segment(seglable,size,lablenum);
    CU_ASSERT_EQUAL(maxseg[0], 3);
    CU_ASSERT_EQUAL(maxseg[1], 30);
    free(seglable);
    free(maxseg);
}

void ncut_main_seg_test(){
    int nr=10,nc=10;
    int* init_seglable=NEWA(int,nr*nc);
   // int init_lablenum=5;
    int i=0;
    int segsize[5]={20,1,4,5,70};
    ncut_seg seg;
    
    for(i=0;i<100;++i){
        if(i<segsize[0]){
           init_seglable[i]=1;
        }
        else if(i<segsize[0]+segsize[1]){
            init_seglable[i]=2;
        }
        else if(i<segsize[0]+segsize[1]+segsize[2]){
            init_seglable[i]=3;
        }
        else if(i<segsize[0]+segsize[1]+segsize[2]+segsize[3]){
            init_seglable[i]=4;
        }        
        else{
            init_seglable[i]=5;
        }
    }
    
    seg=ncut_main_seg(init_seglable,nr,nc);
    CU_ASSERT_EQUAL(seg.nc,nc);
    CU_ASSERT_EQUAL(seg.nr,nr);
    CU_ASSERT_EQUAL(seg.lablenum,3);
    
    for(i=0;i<100;++i){
        if(i<segsize[0]){
           CU_ASSERT_EQUAL(seg.seglable[i],1);
        }
        else if(i<segsize[0]+segsize[1]){
            CU_ASSERT_EQUAL(seg.seglable[i],0);
        }
        else if(i<segsize[0]+segsize[1]+segsize[2]){
            CU_ASSERT_EQUAL(seg.seglable[i],0);
        }
        else if(i<segsize[0]+segsize[1]+segsize[2]+segsize[3]){
            CU_ASSERT_EQUAL(seg.seglable[i],4);
        }        
        else{
            CU_ASSERT_EQUAL(seg.seglable[i],5);
        }
    }
    free(init_seglable);
    free(seg.seglable);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Image Unit Test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testRgb2gray", testRgb2gray)) ||
        (NULL == CU_add_test(pSuite, "testRgb2hsl", testRgb2hsl)) ||
        (NULL == CU_add_test(pSuite, "testRgb2hsv", testRgb2hsv)) ||
        (NULL == CU_add_test(pSuite, "get_histogram_test", get_histogram_test)) ||
        (NULL == CU_add_test(pSuite, "get_saliency_map_test", get_saliency_map_test))||
        (NULL == CU_add_test(pSuite, "count_segarea_test", count_segarea_test))||
        (NULL == CU_add_test(pSuite, "segment_hue_histogram_test", segment_hue_histogram_test))||
        (NULL == CU_add_test(pSuite, "maxarea_segment_test", maxarea_segment_test))||  
        (NULL == CU_add_test(pSuite, "ncut_main_seg_test", ncut_main_seg_test))
       ){
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
