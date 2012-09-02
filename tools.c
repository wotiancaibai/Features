#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>
#include "tools.h"

void list_files(char *path, char** filenames, int* index) {
    struct dirent* ent = NULL;
    DIR* pDir;
    char dir[512];
    struct stat statbuf;  
    if((pDir=opendir(path))==NULL) {
        fprintf( stderr, "Cannot open directory:%s\n", path );
        return;
    }
    while(ent=readdir(pDir))
    {
        //得到读取文件的绝对路径名
        snprintf( dir, 512,"%s/%s", path, ent->d_name );   
        //得到文件信息
        lstat(dir, &statbuf);
        //判断是目录还是文件
        if(S_ISDIR(statbuf.st_mode)) {
            //排除当前目录和上级目录
            if(strcmp( ".",ent->d_name) == 0 || strcmp( "..",ent->d_name) == 0)
                continue;
            //如果是子目录,递归调用函数本身,实现子目录中文件遍历
            //printf( "%*s子目录:%s/\n", indent, "", ent->d_name );
            //递归调用,遍历子目录中文件
            list_files(dir, filenames, index);               
        }
        else {
			filenames[*index] = (char *)malloc((strlen(dir)+1)*sizeof(char));
            strcpy(filenames[(*index)++], dir);
            //printf( "%*s文件:%s\n", indent, "", ent->d_name );
        }       
    }//while
    closedir(pDir);
}

void count_files(char *path, int* count) {
    struct dirent* ent = NULL;
    DIR* pDir;
    char dir[512];
    struct stat statbuf;  
    if((pDir=opendir(path))==NULL) {
        fprintf( stderr, "Cannot open directory:%s\n", path );
        return;
    }
    while(ent=readdir(pDir))
    {
        //得到读取文件的绝对路径名
        snprintf( dir, 512,"%s/%s", path, ent->d_name );   
        //得到文件信息
        lstat(dir, &statbuf);
        //判断是目录还是文件
        if(S_ISDIR(statbuf.st_mode)) {
            //排除当前目录和上级目录
            if(strcmp( ".",ent->d_name) == 0 || strcmp( "..",ent->d_name) == 0)
                continue;
            //如果是子目录,递归调用函数本身,实现子目录中文件遍历
            count_files(dir, count); //递归调用,遍历子目录中文件       
        }
        else {
			++(*count); // files number +1
        }       
    }//while
    closedir(pDir);
}

char** get_files_list(char* path, int* count) {
	char** filenames;
	int index = 0;
	
	*count = 0;
	count_files(path, count); // get the number of files
	filenames = (char**)malloc(*count * sizeof(char*));
	list_files(path, filenames, &index);
	
	return filenames;
}

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

int intamax(int* a, int n) {
    int i;
    int r = a[0];
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

int intamin(int* a, int n) {
    int i;
    int r = a[0];
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

void fft1D(float *arrayBuf, int n) {
	int i, k, r;
	float *buf1 = (float*)malloc(sizeof(float) * n * 2);
	for(i=0;i<n*2;i++)
		buf1[i]=arrayBuf[i];

	float *buf2 = (float*)malloc(sizeof(float)*n*2);
	int t1,t2;
	for(r=1;pow(2,r)<n;r++){
		t1=pow(2,r);
		t2=pow(2,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){				
				buf2[k*n/t1*2+i*2+0]=buf1[k/2*n/t2*2+(i*2+k%2)*2+0];
				buf2[k*n/t1*2+i*2+1]=buf1[k/2*n/t2*2+(i*2+k%2)*2+1];
			}
		}
		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	float c,s;
	for(r=1;pow(2,r)<=n;r++){
		t1=pow(2,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
				c=cos(2*3.1415927*(-i+t1/2+n/2)/t1);				
				s=sin(2*3.1415927*(-i+t1/2+n/2)/t1);
				buf1[k*t1*2+i*2+0]= buf2[k*t1*2+i*2+0]*c - buf2[k*t1*2+i*2+1]*s;				
				buf1[k*t1*2+i*2+1]=buf2[k*t1*2+i*2+1]*c+buf2[k*t1*2+i*2+0]*s;
			}
		}
		
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+i*2+0]+buf1[k*t1*2+(i+t1/2)*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+i*2+1]+buf1[k*t1*2+(i+t1/2)*2+1];
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+(i-t1/2)*2+0]-buf1[k*t1*2+i*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+(i-t1/2)*2+1]-buf1[k*t1*2+i*2+1];
			}
		}

		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	
	for(i=0;i<n*2;i++)
			arrayBuf[i]=buf2[i];

	free(buf2);
	free(buf1);
}

float* fft2D(float* imgBuf, int width, int height) {
	int i, u, v;
	float *buf=(float*)malloc(sizeof(float)*width*height*2);
	for(i=0;i<width*height;i++){
		buf[i*2+0]=imgBuf[i];
		buf[i*2+1]=0;
	}

	float *array=(float*)malloc(sizeof(float)*height*2);
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		fft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	free(array);

	for(v=0;v<height;v++){
		fft1D(buf+v*width*2, width);
	}

    return buf;
}

void* fftshift(float* fftBuf, int width, int height) {
    int i, j;
    float r_tmp, i_tmp;
    for(i = 0; i < height; ++i) {
		for(j = 0; j < width/2; j++) {
            r_tmp = fftBuf[2*(i*width+j)];
            i_tmp = fftBuf[2*(i*width+j)+1];
            fftBuf[2*(i*width+j)] = fftBuf[2*(i*width+j + (width+1)/2)];
            fftBuf[2*(i*width+j)+1] = fftBuf[2*(i*width+j + (width+1)/2)+1];
            fftBuf[2*(i*width+j + (width+1)/2)] = r_tmp;
            fftBuf[2*(i*width+j + (width+1)/2)+1] = i_tmp;
		}
    }
    for(j = 0; j < width; ++j) {
		for(i = 0; i < height/2; i++) {
            r_tmp = fftBuf[2*(i*width+j)];
            i_tmp = fftBuf[2*(i*width+j)+1];
            fftBuf[2*(i*width+j)] = fftBuf[2*((i+(height+1)/2)*width+j)];
            fftBuf[2*(i*width+j)+1] = fftBuf[2*((i+(height+1)/2)*width+j)+1];
            fftBuf[2*((i+(height+1)/2)*width+j)] = r_tmp;
            fftBuf[2*((i+(height+1)/2)*width+j)+1] = i_tmp;
		}
    }
}

void ifft1D(float *arrayBuf, int n) {
	int i, k, r;
	float *buf1=(float*)malloc(sizeof(float)*n*2);
	for(i=0;i<n*2;i++)
		buf1[i]=arrayBuf[i];

	float *buf2=(float*)malloc(sizeof(float)*n*2);
	int t1,t2;
	for(r=1;pow(2,r)<n;r++){
		t1=pow(2,r);
		t2=pow(2,r-1);
		for(k=0;k<t1;k++){
			for(i=0;i<n/t1;i++){				
				buf2[k*n/t1*2+i*2+0]=buf1[k/2*n/t2*2+(i*2+k%2)*2+0];
				buf2[k*n/t1*2+i*2+1]=buf1[k/2*n/t2*2+(i*2+k%2)*2+1];
			}
		}
		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	float c,s;
	for(r=1;pow(2,r)<=n;r++){
		t1=pow(2,r);
		for(k=0;k<n/t1;k++){
			for(i=t1/2;i<t1;i++){
				c=cos(2*3.1415927*(-i+t1/2)/t1);				
				s=sin(2*3.1415927*(-i+t1/2)/t1);
				buf1[k*t1*2+i*2+0]= buf2[k*t1*2+i*2+0]*c - buf2[k*t1*2+i*2+1]*s;				
				buf1[k*t1*2+i*2+1]=buf2[k*t1*2+i*2+1]*c+buf2[k*t1*2+i*2+0]*s;
			}
		}
		
		for(k=0; k<n/t1; k++){
			for(i=0;i<t1/2;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+i*2+0]+buf1[k*t1*2+(i+t1/2)*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+i*2+1]+buf1[k*t1*2+(i+t1/2)*2+1];
			}
			for(i=t1/2;i<t1;i++){
				buf2[k*t1*2+i*2+0]= buf1[k*t1*2+(i-t1/2)*2+0]-buf1[k*t1*2+i*2+0];
				buf2[k*t1*2+i*2+1]= buf1[k*t1*2+(i-t1/2)*2+1]-buf1[k*t1*2+i*2+1];
			}
		}

		for(i=0;i<n*2;i++)
			buf1[i]=buf2[i];
	}
	
	for(i=0;i<n*2;i++)
		arrayBuf[i]=buf2[i];

	free(buf2);
	free(buf1);
}

float* ifft2D(float* fftBuf, int width, int height) {	
	if(!fftBuf) {
		return 0;
    }

	int i, u, v;
	float *buf=fftBuf;
	for(i=0;i<width*height;i++)
		buf[i*2+1] = -buf[i*2+1];

	float *array=(float*)malloc(sizeof(float)*height*2);
	for(u=0;u<width;u++){
		for(v=0;v<height;v++){
			array[v*2+0]=buf[v*width*2+u*2+0];
			array[v*2+1]=buf[v*width*2+u*2+1];
		}
		ifft1D(array, width);
		for(v=0;v<height;v++){
			buf[v*width*2+u*2+0]=array[v*2+0];
			buf[v*width*2+u*2+1]=array[v*2+1];
		}
	}
	free(array);

	for(v=0;v<height;v++){
		ifft1D(buf+v*width*2, width);
	}

	float *img=(float*)malloc(sizeof(float)*width*height);
	for(i=0;i<width*height;i++){
		float j=sqrt(buf[i*2+0]*buf[i*2+0]+buf[i*2+1]*buf[i*2+1]);
		img[i]= j/(width*height);
	}
	
    return img;
}

float* get_phase_angle(float* fftBuf, int width, int height) {
    int i, j;
    float* angle = (float*)malloc(sizeof(float)*width*height);
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            angle[i*width+j] = atan2(fftBuf[2*(i*width+j)+1], fftBuf[2*(i*width+j)]);
        }
    }
    
    return angle;
}

float* get_log_amplitude(float* fftBuf, int width, int height) {
    int i, j;
    float* amplitude = (float*)malloc(sizeof(float)*width*height);
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            amplitude[i*width+j] = log(sqrt(fftBuf[2*(i*width+j)+1]*fftBuf[2*(i*width+j)+1] + fftBuf[2*(i*width+j)]*fftBuf[2*(i*width+j)]));
        }
    }
    
    return amplitude;
}

float* mean_filter(float* data, int width, int height) {
    int i, j, k, l;
    float tempData[height+2][width+2];
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            tempData[i+1][j+1] = data[i*width+j];
        }
    }
    for (i = 0; i < height; ++i) {
        tempData[i+1][0] = data[i*width];
        tempData[i+1][width+1] = data[i*width+width-1];
    }
    for (j = 0; j < width; ++j) {
        tempData[0][j+1] = data[j];
        tempData[height+1][j+1] = data[(height-1)*width+j];
    }
    // set the corners
    tempData[0][0]              = data[0];
    tempData[0][width+1]        = data[width-1];
    tempData[height+1][0]       = data[(height-1)*width];
    tempData[height+1][width+1] = data[height*width-1];
    
    double tmp;
    float* filtered_data = (float*)malloc(sizeof(float)*width*height);
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            tmp = 0;
            for (k = i; k < i+3; ++k) {
                for (l = j; l < j+3; ++l) {
                    tmp += tempData[k][l];
                }
            }
            filtered_data[i*width+j] = tmp/9;
        }
    }
    
    return filtered_data;
}

float* gaussian_filter(float* data, int width, int height) {
    float templates[9][9] = {   {0.0022759,0.0039843,0.0059439,0.0075562,0.0081855,0.0075562,0.0059439,0.0039843,0.0022759},
                                {0.0039843,0.0069753,0.010406,0.013228,0.01433,0.013228,0.010406,0.0069753,0.0039843},
                                {0.0059439,0.010406,0.015524,0.019735,0.021378,0.019735,0.015524,0.010406,0.0059439},
                                {0.0075562,0.013228,0.019735,0.025088,0.027177,0.025088,0.019735,0.013228,0.0075562},
                                {0.0081855,0.01433,0.021378,0.027177,0.02944,0.027177,0.021378,0.01433,0.0081855},
                                {0.0075562,0.013228,0.019735,0.025088,0.027177,0.025088,0.019735,0.013228,0.0075562},
                                {0.0059439,0.010406,0.015524,0.019735,0.021378,0.019735,0.015524,0.010406,0.0059439},
                                {0.0039843,0.0069753,0.010406,0.013228,0.01433,0.013228,0.010406,0.0069753,0.0039843},
                                {0.0022759,0.0039843,0.0059439,0.0075562,0.0081855,0.0075562,0.0059439,0.0039843,0.0022759}
                            };
    int i, j, k, l;
    float tempData[height+8][width+8];
    for (i = 0; i < height+8; ++i) {
        memset(tempData[i], 0, sizeof(float)*(width+8));
    }
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            tempData[i+4][j+4] = data[i*width+j];
        }
    }
    
    double tmp;
    float* filtered_data = (float*)malloc(sizeof(float)*width*height);
    memset(filtered_data, 0, sizeof(float)*width*height);
    
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            tmp = 0;
            for (k = 0; k < 9; ++k) {
                for (l = 0; l < 9; ++l) {
                    filtered_data[i*width+j] += tempData[i+k][j+l]*templates[k][l];
                }
            }
        }
    }
    
    return filtered_data;
}