/*
 * File:   featureUnitTest.c
 * Author: slee
 *
 * Created on Aug 25, 2012, 9:16:33 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "feature.h"

/*
 * CUnit Test Suite
 */

int init_suite(void) {
    return 0;
}

int clean_suite(void) {
    return 0;
}

void testGet_brightness_feautre() {
    image_hsl* hsl = image_hsl_new(3, 2);
    brightness* feature;
    hsl->l[0] = 1;
    hsl->l[1] = 0.7;
    hsl->l[2] = 0.3;
    hsl->l[3] = 0.5;
    hsl->l[4] = 0.1;
    hsl->l[5] = 0.2;
    
    feature = get_brightness_feature(hsl);
    CU_ASSERT_DOUBLE_EQUAL(feature->max,       1.0, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(feature->mean,  0.46666, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(feature->min,       0.1, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->stdev, 0.30912, 0.00001);
	
	image_hsl_delete(hsl);
	free(feature);
}

void testGet_saturation_feautre() {
    image_hsl* hsl = image_hsl_new(3, 2);
	saturation *feature;
	hsl->s[0] = 1;
    hsl->s[1] = 0.7;
    hsl->s[2] = 0.3;
    hsl->s[3] = 0.5;
    hsl->s[4] = 0.1;
    hsl->s[5] = 0.2;

	feature = get_saturation_feature(hsl);
    CU_ASSERT_DOUBLE_EQUAL(feature->max,       1.0, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(feature->mean,  0.46666, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(feature->min,       0.1, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->stdev, 0.30912, 0.00001);
	
	image_hsl_delete(hsl);
	free(feature);
}

void testGet_colorfulness_feautre() {
    image_rgb* rgb = image_rgb_new(2, 2);
	rgb->r[0] = 35;
	rgb->r[1] = 25;
	rgb->r[2] = 135;
	rgb->r[3] = 235;	
	
	rgb->g[0] = 39;
	rgb->g[1] = 85;
	rgb->g[2] = 85;
	rgb->g[3] = 115;	

	rgb->b[0] = 135;
	rgb->b[1] = 125;
	rgb->b[2] = 45;
	rgb->b[3] = 5;	
	
	float feature = get_colorfulness_feature(rgb);
	CU_ASSERT_DOUBLE_EQUAL(feature, 136.10711, 0.00001);

	image_rgb_delete(rgb);

}

void testGet_naturalness_feautre() {
    image_hsl* hsl = image_hsl_new(3, 2);
	hsl->h[0] = 0.15;
	hsl->h[1] = 0.1;
	hsl->h[2] = 0.7;
	hsl->h[3] = 0.3;
	hsl->h[4] = 0.1;
	hsl->h[5] = 0.8;

	hsl->s[0] = 0.2;
	hsl->s[1] = 0.1;
	hsl->s[2] = 0.7;
	hsl->s[3] = 0.8;	
	hsl->s[4] = 0.3;
	hsl->s[5] = 0.5;
	
	hsl->l[0] = 0.24;
	hsl->l[1] = 0.1;
	hsl->l[2] = 0.7;
	hsl->l[3] = 0.73;
	hsl->l[4] = 0.6;
	hsl->l[5] = 0.5;

	float feature = get_naturalness_feature(hsl);
	CU_ASSERT_DOUBLE_EQUAL(feature, 0.67678, 0.00001);

	image_hsl_delete(hsl);
}

void testGet_contrast_feautre() {
    image_hsl* hsl = image_hsl_new(3, 3);
	
	hsl->l[0] = 0.5;
	hsl->l[1] = 0.3;
	hsl->l[2] = 0.6;
	hsl->l[3] = 0.9;
	hsl->l[4] = 0.1;
	hsl->l[5] = 0.05;	
	hsl->l[6] = 0.55;
	hsl->l[7] = 0.25;
	hsl->l[8] = 0.5;

	float feature = get_contrast_feature(hsl);
	CU_ASSERT_DOUBLE_EQUAL(feature, 0.29737, 0.00001);

	image_hsl_delete(hsl);
}

void testGet_sharpness_feautre() {
    image_hsl* hsl = image_hsl_new(4, 4);

	hsl->l[0] = 0.3;
	hsl->l[1] = 0.2;
	hsl->l[2] = 0.5;
	hsl->l[3] = 0.6;
	hsl->l[4] = 0.7;
	hsl->l[5] = 0.3;	
	hsl->l[6] = 0.5;
	hsl->l[7] = 0.4;
	hsl->l[8] = 0.1;	
	hsl->l[9] = 0.6;
	hsl->l[10] = 0.3;
	hsl->l[11] = 0.4;
	hsl->l[12] = 0.9;	
	hsl->l[13] = 0.5;
	hsl->l[14] = 0.2;
	hsl->l[15] = 0.1;	

	float feature = get_sharpness_feature(hsl);
	CU_ASSERT_DOUBLE_EQUAL(feature, -0.39758, 0.00001);

	image_hsl_delete(hsl);
}

void testGet_grayscale_simplicity_feautre() {
    image_gray* gray = image_gray_new(3, 3);
	
	gray->data[0] = 15;
	gray->data[1] = 15;
	gray->data[2] = 200;
	gray->data[3] = 180;
	gray->data[4] = 180;
	gray->data[5] = 100;
	gray->data[6] = 90;
	gray->data[7] = 85;
	gray->data[8] = 180;

	gray_simplicity* feature = get_grayscale_simplicity_feature(gray);
	CU_ASSERT_EQUAL(feature->contrast, 186);
	CU_ASSERT_EQUAL(feature->sig_pixels_num, 6);
	CU_ASSERT_DOUBLE_EQUAL(feature->stdev,  68.01870, 0.00001);

	free(feature);
    
    image_gray* gray2 = image_gray_new(1, 1);
    gray2->data[0] = 1;
    feature = get_grayscale_simplicity_feature(gray2);
    CU_ASSERT_EQUAL(feature->contrast, 1);
	CU_ASSERT_EQUAL(feature->sig_pixels_num, 1);
	CU_ASSERT_DOUBLE_EQUAL(feature->stdev,  0, 0.00001);
	image_gray_delete(gray);
}

void testGet_rgb_simplicity_feautre() {
    image_rgb* rgb = image_rgb_new(2, 2);

	rgb->r[0] = 35;
	rgb->r[1] = 25;
	rgb->r[2] = 35;
	rgb->r[3] = 235;	
	
	rgb->g[0] = 39;
	rgb->g[1] = 85;
	rgb->g[2] = 45;
	rgb->g[3] = 115;	

	rgb->b[0] = 135;
	rgb->b[1] = 125;
	rgb->b[2] = 145;
	rgb->b[3] = 5;		

	rgb_simplicity* feature = get_rgb_simplicity_feature(rgb);
	CU_ASSERT_EQUAL(feature->sig_pixels_num, 3);
	CU_ASSERT_DOUBLE_EQUAL(feature->ratio, 0.5, 0.00001);

	free(feature);
	image_rgb_delete(rgb);	
    

}

void testGet_color_harmony_feautre() {
    image_hsl* hsl1 = image_hsl_new(1, 1);
	color_harmony* feature;

	hsl1->h[0] = 0.39;
	hsl1->s[0] = 0.5;
	feature = get_color_harmony_feature(hsl1);	
	CU_ASSERT_DOUBLE_EQUAL(feature->bestfit, 0.1*2*PI*0.5, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->fisrt_two_dev, 0.2*2*PI*0.5, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->avg_dev, 1.436, 0.001);
	free(feature);

	image_hsl* hsl2 = image_hsl_new(1, 2);
	hsl2->h[0] = 0.10;
	hsl2->h[1] = 0.90;
	hsl2->s[0] = 1;
	hsl2->s[1] = 1;
	feature = get_color_harmony_feature(hsl2);
	CU_ASSERT_DOUBLE_EQUAL(feature->bestfit, 0, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->fisrt_two_dev, 0, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->avg_dev, 1.1220, 0.02);
	free(feature);
	
	image_hsl_delete(hsl1);
	image_hsl_delete(hsl2);
}

void testGet_hue_histogram_feature() {
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

	hue_hist* feature = get_hue_histogram_feature(hsv);
	CU_ASSERT_EQUAL(feature->sig_pixels_num, 4);
	CU_ASSERT_DOUBLE_EQUAL(feature->contrast, 3.14159, 0.00001);
	CU_ASSERT_DOUBLE_EQUAL(feature->stdev, 0.69305, 0.00001);
    
    free(h_hist);
    free(feature); 
    image_hsv_delete(hsv);
}

void testGet_color_coherence_feature() {
    image_hsv* hsv = image_hsv_new(6, 6);
    float data[][6] = {
                        {0.3, 0.2, 0.3, 0.3, 0.2, 0.2},
                        {0.3, 0.3, 0.2, 0.3, 0.2, 0.2},
                        {0.3, 0.2, 0.4, 0.3, 0.2, 0.2},
                        {0.3, 0.3, 0.3, 0.2, 0.2, 0.3},
                        {0.3, 0.3, 0.2, 0.2, 0.3, 0.3},
                        {0.3, 0.3, 0.2, 0.2, 0.3, 0.3}
                      };
    int i, j;
    for (i = 0; i < 6; ++i) {
        for (j = 0; j < 6; ++j) {
            hsv->h[6*i+j] = data[i][j];
            hsv->s[6*i+j] = data[i][j];
            hsv->v[6*i+j] = data[i][j];
        }
    }

    color_coherence* feature = get_color_coherence_feature(hsv);
    
    CU_ASSERT_EQUAL(feature->n_ccc, 3);
    CU_ASSERT_DOUBLE_EQUAL(feature->r_lc, 0.55556, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(feature->r_slc, 0.41667, 0.00001);
    CU_ASSERT_EQUAL(feature->rank, 2);
    CU_ASSERT_EQUAL(feature->s_rank, 1);
    free(feature);
}

int main() {
    CU_pSuite pSuite = NULL;

    /* Initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add a suite to the registry */
    pSuite = CU_add_suite("Feature Unit Test", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add the tests to the suite */
    if ((NULL == CU_add_test(pSuite, "testGet_brightness", testGet_brightness_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_color_harmony", testGet_color_harmony_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_colorfulness", testGet_colorfulness_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_gray_simplicity", testGet_grayscale_simplicity_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_hue_hist_feature", testGet_hue_histogram_feature)) ||
            (NULL == CU_add_test(pSuite, "testGet_naturalness", testGet_naturalness_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_rgb_simplicity", testGet_rgb_simplicity_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_saturation", testGet_saturation_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_sharpness", testGet_sharpness_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_contrast", testGet_contrast_feautre)) ||
            (NULL == CU_add_test(pSuite, "testGet_color_coherence_feature", testGet_color_coherence_feature))
		) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}


