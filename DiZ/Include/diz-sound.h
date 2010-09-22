/***********************************************
*Diz-sound.h by Chris Lewis
*Declares all the functions/classes for sound in DiZ
***********************************************/

//Check that this header file hasn't already been included
#ifndef DIZ_SOUND_H
#define DIZ_SOUND_H

//Declare any includes we need, checking for anything already define
#include <windows.h>		//Windows.h for general windows functions
#include <al/alut.h>		//Alut.h- includes al.h and alc.h for us

//This structure holds info related to an OpenAL Source
struct DIZ_SOURCEINFO {
	//The position and velocity arrays of the source
	float pos[3], vel[3];
	//The Pitch and Gain properties of the source
	float pitch, gain;
	//The Loop indicator (AL_TRUE or AL_FALSE)
	ALint loop;
	//And also keep a pointer to the buffer we want to use
	ALint *buf;
};

//This class contains functions and properties related to OpenAL Buffers
class DIZ_BUFFER {
public:
	//Declare our Constructor and Deconstructor
	DIZ_BUFFER();
	~DIZ_BUFFER();

	//Declare our public functions
	//This function creates the buffer from the file specified
	bool load();
	//This function deletes our buffer
	void kill();
	//This function sets our desired filename to the specified value
	void setFilename(char f[]);

	//Declare our properties
	//The filename of the file we want to use for our buffer
	char fname[256];
	//The handle for the buffer
	ALuint buf;
};

//This class handles all the source-related functions and properties
class DIZ_SOURCE {
public:
	//Declare our Constructor and Deconstructor
	DIZ_SOURCE();
	~DIZ_SOURCE();

	//Declare our public functions
	//This function sets the specified buffer for the source to use
	bool setBuffer(ALint buf);
	//This function updates the source's properties
	bool update();
	//This function starts the source playing
	bool play();
	//This function stops the source playing
	bool stop();
	//This function pauses the source
	bool pause();
	//This function will fade from a certain volume to another
	void fade(float start, float end, int loops);
	//This function deletes our source
	void kill();

	//Declare our properties
	//This INFO structure holds all the details for our source object
	DIZ_SOURCEINFO info;
	//The handle for the source
	ALuint src;
	//Declare our desired fading destination and our fading rate
	float fadeDest, fadeRate;
};

//This class handles the settings of the OpenAL Listener object
class DIZ_LISTENER {
public:
	//Declare our Constructor and Deconstructor
	DIZ_LISTENER();
	~DIZ_LISTENER();

	//Declare our public functions
	//This function updates the Listener's settings
	bool update();
	//This function ends our alut session and everything
	void kill();

	//Declare our position, velocity and orientation arrays
	float pos[3], vel[3], ori[6];

//private:
	//Declare our private properties
	//Declare our ALC Device handle
	ALCdevice *hDevice;
	//Declare our ALC context handle
	ALCcontext *hContext;
};

#endif