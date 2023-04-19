#ifndef AUDIO_SERVICE_H_
#define AUDIO_SERVICE_H_

#include <SDL2/SDL.h>

#include <thread>

#include "../Rana_Core_Utils/Hermes/CLIENT.h"
#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"
#include "../opus/include/opus.h"

#define FRAME_SIZE_MS 20
#define SAMPLE_RATE 48000  // Sample rate in Hz
#define CHANNELS 2	// Number of audio channels (1 for mono, 2 for stereo)
#define FRAME_SIZE (FRAME_SIZE_MS * SAMPLE_RATE / 1000)  // Number of samples per audio frame
#define MAX_FRAME_SIZE (6 * 960)
#define MAX_PACKET_SIZE ((3 * 1276) + 512)
#define BITRATE 109000
#define OPUS_HEAD_SIZE 385
#define MAX_NAL_SIZE1 2000

struct AUDIO_SERVICE {
	KCONTEXT *ctx;
	CLIENT *c;
	thread *main_loop;
	volatile bool main_loop_running;
	char nal_buffer[MAX_NAL_SIZE1];
	opus_int16 pcm[FRAME_SIZE * CHANNELS * sizeof(opus_int16)];
	OpusDecoder *decoder;
	SDL_AudioDeviceID audioDevice;
};

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE *, KCONTEXT *);
bool Connect_AUDIO_SERVICE(AUDIO_SERVICE *, CLIENT *);
void Disconnect_AUDIO_SERVICE(AUDIO_SERVICE *);
void Delete_AUDIO_SERVICE(AUDIO_SERVICE *);

#endif
