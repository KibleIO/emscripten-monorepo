#include "AUDIO_CLIENT.h"

void Recv_Callback_AUDIO_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	AUDIO_CLIENT *audio = (AUDIO_CLIENT*) user_ptr;

	if (buffer_size == 11) {
		return;
	}

	int num_samples = opus_decode(audio->decoder, (uint8_t *)buffer + 8,
		((int *)buffer)[1], audio->pcm, FRAME_SIZE, 0);
	if (num_samples < 0) {
		fprintf(stderr, "opus_decode error: %s\n",
				opus_strerror(num_samples));
		return;
	}

	SDL_QueueAudio(audio->audioDevice, audio->pcm,
		num_samples * CHANNELS * sizeof(opus_int16));
}

bool AUDIO_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	ctx = ctx;
	//main_loop = NULL;
	main_loop_running = false;

	memset(nal_buffer, 0, MAX_NAL_SIZE1);

	int error;

	decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error);
	if (error != OPUS_OK) {
		fprintf(stderr, "Failed to create Opus decoder: %s\n",
				opus_strerror(error));
		return false;
	}

	// Set the desired audio format
	SDL_AudioSpec desiredSpec, obtainedSpec;
	SDL_zero(obtainedSpec);
	SDL_zero(desiredSpec);
	desiredSpec.freq = SAMPLE_RATE;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = CHANNELS;
	desiredSpec.samples = FRAME_SIZE;
	desiredSpec.callback = NULL;	 // Set the audio callback function
	desiredSpec.userdata = NULL;

	// Open the audio device
	if ((audioDevice = SDL_OpenAudioDevice(
			 NULL, 0, &desiredSpec, &obtainedSpec, 0)) == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return false;
	}

	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_AUDIO_CLIENT, registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	SDL_PauseAudioDevice(audioDevice, 0);

	return true;
}

void AUDIO_CLIENT::Delete() {
	opus_decoder_destroy(decoder);
	SDL_CloseAudioDevice(audioDevice);
	SDL_Quit();
}
