/***********************************************
*Diz-sound.h by Chris Lewis
*Declares all the functions/classes for sound in DiZ
***********************************************/

//Check that this header file hasn't already been included
#ifndef DIZ_SOUND_H
#define DIZ_SOUND_H

//Declare any includes we need, checking for anything already define
#include "../Include/diz-list.h"	//Diz-list.h for our linked list class
#include <windows.h>				//Windows.h for general windows functions
#include <al/alut.h>				//Alut.h- includes al.h and alc.h for us
#include <ogg/ogg.h>				//Ogg.h- for our ogg resources
#include <vorbis/codec.h>			//Codec.h- required Vorbis header
#include <vorbis/vorbisenc.h>		//Vorbisenc.h- Vorbis header
#include <vorbis/vorbisfile.h>		//Vorbisfile.h- Vorbis header for Ogg file I/O

//Define a quick buffer size for streaming
#define DEFAULT_BUFFER_SIZE		(4096 * 8)

//This structure holds info related to an OpenAL Source
struct DIZ_SOURCEINFO {
	//The position and velocity arrays of the source
	ALfloat pos[3], vel[3];
	//The Pitch and Gain properties of the source
	ALfloat pitch, gain;
	//The Loop indicator (AL_TRUE or AL_FALSE)
	ALint loop;
	//And maintain a flag of whether or not our buffer's been set
	bool buf;
};

//This structure holds info for Ogg-streaming targets
struct DIZ_OGGSTREAMTARGET {
	//Declare our double-buffer setup IDs
	ALuint buffers[2];
	//Declare a pointer to our target AL source
	ALuint source;
	//Declare a variable for stream position tracking
	int location;
};

//This class will handle Ogg-file streams to sound buffers/sources
class DIZ_OGGSTREAM {
public:
	//Declare our Constructor and Destructor
	DIZ_OGGSTREAM();
	~DIZ_OGGSTREAM();

	//Declare our public functions
	//This function will load up a specified .ogg file
	bool loadFile(char fname[]);
	//This function will add a new source target to our list
	bool addTarget(ALuint src);
	//This function will update our streams
	bool update();
	//This function will destroy our class' info
	void kill();

private:
	//Declare our private functions
	//This function will stream new ogg data into a specified buffer, starting from a specified position
	bool streamToBuffer(ALuint buf, int *location);

	//Declare our private properties
	//Declare our OggVorbis file handle
	OggVorbis_File oggFile;
	//Declare a Vorbis Info pointer
	vorbis_info *vorbisInfo;
	//Declare a file-format variable
	ALenum format;
	//Declare our source targets list
	DIZ_LIST<DIZ_OGGSTREAMTARGET> targets;
};

//This class contains functions and properties related to OpenAL Buffers
class DIZ_BUFFER {
public:
	//Declare our Constructor and Deconstructor
	DIZ_BUFFER();
	~DIZ_BUFFER();

	//Declare our public functions
	//This function creates the buffer from the file specified
	bool loadWAV();
	//This function deletes our buffer
	void kill();
	//This function sets our desired filename to the specified value
	void setFilename(char f[]);
	//This function will return our filename
	char *getFilename();

	//Declare our properties
	//The handle for the buffer
	ALuint buf;

private:
	//Declare our private properties
	//The filename of the file we want to use for our buffer
	char fname[256];
};

//This class handles all the source-related functions and properties
class DIZ_SOURCE {
public:
	//Declare our Constructor and Deconstructor
	DIZ_SOURCE();
	~DIZ_SOURCE();

	//Declare our public functions
	//This function sets the specified buffer for the source to use
	bool setBuffer(ALuint buf);
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

private:
	//Declare our private properties
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

private:
	//Declare our private functions
	//This function will set up our ALC contexts and so on
	void init();

	//Declare our private properties
	//Declare our ALC Device handle
	ALCdevice *hDevice;
	//Declare our ALC context handle
	ALCcontext *hContext;
};

#endif