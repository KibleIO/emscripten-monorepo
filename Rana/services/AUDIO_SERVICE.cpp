#include "AUDIO_SERVICE.h"

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE *audio, KCONTEXT *ctx) {
	audio->ctx = ctx;
	audio->main_loop = NULL;
	audio->main_loop_running = false;

	memset(audio->nal_buffer, 0, MAX_NAL_SIZE);

	int error;
	audio->decoder = opus_decoder_create(48000, 2, &error);
	if (error != OPUS_OK) {
		return false;
	}

	return true;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;

	while (audio->main_loop_running) {
		if (Receive_CLIENT(audio->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
			opus_int16 *pcm;
			int frame_size;

			int num_samples = opus_decode(
				audio->decoder, (const unsigned char *)audio->nal_buffer, size,
				pcm, frame_size, 0);
			if (num_samples < 0) {
				cout << "error when decoding bytes" << endl;
			}
		}
	}
}

bool Connect_AUDIO_SERVICE(AUDIO_SERVICE *audio, CLIENT *c) {
	audio->c = c;

	ASSERT_E_R(audio->c != NULL, "client is NULL", audio->ctx);

	audio->main_loop_running = true;
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
}

void Delete_AUDIO_SERVICE(AUDIO_SERVICE *audio) {}
