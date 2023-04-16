#include "AUDIO_SERVICE.h"

bool Initialize_AUDIO_SERVICE(AUDIO_SERVICE *audio, KCONTEXT *ctx) {
	audio->ctx = ctx;
	audio->main_loop = NULL;
	audio->main_loop_running = false;

	memset(audio->nal_buffer, 0, MAX_NAL_SIZE1);

	return true;
}

void Main_TCP_Loop_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
	int size;

	while (audio->main_loop_running) {
                if (Receive_CLIENT(audio->c, (char*) &size, sizeof(int)) &&
                        Receive_CLIENT(audio->c, audio->nal_buffer, size)) {
			
			//cout << "received audio bytes " << size << endl;
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
}

void Delete_AUDIO_SERVICE(AUDIO_SERVICE *audio) {
}
