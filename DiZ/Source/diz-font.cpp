/****************************************
*Diz-font.cpp by Chris Lewis
*Defines all functions of the DIZ_FONT class in diz-graphics.h
****************************************/

//Declare our include file
#include "diz-graphics.h"

//Define our Constructor for the class- initialise everything
DIZ_FONT::DIZ_FONT() {
	//About all we need to do is set up some default INFO values
	//These values will provide a proper working font by default
	info.font			= "Arial";
	info.type			= DIZ_FONT_BITMAP;
	info.height			= -24;
	info.width			= 0;
	info.weight			= FW_NORMAL;
	info.italic			= false;
	info.strikeout		= false;
	info.underline		= false;
	info.escAng			= 0;
	info.orientation	= 0;
	info.charSet		= ANSI_CHARSET;
	info.outPrecis		= OUT_TT_PRECIS;
	info.clipPrecis		= CLIP_DEFAULT_PRECIS;
	info.outQuality		= ANTIALIASED_QUALITY;
	info.family			= FF_DONTCARE | DEFAULT_PITCH;
	info.deviation		= 0.2f;
	info.thickness		= 1.0f;
	info.outlineType	= WGL_FONT_POLYGONS;
	//Now we just initialise our DC to null
	hDC = NULL;
}

//Define our Deconstructor for the class
DIZ_FONT::~DIZ_FONT() {

}

//Define our load() function- loads up the font and gets it ready for use
bool DIZ_FONT::load() {
	//Declare font handles for the font we want to use and whatever's currently in use
	HFONT font, oldFont;

	//Generate display lists for our font starting with our base value
	base = glGenLists(96);

	//Create a new font using our specified font info
	font = CreateFont(	info.height,
						info.width,
						info.escAng,
						info.orientation,
						info.weight,
						info.italic,
						info.underline,
						info.strikeout,
						info.charSet,
						info.outPrecis,
						info.clipPrecis,
						info.outQuality,
						info.family,
						info.font);

	//Select our new font to use in our Device Context
	oldFont = (HFONT)SelectObject(*hDC, font);
	//Check what type of font we want
	if (info.type == DIZ_FONT_BITMAP) {
		//If we want a Bitmap font, create that
		wglUseFontBitmaps(	*hDC,		//Device Context to get the font from
							32,			//Starting character we want to use
							96,			//How many characters to create
							base);		//Start point of our display lists
	}else if (info.type == DIZ_FONT_OUTLINE) {
		//Or if we want an Outline font, create that
		wglUseFontOutlines(	*hDC,				//Device Context to get the font from
							0,					//Start character
							255,				//Number of characters to create
							base,				//Starting display list
							info.deviation,		//Our deviation from the true outline
							info.thickness,		//Thickness (Z-direction) of font
							info.outlineType,	//Type of outline to create
							gmf);				//GMF info array
	}
	//Now that we're done, set the Device's font back to whatever it was before
	SelectObject(*hDC, oldFont);
	//Then delete the object we created
	DeleteObject(font);

	//And exit
	return true;
}

//This function deletes our font data
void DIZ_FONT::kill() {
	//First check what type of font we created
	if (info.type == DIZ_FONT_BITMAP) {
		//Bitmap fonts only have 96 characters, so we delete that
		glDeleteLists(base, 96);
	}else if (info.type == DIZ_FONT_OUTLINE) {
		//Whereas Outline fonts have 256
		glDeleteLists(base, 256);
	}
}