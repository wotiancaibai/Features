
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "tools.h"
#include "feature.h"


int main(int argc, char** argv) {

    int i, count;    
    char** filenames;
	char id[50] = {0};
    char seg_file_name[50] = {'s', 'e', 'g', '/', 's', 'e', 'g', '\0'};
    char suffix[5] = {'.', 't', 'x', 't', '\0'};
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
        // test
/*
        if ((i+1)%50==0)
            break;
*/
		// load the image
		opencv_img = cvLoadImage(filenames[i], 1);
		rgb = load_cv_image(opencv_img);
		hsl = rgb2hsl(rgb);
		hsv = rgb2hsv(rgb);
		gray = rgb2gray(rgb);
		
        // tune the filename
        strncpy(id, filenames[i]+4, strlen(filenames[i])-8);
		id[strlen(filenames[i])-8] = '\0';
        strncpy(seg_file_name+7, id, strlen(id));
        strcpy(seg_file_name+7+strlen(id), suffix);
        //printf("%s\n", seg_file_name);

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
        
        // KGL
        ncut_seg seg_arg = ncut_main_seg(seg_file_name, rgb->height, rgb->width);
		segment_size = get_segsize_feature(seg_arg.seglable, seg_arg.nr, seg_arg.nc, seg_arg.lablenum);
        segment_hues = get_seghues_feature(seg_arg.seglable, hsv, seg_arg.nr, seg_arg.nc, seg_arg.lablenum);
		segment_color_harmony = get_segcolor_harmony_feature(hsl, seg_arg.seglable, seg_arg.lablenum);
        segment_lightness = get_seglightness_feature(hsl, seg_arg.seglable, seg_arg.lablenum);
        free(seg_arg.seglable);
        // - KGL
        
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
		
        // KGL
        fprintf(fpout, "%f,%f,", segment_size.rmaxsize, segment_size.rcontrast); // Segment size
        fprintf(fpout, "%d,%d,%d,%d,%f,%f,", segment_hues.fhues1, segment_hues.fhues2, segment_hues.fhues3, segment_hues.fhues4, segment_hues.fhues5, segment_hues.fhues6); // Segment hues
		fprintf(fpout, "%f,%f,%f,", segment_color_harmony.bestfit, segment_color_harmony.first_two_dev, segment_color_harmony.avg_dev); // Segment color harmony
        fprintf(fpout, "%f,%f,%f,", segment_lightness.mseg_ave, segment_lightness.seg_ave_std, segment_lightness.seg_ave_contrast); // Segment lightness
        // - KGL
        
        fprintf(fpout, "%f,%d,%f,%f,%d,%f,%f,%f,%f,", saliency_map.r_bg, saliency_map.n_cc, saliency_map.r_lc, saliency_map.asv_lc, saliency_map.n_cc_bg, saliency_map.r_lc_bg, saliency_map.d_cc, saliency_map.d_rtp, saliency_map.d_ci);
		fprintf(fpout, "%d,", n_sift);	
		fprintf(fpout, "%d\n", n_faces);
	}
    fclose(fpout);
    cvReleaseHaarClassifierCascade( &classifier );
    free(filenames);
    
    printf(" - progress: 100.00%%\n");
    
    return (EXIT_SUCCESS);
}

