#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "pixel.h"

Image *HexenLumps = 0;
Image *Images = 0;
int       ImagesCnt =0;
Pixel *palette = 0;
char startupfilename[200] = "STARTUP.DAT";
char notchfilename[200] = "NOTCH.DAT";
char netnotchfilename[200] = "NETNOTCH.DAT";


void SaveHexenLump(int Index) {
	if (HexenLumps[Index].Width * HexenLumps[Index].Height == 640 * 480) {
		Image_Save(&HexenLumps[Index], startupfilename);
	}
	else if (HexenLumps[Index].Width * HexenLumps[Index].Height == 16 * 23) {
		Image_Save(&HexenLumps[Index], notchfilename);
	}
	else if (HexenLumps[Index].Width * HexenLumps[Index].Height == 4 * 16) {
		Image_Save(&HexenLumps[Index], netnotchfilename);
	}
}


void GenerateHexenStartup(char * PATH, int left) {
	int palette_Length = 0;
	int HexenIMG_Size = 0;

	fprintf(stdout, "GHS01 INI: Loading Image from %s\n", PATH);
	Image *image = Image_Load(PATH);
	if (image) {
		fprintf(stdout, "GHS01 AOK: Image load success\n");
	}
	else {
		fprintf(stderr, "GHS01 ERR: FATAL: Image load failed\n");
		return;
	}
	fprintf(stdout, "GHS02 INI: Creating palette\n");
	if (image->Width == 640 && image->Width == 480) {
		palette = Image_GetPalette(image, 16);
	}
	if (!palette) {
		fprintf(stderr, "GHS02 WRN: Original STARTUP lump not found, continuing with zero palette for NOTCHes.\n");
	}
	else {
		fprintf(stdout, "GHS02 AOK: Palette creation success\n");
	}

	fprintf(stdout, "GHS03 INI: Creating indexed image\n");

	Image *IndexedImage = Image_ImageConvert(image, palette,TYPE_Planar /*or 4BitBMP*/);
	if (IndexedImage) {
		fprintf(stdout, "GHS03 AOK: Successfully converted\n");
	}
	else {
		fprintf(stderr, "GHS03 ERR: FATAL: Conversion failed\n");
		return;
	}


	if (IndexedImage->Width * IndexedImage->Height == 640 * 480) {
		fprintf(stdout, "GHS04 OU1: Generating planar STARTUP image lump\n");
		Image_Save(IndexedImage, startupfilename);
	}
	else if (IndexedImage->Width * IndexedImage->Height == 16 * 23) {
		fprintf(stdout, "GHS04 OU2: Generating bitmap NOTCH image lump\n");
		Image_Save(IndexedImage, notchfilename);
	}
	else if (IndexedImage->Width * IndexedImage->Height == 4 * 16) {
		fprintf(stdout, "GHS04 OU3: Creating bitmap NETNOTCH image lump\n");
		Image_Save(IndexedImage, netnotchfilename);
	}
	else {
		fprintf(stderr, "GHS04 ERR: FATAL: size of image in file %s (%dx%d) does not correspond to a known STARTUP image lump.\n", PATH, image->Width, image->Height);
		return;
	}
	fprintf(stdout, "GHS04 AOK: file %s successfully created.\n", PATH);
	fprintf(stdout, "GHS05 %s: %sing.\n", (left == 0 ? "END" : "CNT"), (left == 0 ? "exit" : "continu"));
}
int main(int argc, char *argv[]) {


	if (argc == 1) {
					fprintf(stderr, "MAIN1 ERR: FATAL: file not specified\n");
						exit(1);
	}
	else {
		//lets find the main palette, apply to all the others images and also save as planar or 4bit bitmap
		int i = 1;
		int PaletteLength = 0;
		ImagesCnt = argc;
		fprintf(stdout, "MAIN1 INI: Loading %d image%s\n", argc-1,(argc==2? "" : "S"));
		Images = (Image*)malloc(ImagesCnt * sizeof(Image));
		for (i = 1; i < argc; i++) {
			Images[i - 1] = *Image_Load(argv[i]);
		}
		fprintf(stdout, "MAIN1 AOK: %d image%s loaded\n", argc-1,(argc==2? "" : "S"));
		//if we reach this area, we have an array of valid images.
		fprintf(stdout, "MAIN2 INI: Creating palette\n");

		for (i = 0; i < argc; i++) {
			if (Images[i].Width == 640 && Images[i].Height == 480) {
				palette = Image_GetPalette(&Images[i], 16);
				break;
			}
		}
		if (!palette) {
			fprintf(stderr, "MAIN2 WRN: Original STARTUP lump not found, continuing with zero palette for NOTCHes.\n");
		}
		else {
			fprintf(stdout, "MAIN2 AOK: Palette created\n");
		}
		//if we reach this area, we have a valid palette
		fprintf(stdout, "MAIN3 INI: Creating indexed image\n");
		HexenLumps = (Image*)malloc(ImagesCnt * sizeof(Image));
		//apply the palette to all those images
		for (i = 0; i < argc -1; i++) {
			HexenLumps[i] = *Image_ImageConvert(&Images[i], palette,TYPE_Planar /*or 4bit BMP*/);
		}
		fprintf(stdout, "MAIN3 AOK: Image converted.\n");
		//if we reach this area, we have a valid palette
		//now we just need to save all the input images in all their respective formats
		fprintf(stdout, "MAIN4 INI: Generating and saving Hexen lumps\n");
		for (i = 0; i < argc -1; i++) {
			SaveHexenLump(i);
		}
		fprintf(stdout, "MAIN4 AOK: Hexen lumps saved\n");
		//if we reach this area, we saved all the image files.
		fprintf(stdout, "MAIN5 END: Conversion successful, exiting\n");
		return 0;
	}
}
