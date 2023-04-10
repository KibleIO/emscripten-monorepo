#include "VIDEO_SERVICE.h"

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE *video, KCONTEXT *ctx) {
	video->ctx = ctx;
	video->main_loop = NULL;
	video->main_loop_running = false;


	memset(video->nal_buffer, 0, MAX_NAL_SIZE);

	return true;
}

void Main_TCP_Loop_VIDEO_SERVICE(VIDEO_SERVICE *video) {
	int size;

	while (video->main_loop_running) {
                if (Receive_CLIENT(video->c, (char*) &size, sizeof(int)) &&
                        Receive_CLIENT(video->c, video->nal_buffer, size)) {
			
			cout << "received video bytes " << size << endl;
                }
	}
}

bool Resize_VIDEO_SERVICE(VIDEO_SERVICE *video, int width, int height) {
	return true;
}

bool Status_VIDEO_SERVICE(VIDEO_SERVICE *video) {
	return video->main_loop_running;
}

bool Connect_VIDEO_SERVICE(VIDEO_SERVICE *video, CLIENT *video_init,
	CLIENT *c) {
	/*screen_dim_scaled.sw *= SCALE_RATIO;
	screen_dim_scaled.bw *= SCALE_RATIO;
	screen_dim_scaled.h *= SCALE_RATIO;*/
	video->c = c;
	int size;

	ASSERT_E_R(video_init != NULL, "video_init is NULL", video->ctx);
	ASSERT_E_R(video->c != NULL, "Client is NULL", video->ctx);

	ASSERT_E_R(Receive_CLIENT(video_init, (char*) &size, sizeof(int)),
                "Failed to receive headers", video->ctx);
        ASSERT_E_R(Receive_CLIENT(video_init, video->nal_buffer, size),
                "Failed to receive headers", video->ctx);
        video->main_loop_running = true;

        video->main_loop = new thread(Main_TCP_Loop_VIDEO_SERVICE, video);

	return true;
}

void Disconnect_VIDEO_SERVICE(VIDEO_SERVICE *video) {
	video->main_loop_running = false;
	if (video->main_loop != NULL) {
		video->main_loop->join();
		delete video->main_loop;
		video->main_loop = NULL;
	}
}

void Delete_VIDEO_SERVICE(VIDEO_SERVICE *video) {
}
