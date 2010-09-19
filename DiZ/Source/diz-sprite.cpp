/*********************************************
*Diz-object.cpp by Chris Lewis
*Defines the DIZ_OBJECT class specified in diz-objects.h
*********************************************/

//Include our header
#include "../Include/diz-objects.h"

//Declare our Constructor to initialise everything to some safe values
DIZ_SPRITE::DIZ_SPRITE() {
	//Set our texture handle to NULL for a safe value
	tex = NULL;

	//Set our default co-ordinate values to 0
	c.x = 0;	c.y = 0;
	
	//And have our default width/height as 50 for the sake of it
	width = 50;	height = 50;

	//And set some default colour values (Opaque White)
	colour.r = 1.0f;	colour.g = 1.0f;	colour.b = 1.0f;
	colour.a = 1.0f;
}

//Our Destructor- doesn't really need to do anything for this
DIZ_SPRITE::~DIZ_SPRITE() {

}

//This function draws the basic object, treating it like a rectangular sprite
void DIZ_SPRITE::draw() {
	//Set our colour we want to use for drawing (using our object's values)
	glColor3f(colour.r, colour.g, colour.b);

	//If we have a texture, bind it (a NULL value would just be blank)
	if (tex != NULL) {
		glBindTexture(GL_TEXTURE_2D, *tex);
	}else {
		glBindTexture(GL_TEXTURE_2D, NULL);
	}

	//Now draw a quad using any specified texture
	glBegin(GL_QUADS);
		//Top-right corner
		glTexCoord2f(1.0f, 0.0f); glVertex2f(c.x + (width / 2), c.y - (height / 2));
		//Top-left corner
		glTexCoord2f(0.0f, 0.0f); glVertex2f(c.x - (width / 2), c.y - (height / 2));
		//Bottom-left corner
		glTexCoord2f(0.0f, 1.0f); glVertex2f(c.x - (width / 2), c.y + (height / 2));
		//Bottom-right corner
		glTexCoord2f(1.0f, 1.0f); glVertex2f(c.x + (width / 2), c.y + (height / 2));
	glEnd();
}