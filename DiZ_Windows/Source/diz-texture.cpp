/***************************************
*Diz-texture.cpp by Chris Lewis
*Defines all DIZ_TEXTURE functions declared in diz-graphics.h
***************************************/

//Declare our include file- of course
#include "../Include/diz-graphics.h"

//Define our constructor- initialises all of our values to safe settings
DIZ_TEXTURE::DIZ_TEXTURE() {
	//Set our status to false to indicate it hasn't been loaded
	status = false;
	//We consider the use of non-Mipmapped textures as default
	mipmap = false;
	//And set our filters to the default LINEAR setting
	minFilter = GL_LINEAR;
	magFilter = GL_LINEAR;
}

//Define our destructor- doesn't really need to do anything in this case
DIZ_TEXTURE::~DIZ_TEXTURE() {

}

//This function loads the texture data for the texture
bool DIZ_TEXTURE::load() {
	//Declare a handle for our texture data
	AUX_RGBImageRec *img = NULL;

	//Make sure the image loads correctly by making sure it exists
	img = LoadBMP(filename);
	if (img) {
		//First set status to true to indicate we've successfully loaded the texture
		status = true;

		//Generate some space in memory for our specific texture handle
		glGenTextures(1, &tex);

		//Bind our newly generated texture for use
		glBindTexture(GL_TEXTURE_2D, tex);

		//Check if we want a mipmap texture or not- build our texture accordingly
		if (mipmap) {
			gluBuild2DMipmaps(	GL_TEXTURE_2D,			//The type of texture we want to create 
								3,						//Components (?)
								img->sizeX,				//Width of the texture
								img->sizeY,				//Height of the texture
								GL_RGB,					//Format- in this case we just go RGB
								GL_UNSIGNED_BYTE,		//Our data is comprised of Unsigned Bytes
								img->data);				//Where our data is stored
		}else {
			glTexImage2D(	GL_TEXTURE_2D,				//Type of texture we want
							0,							//Level (?)
							3,							//Components (?)
							img->sizeX,					//Width
							img->sizeY,					//Height
							0,							//Border- we don't want one
							GL_RGB,						//RGB format
							GL_UNSIGNED_BYTE,			//Data is Unsigned Bytes
							img->data);					//Image data
		}

		//Then set our texture filter properties to whatever was specified
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	}

	//Here we free up our image data space, but we make sure it actually exists first
	//So we first check for the image handle itself
	if (img) {
		//Then check for the data it's meant to contain
		if (img->data) {
			//Free that if it exists
			free(img->data);
		}
		//Then free the handle itself
		free(img);
	}

	//And exit
	return true;
}