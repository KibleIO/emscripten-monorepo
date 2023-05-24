#include "AUDIO_CLIENT.h"

void Recv_Callback_AUDIO_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	//std::cout << "received bytes " << buffer_size << std::endl;
}

bool AUDIO_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio1 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

	ctx = ctx;
	main_loop = NULL;
	main_loop_running = false;

	memset(nal_buffer, 0, MAX_NAL_SIZE1);

	int error;

	decoder = opus_decoder_create(SAMPLE_RATE, CHANNELS, &error);
	if (error != OPUS_OK) {
		fprintf(stderr, "Failed to create Opus decoder: %s\n",
				opus_strerror(error));
		return false;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio2 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


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

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio3 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	// Open the audio device
	if ((audioDevice = SDL_OpenAudioDevice(
			 NULL, 0, &desiredSpec, &obtainedSpec, 0)) == 0) {
		printf("SDL_OpenAudioDevice() failed: %s\n", SDL_GetError());
		return false;
	}

	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_AUDIO_CLIENT, &registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio41 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());

	SDL_PauseAudioDevice(audioDevice, 0);

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio42 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	main_loop_running = true;

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio43 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());



	//main_loop = new thread(Main_TCP_Loop_AUDIO_CLIENT, this);

	printf("Total memory: %u bytes\n", getTotalMemory());
	printf("Free memory: %u bytes\n", getFreeMemory());
	printf("audio5 Used: %u bytes (%.2f%%)\n", getTotalMemory() - getFreeMemory(), (getTotalMemory() - getFreeMemory()) * 100.0 / getTotalMemory());


	return true;
}

void AUDIO_CLIENT::Delete() {
	main_loop_running = false;
	if (main_loop != NULL) {
		main_loop->join();
		delete main_loop;
		main_loop = NULL;
	}
	opus_decoder_destroy(decoder);
	SDL_CloseAudioDevice(audioDevice);
	SDL_Quit();
}

void Main_TCP_Loop_AUDIO_CLIENT(AUDIO_CLIENT *audio) {
	int size;
	while (audio->main_loop_running) {
		Sleep_Milli(1000);
		/*
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
		*/
	}
}
