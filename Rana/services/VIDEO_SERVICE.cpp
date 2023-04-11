#include "VIDEO_SERVICE.h"

SDL_Texture *texture = NULL;
int width = 1920;
int height = 816;

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE *video, KCONTEXT *ctx) {
	video->ctx = ctx;
	video->main_loop = NULL;
	video->main_loop_running = false;

	memset(video->nal_buffer, 0, MAX_NAL_SIZE);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&video->decoder, 1, 0, 0, 0);
	video->byteStrmStart = broadwayCreateStream(&video->decoder, MAX_NAL_SIZE);

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Create an SDL window and renderer
	video->window = SDL_CreateWindow("YUV Rendering", SDL_WINDOWPOS_UNDEFINED,
									 SDL_WINDOWPOS_UNDEFINED, width, height,
									 SDL_WINDOW_OPENGL);
	video->renderer = SDL_CreateRenderer(video->window, -1, 0);

	if (video->renderer == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
	}

	// Create an SDL texture
	texture = SDL_CreateTexture(video->renderer, SDL_PIXELFORMAT_IYUV,
								SDL_TEXTUREACCESS_STREAMING, width,
								height);
	return true;
}

void yuv_to_pixels(uint8_t *data, size_t size) {
	// Lock the texture to get a pointer to the texture pixels
	void *pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	// Copy the YUV bytes to the texture pixels pointer
	int y_size = width * height;
	int u_size = y_size / 4;
	int v_size = y_size / 4;
	unsigned char *y = data;
	unsigned char *u = data + y_size;
	unsigned char *v = data + y_size + u_size;
	unsigned char *dst = (unsigned char *)pixels;
	int dst_pitch = pitch;
	for (int i = 0; i < height; i++) {
		memcpy(dst, y, width);
		dst += dst_pitch;
		y += width;
	}
	dst = (unsigned char *)pixels + height * dst_pitch;
	for (int i = 0; i < height / 2; i++) {
		memcpy(dst, u, width / 2);
		dst += dst_pitch / 2;
		u += width / 2;
	}
	dst = (unsigned char *)pixels + height * dst_pitch * 5 / 4;
	for (int i = 0; i < height / 2; i++) {
		memcpy(dst, v, width / 2);
		dst += dst_pitch / 2;
		v += width / 2;
	}

	// Unlock the texture
	SDL_UnlockTexture(texture);
}

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height) {
	yuv_to_pixels(buffer, (width * height * 3) / 2);
}

extern void broadwayOnHeadersDecoded() { printf("header decoded"); }

void Main_TCP_Loop_VIDEO_SERVICE(void *arg) {
	VIDEO_SERVICE *video = (VIDEO_SERVICE *)arg;
	int size;
	if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
		Receive_CLIENT(video->c, video->nal_buffer, size)) {
		video->decoder.decInput.pStream = (u8*)video->nal_buffer;
		video->decoder.decInput.dataLen = size;

		// Clear the renderer
		SDL_RenderClear(video->renderer);
		// Copy the texture to the renderer
		SDL_RenderCopy(video->renderer, texture, NULL, NULL);
		// Render the texture to the screen
		SDL_RenderPresent(video->renderer);

		if (!video->stop) {
			// playStream(&dec, &dec.broadwayStream);
			u8 *start = video->decoder.decInput.pStream;
			u32 ret = broadwayDecode(&video->decoder);
			printf("Decoded Unit #%d, Size: %d, Result: %d\n", video->counter++,
				   (video->decoder.decInput.pStream - start), ret);
		}

		if (video->decoder.decInput.dataLen <= 0) {
			video->stop = 1;
		}
		cout << "received video bytes " << size << endl;
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

	ASSERT_E_R(Receive_CLIENT(video_init, (char *)&size, sizeof(int)),
			   "Failed to receive headers", video->ctx);
	ASSERT_E_R(Receive_CLIENT(video_init, video->nal_buffer, size),
			   "Failed to receive headers", video->ctx);
	video->main_loop_running = true;
	
	emscripten_set_main_loop_arg(
		[](void *arg) { Main_TCP_Loop_VIDEO_SERVICE(arg); }, video, 30, 1);
	// video->main_loop = new thread(Main_TCP_Loop_VIDEO_SERVICE, video);

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

void Delete_VIDEO_SERVICE(VIDEO_SERVICE *video) {}
