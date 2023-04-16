#ifndef AUDIO_SERVICE_H_
#define AUDIO_SERVICE_H_

#include <SDL2/SDL.h>

#include "/Users/kord/Documents/source.nosync/opus/include/opus.h"
#include <thread>
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../Rana_Core_Utils/Hermes/CLIENT.h"

#define MAX_NAL_SIZE 2000
#define SAMPLE_RATE 48000 // Sample rate in Hz
#define CHANNELS 2 // Number of audio channels (1 for mono, 2 for stereo)
#define SAMPLES_PER_FRAME 960 // Number of samples per audio frame


struct AUDIO_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
	char nal_buffer[MAX_NAL_SIZE];
	opus_int16 pcm[SAMPLES_PER_FRAME*CHANNELS*sizeof(opus_int16)];
	OpusDecoder *decoder;
	SDL_AudioDeviceID audioDevice;
};

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE*, KCONTEXT*);
bool Connect_AUDIO_SERVICE(AUDIO_SERVICE*, CLIENT*);
void Disconnect_AUDIO_SERVICE(AUDIO_SERVICE*);
void Delete_AUDIO_SERVICE(AUDIO_SERVICE*);

#endif
