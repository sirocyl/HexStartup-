#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"

#define RGB_COMPONENT_COLOR 255

PPMImage *readPPM(const char *filename)
{//Code Created by: RPFELGUEIRAS
	char buff[16];
	PPMImage *img;
	FILE *fp;
	int c, rgb_comp_color;
	//open PPM file for reading
	fp = fopen(filename, "rb");
	if (!fp) {
		fprintf(stderr, "RPPM1 ERR: FATAL: Unable to open file at %s\n", filename);
		return 0;
	}

	//read image format
	if (!fgets(buff, sizeof(buff), fp)) {
		perror(filename);
		fprintf(stderr, "RPPM2 ERR: FATAL: Unable to read image file at %s\n", filename);
		fclose(fp);
		return 0;
	}

	//check the image format
	if (buff[0] != 'P' || buff[1] != '6') {
		fprintf(stderr, "RPPM3 ERR: FATAL: Invalid PPM file (Header must be 'P6')\n");
		fclose(fp);
		return 0;
	}

	//alloc memory form image
	img = (PPMImage *)malloc(sizeof(PPMImage));
	if (!img) {
		fprintf(stderr, "RPPM4 ERR: FATAL: Unable to allocate memory\n");
		fclose(fp);
		return 0;
	}

	//check for comments
	c = getc(fp);
	while (c == '#') {
		while (getc(fp) != '\n');
		c = getc(fp);
	}

	ungetc(c, fp);
	//read image size information
	if (fscanf(fp, "%d %d", &img->x, &img->y) != 2) {
		fprintf(stderr, "RPPM5 ERR: FATAL: Invalid image size (error loading '%s')\n", filename);
		fclose(fp);
		return 0;
	}

	//read rgb component
	if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
		fprintf(stderr, "RPPM6 ERR: FATAL: Invalid rgb component (error loading '%s')\n", filename);
		fclose(fp);
		return 0;
	}

	//check rgb component depth
	if (rgb_comp_color != RGB_COMPONENT_COLOR) {
		fprintf(stderr, "RPPM7 ERR: FATAL: '%s' does not have 8-bits components\n", filename);
		fclose(fp);
		return 0;
	}

	while (fgetc(fp) != '\n');
	//memory allocation for pixel data
	img->data = (Pixel*)malloc(img->x * img->y * sizeof(Pixel));

	if (!img) {
		fprintf(stderr, "RPPM8 ERR: FATAL: Unable to allocate memory\n");
		fclose(fp);
		return 0;
	}

	//read pixel data from file
	if (fread(img->data, 3 * img->x, img->y, fp) != img->y) {
		fprintf(stderr, "RPPM9 ERR: FATAL: Error loading image '%s'\n", filename);
		fclose(fp);
		return 0;
	}

	fclose(fp);
	return img;
}

void PPM_GetPixel(PPMImage *img, int Offset,Pixel *p) {
	p->blue = img->data[Offset].blue;
	p->red = img->data[Offset].red;
	p->green = img->data[Offset].green;
}
