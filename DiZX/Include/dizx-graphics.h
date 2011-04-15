/****************************************
*Dizx-graphics.h by Chris Lewis
*Declares all the graphics functions or variables/structs for use in the DiZX library
****************************************/

//Check this header hasn't already been included
#ifndef DIZX_GRAPHICS_H
#define DIZX_GRAPHICS_H

//Declare all the headers we need
//First we check for any symbol typically found in that header to see if we need it
#include <windows.h>		//Windows.h- used for our general windows functions and windowing
#include <d3d9.h>			//D3D9.h- provides Direct3D9 functions
#include <stdio.h>			//Stdio.h- used mainly for texture loading

//Define some constants that we would like to be used
#define		DIZ_2D					0		//This indicates the use of 2D graphics in a window
#define		DIZ_3D					1		//And this, 3D graphics

//This is our window info structure- used to describe all the characteristics of a window we want
struct DIZX_WNDINFO {
	//The title for our window
	char *title;
	//The width, height, and colour bits for our window
	int width, height, bits;
	//Our Direct3D settings
	D3DPRESENT_PARAMETERS d3d;
	//Some flags to indicate whether we want a fullscreen window, a menu, etc.
	bool fullscreen, showCursor, menu;
	//A pointer to our window procedure for handling messages
	WNDPROC *wndProc;
};

//This class handles all window functions and properties
class DIZX_WINDOW {
public:
	//Declare our Constructor and Destructor
	DIZX_WINDOW();
	~DIZX_WINDOW();

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
	DIZX_WNDINFO info;
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
	//This function creates our Direct3D scene
	bool createD3D();
	//This function destroys our Direct3D scene
	void killD3D();
};

#endif