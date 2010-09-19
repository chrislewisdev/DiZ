/**************************************
*Diz-listener.cpp by Chris Lewis
*Defines the DIZ_LISTENER class from diz-sound.h
**************************************/

//Declare our include file
#include "../Include/diz-sound.h"

//Define our Constructor, initialise everything and start up our alut session
DIZ_LISTENER::DIZ_LISTENER() {
	//Set our default Position values
	pos[0] = 0.0f; pos[1] = 0.0f; pos[2] = 0.0f;
	//Set our default Velocity values
	vel[0] = 0.0f; vel[1] = 0.0f; vel[2] = 0.0f;
	//Set our default Orientation values
	ori[0] = 0.0f; ori[1] = 0.0f; ori[2] = -1.0f; ori[3] = 0.0f; ori[4] = 1.0f; ori[5] = 0.0f;
	//Finally, initialise alut so all our sound will work
	alutInit(NULL, NULL);
}

//Define our Deconstructor
DIZ_LISTENER::~DIZ_LISTENER() {

}

//This function sets the OpenAL Listener values to our values
bool DIZ_LISTENER::update() {
	//Set our main values- Position
	alListenerfv(AL_POSITION, pos);
	//Velocity
	alListenerfv(AL_VELOCITY, vel);
	//Orientation
	alListenerfv(AL_ORIENTATION, ori);

	//Then check for errors and return accordingly
	if (alGetError() != AL_NO_ERROR) {
		MessageBox(NULL, "Error setting Listener values.", "Diz Sound Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}else {
		return true;
	}
}

//This function exits our alut session and so on
void DIZ_LISTENER::kill() {
	//Exit alut
	alutExit();
}