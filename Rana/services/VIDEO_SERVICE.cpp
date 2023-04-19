#include "VIDEO_SERVICE.h"

#include "../Utilities/TIMER.h"

SDL_Texture *texture = NULL;
int width = 800;
int height = 600;

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE *video, KCONTEXT *ctx,
							  MOUSE_SERVICE *mouse_service, KEYBOARD_SERVICE *keyboard_service) {
	video->ctx = ctx;
	video->main_loop = NULL;
	video->main_loop_running = false;
	video->mouse_service = mouse_service;
	video->keyboard_service = keyboard_service;
	video->mouse_count = 1;
	video->keyboard_count = 1;

	memset(video->nal_buffer, 0, MAX_NAL_SIZE);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&video->decoder, 0, 0, 0, 0);
	video->byteStrmStart = broadwayCreateStream(&video->decoder, MAX_NAL_SIZE);

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

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
								SDL_TEXTUREACCESS_STREAMING, width, height);
	return true;
}

void yuv_to_pixels(uint8_t *src, u32 src_width, u32 src_height) {
	// Lock the texture to get a pointer to the texture pixels
	uint8_t *dst;
	int dst_width;
	int dst_height = height;
	SDL_LockTexture(texture, NULL, (void **)&dst, &dst_width);

	// Copy the YUV bytes to the texture pixels pointer

	int src_y_size = src_width * src_height;
	int src_u_size = src_y_size / 4;
	int src_v_size = src_y_size / 4;
	unsigned char *src_y = src;
	unsigned char *src_u = src + src_y_size;
	unsigned char *src_v = src + src_y_size + src_u_size;

	int dst_y_size = dst_width * dst_height;
	int dst_u_size = dst_y_size / 4;
	int dst_v_size = dst_y_size / 4;
	unsigned char *dst_y = dst;
	unsigned char *dst_u = dst + dst_y_size;
	unsigned char *dst_v = dst + dst_y_size + dst_u_size;

	for (int i = 0; i < height; i++) {
		memcpy(dst_y, src_y, src_width);
		dst_y += dst_width;
		src_y += src_width;
	}

	for (int i = 0; i < height / 2; i++) {
		memcpy(dst_u, src_u, src_width / 2);
		dst_u += dst_width / 2;
		src_u += src_width / 2;
	}

	for (int i = 0; i < height / 2; i++) {
		memcpy(dst_v, src_v, src_width / 2);
		dst_v += dst_width / 2;
		src_v += src_width / 2;
	}

	// Unlock the texture
	SDL_UnlockTexture(texture);
}

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height) {
	yuv_to_pixels(buffer, width, height);
}

extern void broadwayOnHeadersDecoded() { printf("header decoded\n"); }

TIMER t;

void Main_TCP_Loop_VIDEO_SERVICE(/*void *arg*/VIDEO_SERVICE *video) {
	//cout << "time: " << Stop_TIMER(&t) << endl;

	//============== < 1ms
	//VIDEO_SERVICE *video = (VIDEO_SERVICE *)arg;

	MOUSE_EVENT_T m_event;
	KEYBOARD_EVENT_T k_event;

	while (video->main_loop_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				m_event.x = ((float)event.button.x) /* * sdl->x_scale*/;
				m_event.y = ((float)event.button.y) /* * sdl->y_scale*/;
				m_event.clicked = false;
				m_event.state = MOUSE_ABS_COORD;
				m_event.event_index = video->mouse_count++;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));

				break;
			case SDL_MOUSEBUTTONDOWN:
				m_event.x = ((float)event.button.x) /* * sdl->x_scale*/;
				m_event.y = ((float)event.button.y) /* * sdl->y_scale*/;
				m_event.clicked = true;
				m_event.state = 1;	// pressed
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEBUTTONUP:
				m_event.x = ((float)event.button.x) /* * sdl->x_scale*/;
				m_event.y = ((float)event.button.y) /* * sdl->y_scale*/;
				m_event.clicked = true;
				m_event.state = 0;	// released
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
				break;
			case SDL_KEYDOWN:
				k_event.code = event.key.keysym.sym;
				k_event.value = 1;
				k_event.event_index = video->keyboard_count++;
				Send_CLIENT(video->keyboard_service->c,
					(char*) &k_event,
					sizeof(KEYBOARD_EVENT_T));
				break;
			case SDL_KEYUP:
				k_event.code = event.key.keysym.sym;
				k_event.value = 0;
				k_event.event_index = video->keyboard_count++;
				Send_CLIENT(video->keyboard_service->c,
					(char*) &k_event,
					sizeof(KEYBOARD_EVENT_T));
				break;
			}
		}
		//==============
		
		//============== < 10ms && < 70ms
		int size;
		if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(video->c, video->nal_buffer, size)) {
			//============== < 5ms
			video->decoder.streamStop = (u8 *)video->nal_buffer + size;
			video->decoder.decInput.pStream = (u8 *)video->nal_buffer;
			video->decoder.decInput.dataLen = size;

			// Clear the renderer
			SDL_RenderClear(video->renderer);
			// Copy the texture to the renderer
			SDL_RenderCopy(video->renderer, texture, NULL, NULL);
			// Render the texture to the screen
			SDL_RenderPresent(video->renderer);
			// cout << "before" << endl;
			//==============

			//============== < 5ms
			u32 i = 0;
			do {
				u8 *start = video->decoder.decInput.pStream;
				u32 ret = broadwayDecode(&video->decoder);
				// printf("Decoded Unit #%d, Size: %d, Result: %d\n", i++,
				//	   (video->decoder.decInput.pStream - start), ret);
			} while (video->decoder.decInput.dataLen > 0);
			//==============
		}
		//==============
	}

	//Start_TIMER(&t);
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

	Main_TCP_Loop_VIDEO_SERVICE(video);
	
	//emscripten_set_main_loop_arg(
	//	[](void *arg) { Main_TCP_Loop_VIDEO_SERVICE(arg); }, video, 60, 1);
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
