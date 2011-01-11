/************************************************************
*Diz-OggStream.cpp by Chris Lewis
*Defines the DIZ_OGGSTREAM class in Diz-sound.h
************************************************************/

//Include our Sound header
#include "../Include/diz-sound.h"

//Declare our Constructor
DIZ_OGGSTREAM::DIZ_OGGSTREAM() {
	
}

//Declare our Destructor
DIZ_OGGSTREAM::~DIZ_OGGSTREAM() {

}

//This function will open up a specified .ogg file
bool DIZ_OGGSTREAM::loadFile(char fname[]) {
	//Declare an error-checking variable
	int result;

	//Attempt to open the specified file and check for an error
	result = ov_fopen(fname, &oggFile);
	if (result < 0) {
		return false;
	}

	//Retrieve some info about our file
	vorbisInfo = ov_info(&oggFile, -1);

	//Check our channels info and manually set our format accordingly
	if (vorbisInfo->channels == 1) {
		format = AL_FORMAT_MONO16;
	}else {
		format = AL_FORMAT_STEREO16;
	}

	//And return true for A-OK
	return true;
}

//This function will add a new source target to our stream
bool DIZ_OGGSTREAM::addTarget(ALuint source) {
	//Declare a stream-target pointer to a new item in our list
	DIZ_OGGSTREAMTARGET *newTarget = targets.addItem();

	//Check that the new item succeeded
	if (newTarget != NULL) {
		//Generate our new buffers in our stream-target
		alGenBuffers(2, newTarget->buffers);
		//Set our source target to the specified ID
		newTarget->source = source;
		//And set our stream-location to 0
		newTarget->location = 0;

		//Now attempt to stream into both of our new buffers
		if (!streamToBuffer(newTarget->buffers[0], &newTarget->location)) {
			return false;
		}
		if (!streamToBuffer(newTarget->buffers[1], &newTarget->location)) {
			return false;
		}

		//And finally queue up our buffers on our source ready to be played
		alSourceQueueBuffers(newTarget->source, 2, newTarget->buffers);
	}else {
		return false;
	}

	//Then, check that no OpenAL errors occurred along the way
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}else {
		return true;
	}
}

//This function will update our streams
bool DIZ_OGGSTREAM::update() {
	//Declare a conductor variable set our first target item
	DIZ_OGGSTREAMTARGET *cdtr = targets.iterate(true);
	//Declare a storage variable for no. of processed buffers in each source
	int processedBuffers;
	//Declare a buffer-ID storage variable
	ALuint buf;
	//Declare a result-tracking variable
	bool results = true;

	//Loop while we still have items to move through
	while (cdtr) {
		//First retrieve our no. of processed buffers in our current source
		alGetSourcei(cdtr->source, AL_BUFFERS_PROCESSED, &processedBuffers);

		//Loop until we've replaced all the buffers
		while (processedBuffers > 0) {
			//Pop our finished buffers of the source queue
			alSourceUnqueueBuffers(cdtr->source, 1, &buf);
			//Then fill it up with new data, with a quick error check
			if (!streamToBuffer(buf, &cdtr->location)) {
				results = false;
			}
			//Then put it back on the end of the queue
			alSourceQueueBuffers(cdtr->source, 1, &buf);

			//Then decrement our counter
			processedBuffers--;
		}

		//Check that we have a next item to move onto, otherwise exit our loop
		cdtr = targets.iterate(false);
	}

	//Finally, check for any OpenAL errors
	if (alGetError() != AL_NO_ERROR) {
		results = false;
	}

	//And return our result- false means one or more streams failed
	return results;
}

//This function will close our class' properties
void DIZ_OGGSTREAM::kill() {
	//Declare a conductor object set to our first item
	DIZ_OGGSTREAMTARGET *cdtr = targets.iterate(true);
	//Declare a storage variable for our queued buffers
	int queuedBuffers;
	//Declare a storage variable for buffer-IDs
	ALuint buf;

	//Loop as long as we have items left
	while (cdtr) {
		//Stop our target source (just in case)
		alSourceStop(cdtr->source);
		//Retrieve the amount of queued buffers on our source
		alGetSourcei(cdtr->source, AL_BUFFERS_QUEUED, &queuedBuffers);

		//Loop for as many buffers as we need
		while (queuedBuffers > 0) {
			//Unqueue our left-over buffers on this source
			alSourceUnqueueBuffers(cdtr->source, 1, &buf);

			//Then decrement our counter
			queuedBuffers--;
		}

		//Check if we have any more items, otherwise exit our loop
		cdtr = targets.iterate(false);
	}

	//Close our Ogg file
	ov_clear(&oggFile);
	//And then clear out our list
	targets.kill();
}

//This function will stream new Ogg data to a specified buffer at a specified position
bool DIZ_OGGSTREAM::streamToBuffer(ALuint buf, int *location) {
	//Declare a stream-data buffer
	char data[DEFAULT_BUFFER_SIZE];
	//Declare our tracking variables for our streaming progress
	int size = 0, result;
	//And declare a bitstream variable
	int bitstream;

	//First, seek to our desired starting position in the stream
	ov_pcm_seek(&oggFile, *location);
	//Loop until we've filled up our desired buffer size
	while (size < DEFAULT_BUFFER_SIZE) {
		//Read as much new data as we can into the buffer
		result = ov_read(&oggFile, data + size, DEFAULT_BUFFER_SIZE - size, 0, 2, 1, &bitstream);
		//Now set our stream-location to our current spot
		*location = (int)ov_pcm_tell(&oggFile);
		
		//If our result was positive, it's our amount of data read
		if (result > 0) {
			size += result;
		//If our result was negative, it's an error
		}else if (result < 0) {
			return false;
		//If our result was 0, we've hit the end of the file
		}else {
			break;
		}
	}

	//If size is 0, then we're obviously trying to read from the end of the file, so return false
	if (size == 0) {
		return false;
	}

	//Then load our new data into our buffer
	alBufferData(buf, format, data, size, vorbisInfo->rate);

	//Then double-check for an OpenAL error
	if (alGetError() != AL_NO_ERROR) {
		return false;
	}

	//And exit
	return true;
}