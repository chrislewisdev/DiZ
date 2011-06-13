/******************************************
*Diz-graphics.cpp by Chris Lewis
*Defines all miscellaneous graphics functions
******************************************/

//Include our header file for everything we need
#include "../Include/diz-graphics.h"

//This function loads a Bitmap image and returns the info
AUX_RGBImageRec *LoadBMP(char *fname) {
	//Declare a file handle for our opened file
	FILE *file = NULL;

	//Double-check that we have a filename to avoid errors
	if (!fname) {
		return NULL;
	}

	//Attempt to open the file we want to use
	fopen_s(&file, fname, "r");
	
	//We check that the file has been correctly opened so we know there won't be any errors
	if (file) {
		//We only opened it to check it was OK, so we just close it again
		fclose(file);
		//Then return the loaded image data of the file
		return auxDIBImageLoad(fname);
	}

	//If we reach here, then it means we had an error, so we return NULL
	return NULL;
}

//This function outputs text to the screen of a specified font
void dizPrint(DIZ_FONT *font, const char *format, ...) {
	//Declare a buffer for our output text
	char text[256];
	//Declare a handle for our argument list
	va_list argList;

	//Double-check that we have an actual string to output
	if (format == NULL) {
		return;
	}

	//Start up our variable argument functions
	va_start(argList, format);
		//Use our argument list to format our string into our text[] array
		vsprintf_s(text, sizeof(text), format, argList);
	va_end(argList);

	//Push out our List attribute to prevent it interfering with anything else
	glPushAttrib(GL_LIST_BIT);
	//Check what type of font we're using
	if (font->info.type == DIZ_FONT_BITMAP) {
		//If it's a Bitmap, we account for the 32 skipped characters
		glListBase(font->base - 32);
	}else if (font->info.type == DIZ_FONT_OUTLINE) {
		//Otherwise we just set it to the normal base value for Outlines
		glListBase(font->base);
	}
	//Call the lists to output our text
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	//Pop our List attribute back into place
	glPopAttrib();
}