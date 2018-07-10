#include <stdio.h>
#include <stdlib.h>

//#include <MT2D/MT2D.h>
//#include <MT2D/MT2D_Display.h>
//#include <MT2D/MT2D_Keyboard.h>
//#include <MT2D/MT2D_System_Calls.h>
//#include <MT2D/InputBox/MT2D_InputBox_String.h>
//#include <MT2D/MessageBox/MT2D_MessageBox.h>
//#include <MT2D/MessageBox/MT2D_MessageBox_With_Result.h>
#include "image.h"
#include "pixel.h"
//#include "PPM.h"
//#include "Hexen.h"

char str_buffer[200];




/*
void screen_reset() {
	MT2D_Clear_Main_Window();
	insert_string_on_display("+++++++++++++++", 20, 60, DISPLAY_WINDOW1);
	insert_string_on_display("+  powered by +", 21, 60, DISPLAY_WINDOW1);
	insert_string_on_display("+ MT2D ENGINE +", 22, 60, DISPLAY_WINDOW1);
	insert_string_on_display("+++++++++++++++", 23, 60, DISPLAY_WINDOW1);
	insert_string_on_display("HexStartup++ v2.0, created by IBM5155", 24, 40, DISPLAY_WINDOW1);
	MT2D_Draw_Window(DISPLAY_WINDOW1);
}
*/

Image *HexenLumps = 0;
Image *Images = 0;
int       ImagesCnt =0;
Pixel *palette = 0;


void SaveHexenLump(int Index) {
	if (HexenLumps[Index].Width * HexenLumps[Index].Height == 640 * 480) {
		Image_Save(&HexenLumps[Index], "STARTUP.DAT");
	}
	else if (HexenLumps[Index].Width * HexenLumps[Index].Height == 16 * 23) {
		Image_Save(&HexenLumps[Index], "NOTCH.DAT");
	}
	else if (HexenLumps[Index].Width * HexenLumps[Index].Height == 4 * 16) {
		Image_Save(&HexenLumps[Index], "NETNOTCH.DAT");
	}
}


void GenerateHexenStartup(char * PATH, int left) {
	int palette_Length = 0;
	int HexenIMG_Size = 0;

/*
	for (int i = 0; i < 4; i++) {
		insert_string_on_display("                                              ", i, 0, DISPLAY_WINDOW1);
	}
*/
//	insert_string_on_display("PART 1 - LOADING IMAGE:", 0, 0, DISPLAY_WINDOW1);
fprintf(stdout, "GHS01 INI: Loading Image from %s\n", PATH);
//	MT2D_Draw_Window(DISPLAY_WINDOW1);
	Image *image = Image_Load(PATH);
	if (image) {
//		insert_string_on_display(" OK    ", 0, 25, DISPLAY_WINDOW1);
		fprintf(stdout, "GHS01 AOK: Image load success\n");

	}
	else {
//		insert_string_on_display(" FAILED", 0, 25, DISPLAY_WINDOW1);
		fprintf(stderr, "GHS01 ERR: FATAL: Image load failed\n");
		return;
	}
//	insert_string_on_display("PART 2 - CREATING PALETTE:", 1, 0, DISPLAY_WINDOW1);
	fprintf(stdout, "GHS02 INI: Creating palette\n");
//	MT2D_Draw_Window(DISPLAY_WINDOW1);
	if (image->Width == 640 && image->Width == 480) {
		palette = Image_GetPalette(image, 16);
	}
	if (!palette) {
//		insert_string_on_display(" FAILED", 1, 27, DISPLAY_WINDOW1);
//		sprintf(str_buffer, "WARNING!: no STARTUP image found, you need this file in order to generate the palette for NOTCH and NETNOTCH. The files are going to be saved but with all palette set to zero.");
//		MT2D_MessageBox(str_buffer);
		fprintf(stderr, "GHS02 WRN: Original STARTUP lump not found, continuing with zero palette for NOTCHes.\n");

	}
	else {
//		insert_string_on_display(" OK      ", 1, 27, DISPLAY_WINDOW1);
		fprintf(stdout, "GHS02 AOK: Palette creation success\n");
	}

//	insert_string_on_display("PART 3 - CREATING INDEXED IMAGE:", 2, 0, DISPLAY_WINDOW1);
//	MT2D_Draw_Window(DISPLAY_WINDOW1);
	fprintf(stdout, "GHS03 INI: Creating indexed image\n");

	Image *IndexedImage = Image_ImageConvert(image, palette,TYPE_Planar /*or 4BitBMP*/);
	if (IndexedImage) {
//		insert_string_on_display(" OK     ", 2, 32, DISPLAY_WINDOW1);
		fprintf(stdout, "GHS03 AOK: Successfully converted\n");

	}
	else {
//		insert_string_on_display(" FAILED", 2, 32, DISPLAY_WINDOW1);
		fprintf(stderr, "GHS03 ERR: FATAL: Conversion failed\n");
		return;
	}


	if (IndexedImage->Width * IndexedImage->Height == 640 * 480) {
//		insert_string_on_display("PART 4 - GENERATING PLANAR STARTUP:          ", 3, 0, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		fprintf(stdout, "GHS04 OU1: Generating planar STARTUP image lump\n");
		Image_Save(IndexedImage, "STARTUP.DAT");
	}
	else if (IndexedImage->Width * IndexedImage->Height == 16 * 23) {
//		insert_string_on_display("PART 4 - GENERATING BITMAP NOTCH:          ", 3, 0, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		fprintf(stdout, "GHS04 OU2: Generating bitmap NOTCH image lump\n");
		Image_Save(IndexedImage, "NOTCH.DAT");
	}
	else if (IndexedImage->Width * IndexedImage->Height == 4 * 16) {
//		insert_string_on_display("PART 4 - GENERATING BITMAP NOTCH:          ", 3, 0, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		fprintf(stdout, "GHS04 OU3: Creating bitmap NETNOTCH image lump\n");
		Image_Save(IndexedImage, "NETNOTCH.DAT");
	}
	else {
//		insert_string_on_display("PART 4 - UNKNOWN TYPE OF LUMP TO BE GENERATED", 3, 0, DISPLAY_WINDOW1);
//		sprintf(str_buffer, "ERROR: file %s has a non standart size (%dx%d)", PATH, image->Width, image->Height);
//		MT2D_MessageBox(str_buffer);
		fprintf(stderr, "GHS04 ERR: FATAL: size of image in file %s (%dx%d) does not correspond to a known STARTUP image lump.\n", PATH, image->Width, image->Height);
		return;
	}
//	insert_string_on_display(" OK       ", 3, 35, DISPLAY_WINDOW1);
//	sprintf(str_buffer, "File '%s'  was created in the image folder.                     Press any key to %s", PATH, (left == 0 ? "exit." : "continue."));
//	MT2D_MessageBox(str_buffer);
	fprintf(stdout, "GHS04 AOK: file %s successfully created.\n", PATH);
	fprintf(stdout, "GHS05 %s: %sing.\n", (left == 0 ? "END" : "CNT"), (left == 0 ? "exit" : "continu"));
}
/*
void menu() {
	draw
}
*/
int main(int argc, char *argv[]) {


/*  MT2D STUFF*/
//	MT2D_Init();
//	screen_reset();
	/*  ==========*/
	if (argc == 1) {
//		char *PATH = 0;
//		FILE *test;
//		do {
			//PART 1: Get a valid file + PATH in case the file is in a folder other than the one where this software is
//			do {
//				PATH = MT2D_InputBox_String("type the name of the image file (and the file address if the image is in   another folder) - example 'test.ppm' ,' 'c:/test/img.ppm'");
//				test = fopen(PATH, "r");
//				if (!test) {
//					if (MT2D_MessageBox_With_Result("FILE NOT FOUND!", "Do you want to retry or exit?", "try again", "Exit") == 2) {
					fprintf(stderr, "MAIN1 ERR: FATAL: file not specified\n");
						exit(1);
//					}
//					else {
//						if (PATH) {
//							free(PATH);
//						}
//						PATH = 0;
//					}
//				}
//				else {
//					fclose(test);
//				}
//			} while (!PATH);
			// END PART 1
//			GenerateHexenStartup(PATH, 0);
//		}while(MT2D_MessageBox_With_Result("MessageBox", "Do you want to load another file?", "Yes", "No") == 1);

	}
	else {
		//lets find the main palette, apply to all the others images and also save as planar or 4bit bitmap
		int i = 1;
		int PaletteLength = 0;
		ImagesCnt = argc;
		fprintf(stdout, "MAIN1 INI: Loading %d image%s\n", argc-1,(argc==2? "" : "S"));
//		insert_string_on_display(str_buffer, 0, 0, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		Images = (Image*)malloc(ImagesCnt * sizeof(Image));
		for (i = 1; i < argc; i++) {
			Images[i - 1] = *Image_Load(argv[i]);
		}
//		insert_string_on_display(" OK", 0, 28, DISPLAY_WINDOW1);
		fprintf(stdout, "MAIN1 AOK: %d image%s loaded\n", argc-1,(argc==2? "" : "S"));
		//if we reach this area, we have an array of valid images.
//		insert_string_on_display("PART 2 - CREATING PALETTE:", 1, 0, DISPLAY_WINDOW1);
		fprintf(stdout, "MAIN2 INI: Creating palette\n");

		for (i = 0; i < argc; i++) {
			if (Images[i].Width == 640 && Images[i].Height == 480) {
				palette = Image_GetPalette(&Images[i], 16);
				break;
			}
		}
		if (!palette) {
//			insert_string_on_display(" FAILED", 1, 27, DISPLAY_WINDOW1);
			fprintf(stderr, "MAIN2 WRN: Original STARTUP lump not found, continuing with zero palette for NOTCHes.\n");

//			sprintf(str_buffer, "WARNING!: no STARTUP image found, you need this file in order to generate the palette for NOTCH and NETNOTCH. The files are going to be saved but with all palette set to zero.");
//			MT2D_MessageBox(str_buffer);
		}
		else {
//			insert_string_on_display(" OK", 1, 27, DISPLAY_WINDOW1);
			fprintf(stdout, "MAIN2 AOK: Palette created\n");
		}
		//if we reach this area, we have a valid palette
//		insert_string_on_display("PART 3 - CREATING INDEXED IMAGE:", 2, 0, DISPLAY_WINDOW1);
		fprintf(stdout, "MAIN3 INI: Creating indexed image\n");
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		HexenLumps = (Image*)malloc(ImagesCnt * sizeof(Image));
		//apply the palette to all those images
		for (i = 0; i < argc -1; i++) {
			HexenLumps[i] = *Image_ImageConvert(&Images[i], palette,TYPE_Planar /*or 4bit BMP*/);
		}
		fprintf(stdout, "MAIN3 AOK: Image converted.\n");
//		insert_string_on_display(" OK", 2, 32, DISPLAY_WINDOW1);
		//if we reach this area, we have a valid palette
		//now we just need to save all the input images in all their respective formats
//		insert_string_on_display("PART 4 - GENERATING HEXEN LUMPS:", 3, 0, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		fprintf(stdout, "MAIN4 INI: Generating and saving Hexen lumps\n");
		for (i = 0; i < argc -1; i++) {
			SaveHexenLump(i);
		}
		fprintf(stdout, "MAIN4 AOK: Hexen lumps saved\n");
//		insert_string_on_display(" OK", 3, 35, DISPLAY_WINDOW1);
//		MT2D_Draw_Window(DISPLAY_WINDOW1);
		//if we reach this area, we saved all the image files.
		fprintf(stdout, "MAIN5 END: Conversion successful, exiting\n");
//		MT2D_MessageBox("All the input files were successfully converted and were saved in the folder where the images were.");
		return 0;
	}
}
