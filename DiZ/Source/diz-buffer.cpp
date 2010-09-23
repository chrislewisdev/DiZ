/*********************************************
*Diz-buffer.cpp by Chris Lewis
*Defines the DIZ_BUFFER class from diz-sound.h
*********************************************/

//Declare our include file
#include "../Include/diz-sound.h"

//Define our Constructor to intialise anything necessary to a default value
DIZ_BUFFER::DIZ_BUFFER() {
	//Set our filename to an empty string
	fname[0] = '\0';
	//Set our buffer handle to 0 as default
	buf = 0;
}

//Define our Deconstructor
DIZ_BUFFER::~DIZ_BUFFER() {

}

//This function loads the specified file and properly creates our buffer
bool DIZ_BUFFER::load() {
	//Declare our data storage for the WAV file
	ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;

	//First check that we have an actual filename
	if (fname == NULL) {
		return false;
	}

	//Then create our buffer using the alut function
	//buf = alutCreateBufferFromFile(fname);
	//Create our buffer
	alGenBuffers(1, &buf);
	alutLoadWAVFile(fname, &format, &data, &size, &freq, &loop);
    alBufferData(buf, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);

	//Then check for any errors and return accordingly
	if (alGetError() != AL_NO_ERROR) {
		MessageBox(NULL, "Error creating buffer from file.", "DiZ Load Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}else {
		return true;
	}
}

//This function deletes our buffer
void DIZ_BUFFER::kill() {
	//Delete it
	alDeleteBuffers(1, &buf);
}

//This function sets our filename property to the path specified
void DIZ_BUFFER::setFilename(char f[]) {
	//Declare a quick iterative variable
	int i = 0;

	//While we haven't reached the end of our string
	while (f[i] != '\0') {
		//Set our filename character equal to the desired one and move along
		fname[i] = f[i];
		i++;
	}
	//Then just null-terminate our string
	fname[i] = '\0';
}