
#include <math.h>
#include <malloc.h>
#include <memory.h>

#include "tools.h"
#include "feature.h"

brightness* get_brightness_feature(image_hsl* hsl) {
    float mean, stdev, max, min;
    int width = hsl->width, height = hsl->height;
    int len = width*height;
    brightness* feature = NEW(brightness);
    
    mean = amean(hsl->l, len);
    stdev = get_stdev(hsl->l, len);
    max = amax(hsl->l, len);
    min = amin(hsl->l, len);
    
    feature->mean = mean;
    feature->stdev = stdev;
    feature->max = max;
    feature->min = min;
    
    return feature;
}

saturation* get_saturation_feature(image_hsl* hsl) {
    float mean, stdev, max, min;
    int width = hsl->width, height = hsl->height;
    int len = width*height;
    saturation* feature = NEW(saturation);
    
    mean = amean(hsl->s, len);
    stdev = get_stdev(hsl->s, len);
    max = amax(hsl->s, len);
    min = amin(hsl->s, len);
    
    feature->mean = mean;
    feature->stdev = stdev;
    feature->max = max;
    feature->min = min;
    
    return feature;
}

float get_colorfulness_feature(image_rgb* rgb) {
    int i;
    float u_rg, u_yb, stdev_rg, stdev_yb, result;
    int width = rgb->width, height = rgb->height;
    int len = width*height;
    
    float* rg, * yb;
    
    rg = NEWA(float, len);
    yb = NEWA(float, len);
    
    for (i = 0; i < len; ++i) {
        rg[i] = rgb->r[i] - rgb->g[i];
        yb[i] = (rgb->r[i] + rgb->g[i])/2 - rgb->b[i];
    }
    
    u_rg = amean(rg, len);
    u_yb = amean(yb, len);
    stdev_rg = get_stdev(rg, len);
    stdev_yb = get_stdev(yb, len);
    
    result = sqrt(stdev_rg*stdev_rg + stdev_yb*stdev_yb) + 0.3*sqrt(u_rg*u_rg + u_yb*u_yb);
    
    return result;
}

float get_naturalness_feature(image_hsl* hsl) {
    int i, j;
    int n_skin = 0, n_grass = 0, n_sky = 0;
    float Nskin, Ngrass, Nsky;
    float S_avg_skin = 0, S_avg_grass = 0, S_avg_sky = 0;
    float naturalness;
    int width = hsl->width, height = hsl->height;
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            if (hsl->s[i*width+j] > 0.1 && hsl->l[i*width+j] > 0.2 && hsl->l[i*width+j] < 0.8) {
                if (hsl->h[i*width+j] > 25.f/360 && hsl->h[i*width+j] < 70.f/360) {
                    ++n_skin;
                    S_avg_skin = (S_avg_skin*(n_skin-1) + hsl->s[i*width+j]) / n_skin;
                }
                else if (hsl->h[i*width+j] > 95.f/360 && hsl->h[i*width+j] < 135.f/360) {
                    ++n_grass;
                    S_avg_grass = (S_avg_grass*(n_grass-1) + hsl->s[i*width+j]) / n_grass;
                }
                else if (hsl->h[i*width+j] > 185.f/360 && hsl->h[i*width+j] < 260.f/360) {
                    ++n_sky;
                    S_avg_sky = (S_avg_sky*(n_sky-1) + hsl->s[i*width+j]) / n_sky;
                }
            }
        }
    }
    
    Nskin = exp(-0.5 * pow((S_avg_skin - 0.76)/0.52, 2));
    Ngrass = exp(-0.5 * pow((S_avg_grass - 0.81)/0.53, 2));
    Nsky = exp(-0.5 * pow((S_avg_sky - 0.43)/0.22, 2));
    
    naturalness = (Nskin * n_skin + Ngrass * n_grass + Nsky * n_sky) / (n_skin + n_grass + n_sky);
    
    return naturalness;
}

float get_contrast_feature(image_hsl* hsl) {
    int i;
    float stdev, max, min;
    int width = hsl->width, height = hsl->height;
    int len = width*height;
    
    float* norm_l = NEWA(float, len);
    
    max = amax(hsl->l, len);
    min = amin(hsl->l, len);
    
    for (i = 0; i < len; ++i) {
        norm_l[i] = (hsl->l[i] - min)/(max - min);
    }
    
    stdev = get_stdev(norm_l, len);
    
    return stdev;
}

float get_sharpness_feature(image_hsl* hsl) {
    int i, j, k, l;
    float sh = 0;
    int width = hsl->width, height = hsl->height;
    int len = width*height;
    
    float* lp = NEWA(float, len);
    float* u = NEWA(float, len);
    
    for (i = 1; i < height-1; ++i) {
        for (j = 1; j < width-1; ++j) {
            u[i*width+j] = 0;
            for (k = i-1; k < i+2; ++k) {
                for (l = j-1; l < j+2; ++l) {
                    u[i*width+j] += hsl->l[k*width+l];
                }
            }
            u[i*width+j] /= 9;
			lp[i*width+j] = hsl->l[(i+1)*width+j] + hsl->l[(i-1)*width+j] + hsl->l[i*width+j+1] + hsl->l[i*width+j-1] - 4*hsl->l[i*width+j];
        }
    }
    
    for (i = 1; i < height-1; ++i) {
        for (j = 1; j < width-1; ++j) {
            sh += lp[i*width+j] / u[i*width+j];
        }
    }
    
    free(lp);
    free(u);
    return sh;
}

gray_simplicity* get_grayscale_simplicity_feature(image_gray* gray) {
    int i;
    int width = gray->width, height = gray->height;
    int wh = width * height;
    int contrast = 0, sig_pixels_num;
    float stdev;
    int lower_bound, upper_bound;
    int add_up = 0;
    
    int* histogram = get_gray_histogram(gray);
    gray_simplicity* feature = NEW(gray_simplicity);
    
    // get the first feature
    lower_bound = (int)(0.025 * wh);
    upper_bound = (int)(0.975 * wh + 0.5);
    
    for (i = 0; i < 256; ++i) {
        add_up += histogram[i];
        if (add_up > lower_bound) {
            // if the add_up is closer to the lower bound this iteration than last
            if ((add_up - lower_bound) < (lower_bound - (add_up - histogram[i]))) {
                ++i;
            }
            else {
                add_up -= histogram[i];
            }
            break;
        }
    }
    
    for (; i < 256; ++i) {
        ++contrast;
        add_up += histogram[i];
        if (add_up >= upper_bound) {
            // if the add_up is further to the upper bound this iteration than last
            if ((add_up - upper_bound) > (upper_bound - (add_up - histogram[i]))) {
                --contrast;
            }
            break;
        }
    }
    
    // sort the histogram
    isort(histogram, NULL, 256);
    
    // get the second feature
    lower_bound = VALVE * histogram[0];
    sig_pixels_num = 1;
    for (i = 1; i < 256; ++i) {
        if (histogram[i] > VALVE * histogram[0]) 
            ++sig_pixels_num;
        else
            break;
    }
    
    // get the third feature
    float tmp[wh]; // save the histogram for calculate standard deviation
    for (i = 0; i < wh; ++i) {
        tmp[i] = gray->data[i];
    }
    stdev = get_stdev(tmp, wh);
    
    feature->contrast = contrast;
    feature->sig_pixels_num = sig_pixels_num;
    feature->stdev = stdev;
    
    free(histogram);
    
    return feature;
}

rgb_simplicity* get_rgb_simplicity_feature(image_rgb* rgb) {
    int i;
    int width = rgb->width, height = rgb->height;
    int wh = width * height;
    int sig_pixels_num;
    float ratio;
    int lower_bound;
    
    int* histogram = get_rgb_histogram(rgb);
    rgb_simplicity* feature = NEW(rgb_simplicity);
    
    // sort the histogram
    isort(histogram, NULL, 512);

    lower_bound = (int)(VALVE * histogram[0]);
    sig_pixels_num = 1;
    for (i = 1; i < 512; ++i) {
        if (histogram[i] > lower_bound) {
            ++sig_pixels_num;
        }
        else {
            break;
        }
    }
    
    ratio = (float)histogram[0] / wh; // the ratio of the dominant color
    
    feature->sig_pixels_num = sig_pixels_num;
    feature->ratio = ratio;
    
    free(histogram);
    
    return feature;
}

hsv_simplicity* get_hsv_simplicity_feature(image_hsv* hsv) {
    int i;
    int width = hsv->width, height = hsv->height;
    int wh = width * height;
    int sig_pixels_num;
    float ratio;
    int lower_bound;
    
    int* histogram = get_hsv_histogram(hsv);
    hsv_simplicity* feature = NEW(hsv_simplicity);
    
    // sort the histogram
    isort(histogram, NULL, 512);

    lower_bound = (int)(VALVE * histogram[0]);
    sig_pixels_num = 1;
    for (i = 1; i < 512; ++i) {
        if (histogram[i] > lower_bound) {
            ++sig_pixels_num;
        }
        else {
            break;
        }
    }
    
    ratio = (float)histogram[0] / wh; // the ratio of the dominant color
    
    feature->sig_pixels_num = sig_pixels_num;
    feature->ratio = ratio;
    
    free(histogram);
    
    return feature;
}

color_harmony* get_color_harmony_feature(image_hsl* hsl) {
    int i, j, k;
    float delta_alpha;
    int width = hsl->width, height = hsl->height;
    color_harmony* feature = NEW(color_harmony);
    
    float temp_d[2];
    float gamma[7] = {3E34, 3E34, 3E34, 3E34, 3E34, 3E34, 3E34};
    float temp_gamma[7] = {0}, dis;
    float alpha[7] = {0};
    float templates_origin[7][4] = {
									{0.725, 0.775, 0,     0},
                                    {0.62,  0.88,  0,     0}, 
                                    {0.725, 0.775, 0.89,  0.11},
                                    {0.725, 0.775, 0.225, 0.275},
                                    {0.75,  0,     0,     0.25},
                                    {0.62,  0.88,  0.225, 0.275},
                                    {0.12,  0.38,  0.62,  0.88}
								   };
	float templates[7][4];
    int type[7] = {0, 0, 1, 1, 1, 1, 1};
    
	
	for (delta_alpha = 0; delta_alpha < 1; delta_alpha += 0.05) {
		
		// the templates roll an angle
		for (i = 0; i < 7; ++i) {
			temp_gamma[i] = 0;
			for (j = 0; j < 4; ++j) {
				templates[i][j] = + templates_origin[i][j] + delta_alpha;
				if (templates[i][j] > 1) {
					templates[i][j] -= 1;
				}
			}
		}
		
		for (i = 0; i < height; ++i) {
			for (j = 0; j < width; ++j) {
                for (k = 0; k < 7; ++k) { // with 7 templates
                    temp_d[0] = get_arc_length(templates[k][0], hsl->h[i*width+j]);
                    temp_d[1] = get_arc_length(templates[k][1], hsl->h[i*width+j]);
                    if (fabs((temp_d[0] + temp_d[1]) - get_arc_length(templates[k][0], templates[k][1])) < 0.0001) {
                        dis = 0;
                    }
                    else if (temp_d[0] < temp_d[1]) {
                        dis = temp_d[0];
                    }
                    else {
                        dis = temp_d[1];
                    }
                    // if this template has more than one sector
                    if (type[k]) {
                        temp_d[0] = get_arc_length(templates[k][2], hsl->h[i*width+j]);
                        temp_d[1] = get_arc_length(templates[k][3], hsl->h[i*width+j]);
                        if (fabs((temp_d[0] + temp_d[1]) - get_arc_length(templates[k][2], templates[k][3])) < 0.0001) {
                            dis = 0;
                        }
                        else if (temp_d[0] < dis) {
                            dis = temp_d[0];
                        }
                        else if (temp_d[1] < dis) {
                            dis = temp_d[1];
                        }
                    }

                    temp_gamma[k] += dis * hsl->s[i*width+j];
                }
                
            }
        }
		
		// find out the best delta_alpha
		for (k = 0; k < 7; ++k) {
			if (temp_gamma[k] < gamma[k]) {
				alpha[k] = delta_alpha;
				gamma[k] = temp_gamma[k];
			}
		}
    }
    
	for (k = 0; k < 7; ++k) {
		if (alpha[k] > 0.5) {
			alpha[k] = 1 - alpha[k];
		}
	}
	
    fsort(alpha, 0, 7);
    
    feature->bestfit = alpha[6] * 2*PI;
    feature->fisrt_two_dev = fabs(alpha[5] - alpha[6]) * 2*PI;
    feature->avg_dev = amean(alpha, 7) * 2*PI;
    return feature;
}

hue_hist* get_hue_histogram_feature(image_hsv* hsv) {
    int i, j;
    int index[20];
    int width = hsv->width, height = hsv->height;
    int wh = width * height;
    int sig_pixels_num = 0;
    float stdev, arc_dis = 0, contrast = 0;
    
    int* histogram = get_hue_histogram(hsv);
    hue_hist* feature = NEW(hue_hist);
    
    isort(histogram, index, 20);
    // first feature
    for (i = 0; i < 20; ++i) {
        if (histogram[i] > VALVE * wh) 
            ++sig_pixels_num;
        else
            break;
    }
    // calculate the contrast
    for (i = 0; i < sig_pixels_num - 1; ++i) {
        for (j = i + 1; j < sig_pixels_num; ++j) {
            arc_dis = fabs(index[i] - index[j]) * 2 * PI / 20;
            if (arc_dis > PI) 
                arc_dis = 2*PI - arc_dis;
            if (arc_dis > contrast)
                contrast = arc_dis;
        }
    }
    
    float tmp[wh];
    for (i = 0; i < wh; ++i) {
        tmp[i] = hsv->h[i] / 0.05;
        if (tmp[i] > 10) {
            tmp[i] = 20 - tmp[i];
        }
    }
    
    stdev = get_stdev(tmp, wh); 
    stdev *= PI/10;                 // change to arc scale
    
    feature->sig_pixels_num = sig_pixels_num;
    feature->contrast = contrast;
    feature->stdev = stdev;
    
    free(histogram);
    
    return feature;
}

color_coherence* get_color_coherence_feature(image_hsv* hsv) {
    int i, j, k;
    int width = hsv->width, height = hsv->height;
    int wh = width * height;
    int direction[8][2]={{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
    int tmp_connected;
    int connected[512] = {0};

    typedef struct {
        int x;
        int y;
    } Point;
    
    
    color_coherence* feature = NEW(color_coherence);
    
    int descrized[wh];
    for (i = 0; i < wh; ++i) {
        descrized[i] = (int)(hsv->h[i] / 0.125)*64 + (int)(hsv->s[i] / 0.125)*8 + (int)(hsv->v[i] / 0.125);
        if (descrized[i] > 511)
            descrized[i] = 511;
    }
    
    unsigned char flag[wh];
    memset(flag, 0, wh*sizeof(unsigned char));

    //栈申请，此处假定进栈的像素最多为图像总像素数
    Point stack[wh];
    //栈顶指针
    int top;
    //当前正处理的点和弹出的点
    Point currentPoint, popPoint;
    //临时变量
    int temp1, temp2;
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            // did not go through it before
            if (!flag[i*width+j]) {
                tmp_connected = 1;
                //记录种子像素的灰度值
                temp1 = descrized[i*width+j];
                //将给定种子点置标记1,入栈
                flag[i*width+j] = 1;
                top=0;
                stack[top].x=i;
                stack[top].y=j;

                //堆栈
                while(top>-1){
                    //弹出栈顶元素,该元素已经生长过
                    popPoint.x = stack[top].x;
                    popPoint.y = stack[top].y;
                    top--;

                    //考察弹出像素周围是否有没有生长的像素
                    for(k=0; k<8; k++){
                        //待考察的邻域点
                        currentPoint.x=popPoint.x+direction[k][0];
                        currentPoint.y=popPoint.y+direction[k][1];

                        //如果待考察的点不在图像内，则跳过
                        if(currentPoint.x < 0 || currentPoint.x > height-1||
                            currentPoint.y < 0 || currentPoint.y > width-1)
                            continue;

                        //弹出的点周围有尚没生长的点
                        if(!flag[currentPoint.x*width+currentPoint.y]){
                            temp2 = descrized[currentPoint.x*width+currentPoint.y];
                            if(temp1 == temp2) {
                                ++tmp_connected;
                                //给该点置生长标记0
                                flag[currentPoint.x*width+currentPoint.y] = 1;
                                top++;
                                stack[top].x=currentPoint.x;
                                stack[top].y=currentPoint.y;
                            }
                        }
                    }

                }
                if (tmp_connected > VALVE*wh) {
                    connected[temp1] += tmp_connected;
                }
            }
        }
    }
    
    int n_ccc = 0;
    for (i = 0; i < 512; ++i) 
        if (connected[i] > 0)
            ++n_ccc;
    
    feature->n_ccc = n_ccc;
    
    int index[512];
    isort(connected, index, 512);
    
    feature->r_lc = (float)connected[0] / wh;
    feature->r_slc = (float)connected[1] / wh;
    // get the corresponding Hue rank by divide the bin number by 64
    feature->rank = index[0] / 64;
    feature->s_rank = index[1] / 64;
    
    return feature;
}