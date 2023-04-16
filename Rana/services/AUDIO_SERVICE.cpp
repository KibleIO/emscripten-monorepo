#include "AUDIO_SERVICE.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
    AUDIO_SERVICE *audio = (AUDIO_SERVICE *)userdata;

    int size;
    if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
        Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
        int num_samples = opus_decode(
            audio->decoder, (const unsigned char *)audio->nal_buffer, size,
            audio->pcm, 960, 0);
        if (num_samples < 0) {
            // cout << "error when decoding bytes" <<  endl;
        } else {
            memcpy(stream, audio->pcm, len); // Fill the audio buffer with samples
        }
    }
}

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE *audio, KCONTEXT *ctx) {
	audio->ctx = ctx;
	audio->main_loop = NULL;
	audio->main_loop_running = false;

	memset(audio->nal_buffer, 0, MAX_NAL_SIZE);

	int error;
	audio->decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error);
	if (error != OPUS_OK) {
		return false;
	}

	// Set the desired audio format
	SDL_AudioSpec desiredSpec = {0};
	desiredSpec.freq = SAMPLE_RATE;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = CHANNELS;
	desiredSpec.samples = SAMPLES_PER_FRAME;
	desiredSpec.callback = audio_callback; // Set the audio callback function


	// Open the audio device
	SDL_AudioSpec obtainedSpec = {0};
	audio->audioDevice =
		SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, 0);
	if (audio->audioDevice == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return 1;
	}
	SDL_PauseAudio(0);
	return true;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;

	while (audio->main_loop_running) {
		if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
			int num_samples = opus_decode(
				audio->decoder, (const unsigned char *)audio->nal_buffer, size,
				audio->pcm, 960, 0);
			if (num_samples < 0) {
				// cout << "error when decoding bytes" <<  endl;
			} else {
				SDL_QueueAudio(audio->audioDevice, audio->pcm, sizeof(audio->pcm));
				// SDL_Delay(SAMPLES_PER_FRAME * 1000 / SAMPLE_RATE);
			}
		}
	}
}

bool Connect_AUDIO_SERVICE(AUDIO_SERVICE *audio, CLIENT *c) {
	audio->c = c;

	ASSERT_E_R(audio->c != NULL, "client is NULL", audio->ctx);

	audio->main_loop_running = true;
	
	// audio->main_loop = new thread(Main_TCP_Loop_AUDIO_SERVICE, audio);

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
