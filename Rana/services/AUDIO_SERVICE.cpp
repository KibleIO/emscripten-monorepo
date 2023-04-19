#include "AUDIO_SERVICE.h"

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE *audio, KCONTEXT *ctx) {
	audio->ctx = ctx;
	audio->main_loop = NULL;
	audio->main_loop_running = false;

	memset(audio->nal_buffer, 0, MAX_NAL_SIZE);

	int error;

	audio->decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error);
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
	if ((audio->audioDevice = SDL_OpenAudioDevice(
			 NULL, 0, &desiredSpec, &obtainedSpec, 0)) == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;
	while (audio->main_loop_running) {
		if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
			if (size == 11) {
				continue;
			}
			int num_samples = opus_decode(
				audio->decoder, (uint8_t *)audio->nal_buffer + 8,
				((int *)audio->nal_buffer)[1], audio->pcm, FRAME_SIZE, 0);
			if (num_samples < 0) {
				fprintf(stderr, "opus_decode error: %s\n",
						opus_strerror(num_samples));
				continue;
			}

			SDL_QueueAudio(audio->audioDevice, audio->pcm,
						   num_samples * CHANNELS * sizeof(opus_int16));
		}
	}
}

bool Connect_AUDIO_SERVICE(AUDIO_SERVICE *audio, CLIENT *c) {
	audio->c = c;

	ASSERT_E_R(audio->c != NULL, "client is NULL", audio->ctx);

	audio->main_loop_running = true;

	SDL_PauseAudioDevice(audio->audioDevice, 0);
	audio->main_loop = new thread(Main_TCP_Loop_AUDIO_SERVICE, audio);

	return true;
}

void Disconnect_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	audio->main_loop_running = false;
	if (audio->main_loop != NULL) {
		audio->main_loop->join();
		delete audio->main_loop;
		audio->main_loop = NULL;
	}
	opus_decoder_destroy(audio->decoder);
	SDL_CloseAudioDevice(audio->audioDevice);
	SDL_Quit();
}

void Delete_AUDIO_SERVICE(AUDIO_SERVICE *audio) {}
