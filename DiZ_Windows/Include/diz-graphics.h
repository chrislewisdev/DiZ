/****************************************
*Diz-graphics.h by Chris Lewis
*Declares all the graphics functions or variables/structs for use in the DiZ library
****************************************/

//Check this header hasn't already been included
#ifndef DIZ_GRAPHICS_H
#define DIZ_GRAPHICS_H

//Declare all the headers we need
//First we check for any symbol typically found in that header to see if we need it
#include <windows.h>		//Windows.h- used for our general windows functions and windowing
#include <gl/gl.h>			//Gl.h- used for our main OpenGL functions
#include <gl/glu.h>			//Glu.h- used for various extra OpenGL functionalities
#include <gl/glaux.h>		//Glaux.h- used mainly for image loading
#include <d3d9.h>			//D3D9.h- provides Direct3D9 functions
#include <stdio.h>			//Stdio.h- used mainly for texture loading

//Define some constants that we would like to be used
#define		DIZ_2D					0		//This indicates the use of 2D graphics in a window
#define		DIZ_3D					1		//And this, 3D graphics

#define		DIZ_FONT_BITMAP			2		//This indicates the use of a Bitmap Font
#define		DIZ_FONT_OUTLINE		3		//This indicates the use of an Outline font

#define		DIZ_GRAPHICS_OPENGL		4		//This indicates an OpenGL graphics mode
#define		DIZ_GRAPHICS_DIRECT3D9	5		//This indicates Direct3D9 graphics mode

//This structure will hold OpenGL info for a window
struct DIZ_INFO_GL {
	//The mode of our window- 2D, 3D, etc
	int mode;
	//The Near and Far draw distances for our scene
	GLfloat zNear, zFar;
};

//This is our window info structure- used to describe all the characteristics of a window we want
struct DIZ_WNDINFO {
	//The title for our window
	char *title;
	//The width, height, and colour bits for our window
	int width, height, bits;
	//The graphics mode of our window- OPENGL or DIRECT3D9
	int mode;
	//Our OpenGL settings
	DIZ_INFO_GL gl;
	//Our Direct3D settings
	D3DPRESENT_PARAMETERS d3d;
	//Some flags to indicate whether we want a fullscreen window, a menu, etc.
	bool fullscreen, showCursor, menu;
	//A pointer to our window procedure for handling messages
	WNDPROC *wndProc;
};

//This structure contains all the info needed for a GL font
struct DIZ_FONTINFO {
	//The name of the font we want to use
	char *font;
	//The type of font- Bitmap or Outline
	int type;
	//The height, width, and weight values of our font
	int height, width, weight;
	//The Angle of Escapement and Orientation values
	int escAng, orientation;
	//Options for Italics, Underlines, Strikeouts
	bool italic, underline, strikeout;
	//The character set we want to use
	DWORD charSet;
	//Output Precision and Clipping Precision
	DWORD outPrecis, clipPrecis;
	//Output Quality
	DWORD outQuality;
	//Font Family and Pitch
	DWORD family;
	//Deviation and Thickness values- used for Outline fonts
	float deviation, thickness;
	//Type of outline- typically WGL_FONT_POLYGONS or WGL_FONT_LINES
	int outlineType;
};

//This class handles all window functions and properties
class DIZ_WINDOW {
public:
	//Declare our Constructor and Destructor
	DIZ_WINDOW();
	~DIZ_WINDOW();

	//Declare all our window functions
	//This function sizes our OpenGL viewport to the desired size
	void resizeGL();
	//This function initialises all our OpenGL settings
	bool initGL();
	//This function closes our window down
	void killWindow();
	//This function creates our window
	bool createWindow();

	//Declare a WNDINFO structure to hold all the properties of our window
	DIZ_WNDINFO info;
	//Declare our handle for our Rendering Context
	HGLRC hRC;
	//Declare our handle for our Device Context
	HDC hDC;
	//Declare our window handle
	HWND hWnd;
	//Declare a handle for our application instance
	HINSTANCE hInstance;
	//Declare a Direct3D Interface
	IDirect3D9 *interface3D9;
	//Declare a Direct3D Device
	IDirect3DDevice9 *device3D9;

private:
	//Declare our private functions
	//This function creates our OpenGL scene
	bool createGL();
	//This function destroys our OpenGL scene
	void killGL();
	//This function creates our Direct3D scene
	bool createD3D();
	//This function destroys our Direct3D scene
	void killD3D();
};

//This class is used to handle loading and properties of textures
class DIZ_TEXTURE {
public:
	//Declare the constructor and destructor
	DIZ_TEXTURE();
	~DIZ_TEXTURE();

	//Our functions- just the function to load the texture for now
	bool load();

	//Our texture properties- first just our texture handle
	GLuint tex;
	//The filename that we use when loading
	char *filename;
	//Filters- these specify the various filters to use for our texture, such as LINEAR or whatever else
	GLint minFilter, magFilter;
	//Mipmap indicates we want a mipmap texture, status indicates success of loading
	bool status, mipmap;
};

//This class handles all the properties and functions involved in font usage
class DIZ_FONT {
public:
	//Declare our Constructor and Deconstructor
	DIZ_FONT();
	~DIZ_FONT();

	//Declare all our public functions
	//This function loads up the font
	bool load();
	//This function kills the font (obviously)
	void kill();

	//Declare all our properties for the class
	//Declare a FONTINFO structure to hold all the properties for the font
	DIZ_FONTINFO info;
	//Declare our base value for our display list of characters
	GLuint base;
	//Declare a GMF array for use if we want an Outline font
	GLYPHMETRICSFLOAT gmf[256];
	//Declare a pointer to a Device Context to use
	HDC *hDC;
};

//Declare the functions we use for loading
//Function to load a BMP image
AUX_RGBImageRec *LoadBMP(char *fname);

//Declare any other graphics functions
//Function to draw a specified font to the screen
void dizPrint(DIZ_FONT *font, const char *fmt, ...);

#endif