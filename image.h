#pragma once
#include "pixel.h"

enum ImageFormat {
	TYPE_UNSUPPORTED = -1,
	TYPE_PPM,
	TYPE_PNG,
	TYPE_Planar,
	TYPE_4BitBitmap
};

struct Image {
	void *ImagePointer;
	unsigned int Width;
	unsigned int Height;
	ImageFormat Type;
	Pixel *Palette;
	bool Loaded;
	bool Saved;

};

Image *Image_CreateBlank();
Image *Image_Load(char*PATH);
bool Image_Save(Image *img,char *PATH);
void Image_GetPixel(Image *img, unsigned int X, unsigned int Y, Pixel *p);
void Image_GetPixel(Image *img, int offset, Pixel *p);
Pixel *Image_GetPalette(Image *image, int paletteLength);
Image *Image_ImageConvert(Image *image,Pixel *Palette, ImageFormat ConverTo);
