#include "AUDIO_SERVICE.h"

void audio_callback(void *userdata, Uint8 *stream, int len) {
	AUDIO_SERVICE *audio = (AUDIO_SERVICE *)userdata;

	int size;
	if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
		Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
		int num_samples = opus_decode(audio->decoder,
									  (const unsigned char *)audio->nal_buffer,
									  size, audio->pcm, 960, 0);
		if (num_samples < 0) {
			// cout << "error when decoding bytes" <<  endl;
		} else {
			memcpy(stream, audio->pcm,
				   len);  // Fill the audio buffer with samples
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
        fprintf(stderr, "Failed to create Opus decoder: %s\n", opus_strerror(error));
        return false;
    }
	Sleep_Milli(5000);
	// Set the desired audio format
	SDL_AudioSpec desiredSpec = {0};
	desiredSpec.freq = SAMPLE_RATE;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = CHANNELS;
	desiredSpec.samples = SAMPLES_PER_FRAME / 2;
	// desiredSpec.callback = audio_callback; // Set the audio callback function

	// Open the audio device
	SDL_AudioSpec obtainedSpec = {0};
	audio->audioDevice =
		SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, 0);
	if (audio->audioDevice == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return 1;
	}
	return true;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;
	FILE *infile = fopen("test-2.opus", "rb");
	if (infile == NULL) {
		printf("Error opening input file\n");
		return;
	}
	int16_t header[22];
	fread(header, sizeof(int16_t), 22, infile);
	unsigned char compressed[SAMPLES_PER_FRAME];
	opus_int32 nbytes;
    while (!feof(infile)) {
        opus_int16 pcm[SAMPLES_PER_FRAME * CHANNELS * sizeof(opus_int16)];
        nbytes = fread(compressed, sizeof(unsigned char), SAMPLES_PER_FRAME, infile);
        if (nbytes <= 0) {
			cout << "error" << endl;
            break;
        }
        int frame_size = opus_decode(audio->decoder, compressed, nbytes, pcm, SAMPLES_PER_FRAME, 0);
		if (frame_size < 0) {
			fprintf(stderr, "opus_decode error: %s\n", opus_strerror(frame_size));
			continue;
		}
        SDL_QueueAudio(audio->audioDevice, pcm, frame_size * CHANNELS * sizeof(int16_t));
    }
	SDL_Delay(60000);
	cout << "end" << endl;
	// while (audio->main_loop_running) {
	// 	if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
	// 		Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
	// 		int num_samples = opus_decode(
	// 			audio->decoder, (const unsigned char *)audio->nal_buffer, size,
	// 			audio->pcm, 960, 0);
	// 		if (num_samples >= 0) {
	// 			SDL_QueueAudio(audio->audioDevice, audio->pcm,
	// 						   num_samples * CHANNELS * 2);
	// 		}
	// 	}
	// }
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
