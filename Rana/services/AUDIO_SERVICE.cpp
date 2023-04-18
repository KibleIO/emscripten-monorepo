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
	Sleep_Milli(5000);
	// Set the desired audio format
	SDL_AudioSpec desiredSpec, obtainedSpec;
	SDL_zero(obtainedSpec);
	SDL_zero(desiredSpec);
	desiredSpec.freq = SAMPLE_RATE;
	desiredSpec.format = AUDIO_S16SYS;
	desiredSpec.channels = CHANNELS;
	// desiredSpec.samples = FRAME_SIZE;
	desiredSpec.callback = NULL;  // Set the audio callback function

	// Open the audio device
	audio->audioDevice =
		SDL_OpenAudioDevice(NULL, 0, &desiredSpec, &obtainedSpec, 0);
	if (audio->audioDevice == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return 1;
	}
	return true;
}

int op_read(FILE *file, unsigned char *buffer, int max_length) {
	int packet_size = 0;
	if (fread(&packet_size, sizeof(packet_size), 1, file) != 1) {
		return 0;
	}
	cout << packet_size << endl;

	if (packet_size > max_length) {
		return -1;
	}
	if (fread(buffer, 1, packet_size, file) != packet_size) {
		return -1;
	}
	return packet_size;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;
	FILE *infile = fopen("test.opus", "rb");
	if (infile == NULL) {
		printf("Error opening input file\n");
		return;
	}

	fseek(infile, OPUS_HEAD_SIZE, SEEK_SET);
	// unsigned char encoded[FRAME_SIZE * CHANNELS * sizeof(opus_int16)];
	unsigned char encoded[MAX_FRAME_SIZE];
	opus_int32 nbytes;
	opus_int16 pcm[FRAME_SIZE * CHANNELS];
	while (!feof(infile)) {
		int packet_size = fgetc(infile);
		if (packet_size == EOF) {
			cout << "EOF" << endl;
			break;	// End of file
		}
		cout << packet_size << endl;
		if (fread(encoded, 1, packet_size, infile) != packet_size) {
			printf("Error: could not read Opus packet\n");
			break;
		}
		// nbytes = op_read(infile, encoded, SAMPLE_RATE);
		// nbytes = fread(encoded, sizeof(unsigned char),
		// 			   FRAME_SIZE * CHANNELS * sizeof(opus_int16), infile);
		// if (nbytes < 0) {
		// 	fprintf(stderr, "fread error: %s\n");
		// 	break;
		// }
		int frame_size =
			opus_decode(audio->decoder, encoded, packet_size, pcm, FRAME_SIZE, 0);
		if (frame_size < 0) {
			fprintf(stderr, "opus_decode error: %s\n",
					opus_strerror(frame_size));
			break;
		}
		// cout << "bytes: " << nbytes << endl;
		cout << "frame size: " << frame_size << endl;
		int error = SDL_QueueAudio(audio->audioDevice, pcm,
								   frame_size * sizeof(opus_int16));
		if (error < 0) {
			cout << "hi: " << SDL_GetError() << endl;
		}
	}
	while (SDL_GetQueuedAudioSize(audio->audioDevice) > 0) {
		cout << "spin" << endl;
		SDL_Delay(100);
	}
	cout << "end" << endl;
	// while (audio->main_loop_running) {
	// 	if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
	// 		Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
	// 		cout << size << endl;
	// 		int num_samples = opus_decode(
	// 			audio->decoder, (const unsigned char *)audio->nal_buffer, size,
	// 			audio->pcm, MAX_FRAME_SIZE, 0);
	// 		if (num_samples < 0) {
	// 			fprintf(stderr, "opus_decode error: %s\n",
	// 					opus_strerror(num_samples));
	// 			continue;
	// 		}
	// 		SDL_QueueAudio(audio->audioDevice, audio->pcm,
	// 					   num_samples * CHANNELS * sizeof(opus_int16));
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
