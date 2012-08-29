
#include <stdio.h>
#include <stdlib.h>

#include "feature.h"


int main(int argc, char** argv) {
    FILE *fp;
    char filename[50] = "test.jpg";
    int i, j;
    int width = 3, height = 3;
    image_rgb* rgb = load_rgb_image(filename, 1);
    image_hsl* hsl = rgb2hsl(rgb);
    
    fp = fopen("out.txt", "w");
    for (i = 0; i < height; ++i) {
        for (j = 0; j < width; ++j) {
            fprintf(fp, "%d\t", rgb->b[i*width+j]);

        }
        fprintf(fp, "\n");
    }
    
    return (EXIT_SUCCESS);
}

