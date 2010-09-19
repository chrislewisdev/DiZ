/******************************************
*Diz-objects.cpp by Chris Lewis
*Declares all the functions/variables/structs for Objects used in the DiZ library
******************************************/

//Check that this header hasn't already been included
#ifndef DIZ_OBJECTS_H
#define DIZ_OBJECTS_H

//Declare all the headers we need
#include <windows.h>			//Windows.h- used for all our general Windows functions, windowing, etc.
#include <gl/gl.h>				//Gl.h- used for our main OpenGL graphics functions

//This structure just holds a basic x,y pair for screen co-ordinates
struct DIZ_COORD {
	GLfloat x, y;
};

//This structure holds colour values- Red, Green, Blue, and Alpha values
struct DIZ_COLOUR {
	float r, g, b, a;
};

//This structure has all mouse-related info we'd want
struct DIZ_MOUSE {
	//The screen co-ordinates of the mouse
	int x, y;
	//Whether or not Left Click is being held down
	bool lClick;
};

//This structure brings together all main control elements for an application
struct DIZ_CONTROL {
	//We have a mouse structure for all mouse info of course
	DIZ_MOUSE m;
	//And then an array for tracking keyboard presses
	bool keys[256];
};

//This is the base class for any DiZ Sprite- should cover main information needed and basic functions
class DIZ_SPRITE {
public:
	//Declare our Constructor and Destructor, as always
	DIZ_SPRITE();
	~DIZ_SPRITE();

	//Declare our functions
	//This function will draw a basic textured sprite
	void draw();

	//Declare a COORD structure to hold the screen location of the object
	DIZ_COORD c;
	//Declare a COLOUR object to hold the colour values of the object
	DIZ_COLOUR colour;
	//Use a pointer to a CONTROL structure that the object can use
	DIZ_CONTROL *control;
	//Declare some basic width/height values for the object
	int width, height;
	//And declare a pointer to the texture handle that the object uses
	GLuint *tex;
};

#endif