/****************************************
*Diz-source.cpp by Chris Lewis
*Defines the DIZ_SOURCE class from diz-sound.h
****************************************/

//Declare our include file
#include "diz-sound.h"

//Define our Constructor- generates our source and sets everything to default values
DIZ_SOURCE::DIZ_SOURCE() {
	//First we attempt to generate our source
	alGenSources(1, &src);
	//Then check for errors and give a warning if necessary
	if (alGetError() != AL_NO_ERROR) {
		MessageBox(NULL, "Error Generating Source.", "DiZ Sound Error", MB_OK | MB_ICONINFORMATION);
	}

	//Set our position and velocity arrays to default zeros
	info.pos[0] = 0.0f; info.pos[1] = 0.0f; info.pos[2] = 0.0f;
	info.vel[0] = 0.0f; info.vel[1] = 0.0f; info.vel[2] = 0.0f;
	//Set our pitch and gain to normal values
	info.gain = 1.0f;
	info.pitch = 1.0f;
	//And set looping to default false
	info.loop = AL_FALSE;
}

//Define our Deconstructor
DIZ_SOURCE::~DIZ_SOURCE() {

}

//This function sets the source to use the specified buffer
bool DIZ_SOURCE::setBuffer(ALint buf) {
	alSourcei(src, AL_BUFFER, buf);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}else {
		return true;
	}
}

//This function sets all our properties for our OpenAL source
bool DIZ_SOURCE::update() {
	//Set our Position
	alSourcefv(src, AL_POSITION, info.pos);
	//Set our Velocity
	alSourcefv(src, AL_VELOCITY, info.vel);
	//Set our Pitch
	alSourcef(src, AL_PITCH, info.pitch);
	//Set our Gain
	alSourcef(src, AL_GAIN, info.gain);
	//Set our Loop setting
	alSourcei(src, AL_LOOPING, info.loop);

	//Check for any errors and return accordingly
	if (alGetError() != AL_NO_ERROR) {
		MessageBox(NULL, "Error setting Source values.", "DiZ Sound Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}else {
		return true;
	}
}

//This function starts the source playing
bool DIZ_SOURCE::play() {
	//Play the source and check for errors
	alSourcePlay(src);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}else {
		return true;
	}
}

//This function stops the source playing
bool DIZ_SOURCE::stop() {
	//Stop the source and check for errors
	alSourceStop(src);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}else {
		return true;
	}
}

//This function pauses the source
bool DIZ_SOURCE::pause() {
	//Pause the source and check for errors
	alSourcePause(src);
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}else {
		return true;
	}
}

//This function deletes the source
void DIZ_SOURCE::kill() {
	//Delete our source
	alDeleteSources(1, &src);
}