
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "tools.h"
//#include "image.h"
#include "feature.h"


int main(int argc, char** argv) {

    int i, count;    
    char** filenames;
	char id[50];
    FILE* fpout;

    if (argc != 3) {
        printf("Please input the path to the pictures & the output file name!\n");
		exit(1);
    }
	
	// define those features
	fbrightness brightness;
	fsaturation saturation;
	float colorfulness;
	float naturalness;
	float contrast;
	float sharpness;
	fgray_simplicity gray_simplicity;
	frgb_simplicity rgb_simplicity;
	fhsv_simplicity hsv_simplicity;
	fhue_hist hue_hist;
	fcolor_harmony color_harmony;
	fcolor_coherence color_coherence;
	fsegment_lightness segment_lightness;
	fSegment_size segment_size;
	fSegment_hues segment_hues;
    fcolor_harmony segment_color_harmony;
	fsaliency_map saliency_map;
	int n_sift;
	int n_faces;	
	
	// point to images
	IplImage* opencv_img;
	image_rgb* rgb;
	image_hsl* hsl;
	image_hsv* hsv;
	image_gray* gray;
    filenames = get_files_list(argv[1], &count);
    fpout = fopen(argv[2], "w");
	
	CvHaarClassifierCascade* classifier = (CvHaarClassifierCascade*)cvLoad("haarcascade_frontalface_alt_tree.xml", 0, 0, 0);
	for (i = 0; i < count; ++i) {
        if ((i % 100) == 0) {
            printf(" - progress: %6.2f%%\n", 100*(float)i/count);
        }
		// load the image
		opencv_img = cvLoadImage(filenames[i], 1);
		rgb = load_cv_image(opencv_img);
		hsl = rgb2hsl(rgb);
		hsv = rgb2hsv(rgb);
		gray = rgb2gray(rgb);
		
		// compute those features
		brightness = get_brightness_feature(hsl);
		saturation = get_saturation_feature(hsl);
		colorfulness = get_colorfulness_feature(rgb);
		naturalness = get_naturalness_feature(hsl);
		contrast = get_contrast_feature(hsl);
		sharpness = get_sharpness_feature(hsl);
		gray_simplicity = get_grayscale_simplicity_feature(gray);
		rgb_simplicity = get_rgb_simplicity_feature(rgb);
		hsv_simplicity = get_hsv_simplicity_feature(hsv);
		hue_hist = get_hue_histogram_feature(hsv);
		color_harmony = get_color_harmony_feature(hsl);
		color_coherence = get_color_coherence_feature(hsv);
		//segment_lightness;
		//segment_size;
		//segment_hues;
		
		float* saliency = get_saliency_map(rgb);
		saliency_map = get_saliency_map_feature(saliency, rgb->width, rgb->height);
		n_sift = get_sift_number(opencv_img);	
		n_faces = get_face_feature(opencv_img, classifier, cvSize(10, 20));
		
		// free those space
		cvReleaseImage(&opencv_img);
		image_rgb_delete(rgb);
		image_hsl_delete(hsl);
		image_hsv_delete(hsv);
		image_gray_delete(gray);
		free(saliency);
		
		// output
		strncpy(id, filenames[i]+4, strlen(filenames[i])-8);
		id[strlen(filenames[i])-8] = '\0';
		
		fprintf(fpout, "%s,", id); // the filename of the image without suffix
		fprintf(fpout, "%f,%f,%f,%f,", brightness.mean, brightness.stdev, brightness.max, brightness.min); // brightness
		fprintf(fpout, "%f,%f,%f,%f,", saturation.mean, saturation.stdev, saturation.max, saturation.min); // saturation
		fprintf(fpout, "%f,%f,%f,%f,", colorfulness, naturalness, contrast, sharpness);	
		fprintf(fpout, "%d,%d,%f,", gray_simplicity.contrast, gray_simplicity.sig_pixels_num, gray_simplicity.stdev); // gray simplicity
		fprintf(fpout, "%d,%f,", rgb_simplicity.sig_pixels_num, rgb_simplicity.ratio); // RGB simplicity
		fprintf(fpout, "%d,%f,", hsv_simplicity.sig_pixels_num, hsv_simplicity.ratio); // HSV simplicity
		fprintf(fpout, "%d,%f,%f,", hue_hist.sig_pixels_num, hue_hist.contrast, hue_hist.stdev); // Hue histogram
		fprintf(fpout, "%f,%f,%f,", color_harmony.bestfit, color_harmony.first_two_dev, color_harmony.avg_dev); // color harmony
		fprintf(fpout, "%d,%f,%f,%d,%d,", color_coherence.n_ccc, color_coherence.r_lc, color_coherence.r_slc, color_coherence.rank, color_coherence.s_rank); // color coherence
		///////////////////////////////////////
        /* to be filled in */
        ///////////////////////////////////////        
        fprintf(fpout, "%f,%d,%f,%f,%d,%f,%f,%f,%f,", saliency_map.r_bg, saliency_map.n_cc, saliency_map.r_lc, saliency_map.asv_lc, saliency_map.n_cc_bg, saliency_map.r_lc_bg, saliency_map.d_cc, saliency_map.d_rtp, saliency_map.d_ci);
		fprintf(fpout, "%d,", n_sift);	
		fprintf(fpout, "%d\n", n_faces);
	}
    fclose(fpout);

    printf(" - progress: 100.00%%\n");
    
    return (EXIT_SUCCESS);
}

