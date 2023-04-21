#include "VIDEO_SERVICE.h"

static EM_BOOL Emscripten_HandleResize(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
	VIDEO_SERVICE *video = (VIDEO_SERVICE*) userData;

	/*
	int width;
	int height;

	get_screen_width_height(&width, &height);
	*/

	SDL_Event event;
	event.type = SDL_WINDOWEVENT;
	event.window.event = SDL_WINDOWEVENT_RESIZED;
	//event.window.data1 = width;
	//event.window.data2 = height;
	SDL_PushEvent(&event);

	return 0;
}

void Report_Resize_Function_VIDEO_SERVICE(VIDEO_SERVICE *video) {
	int width;
	int height;

	get_screen_width_height(&width, &height);

	bool changed = false;

	if (width < MINIMUM_WIDTH) {
		width = MINIMUM_WIDTH;
		changed = true;
	}

	if (height < MINIMUM_HEIGHT) {
		height = MINIMUM_HEIGHT;
		changed = true;
	}

	if (width > MAXIMUM_WIDTH) {
		width = MAXIMUM_WIDTH;
		changed = true;
	}

	if (height > MAXIMUM_HEIGHT) {
		height = MAXIMUM_HEIGHT;
		changed = true;
	}

	if (width % 32 != 0) {
		width = (width / 32) * 32;
		changed = true;
	}

	if (height % 32 != 0) {
		height = (height / 32) * 32;
		changed = true;
	}

	//if (changed) {
		Set_Screen_Dim_KCONTEXT(video->ctx, (SCREEN_DIM) {
		width,
		width,
		height});
	//}
}

void Actually_Resize_Window_VIDEO_SERVICE(VIDEO_SERVICE *video, int width, int height) {
	video->width = width;
	video->height = height;

	SDL_SetWindowSize(video->window, video->width, video->height);

	if (video->texture != NULL) {
		SDL_DestroyTexture(video->texture);
	}

	video->texture = SDL_CreateTexture(video->renderer, SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING, video->width, video->height);
}

bool Initialize_VIDEO_SERVICE(VIDEO_SERVICE *video, KCONTEXT *ctx,
	MOUSE_SERVICE *mouse_service, KEYBOARD_SERVICE *keyboard_service) {
	
	video->ctx = ctx;
	video->main_loop = NULL;
	video->main_loop_running = false;
	video->mouse_service = mouse_service;
	video->keyboard_service = keyboard_service;
	video->mouse_count = 1;
	video->keyboard_count = 1;
	video->texture = NULL;

	memset(video->nal_buffer, 0, MAX_NAL_SIZE);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&video->decoder, 0, 0, 0, 0, (void*) video);
	video->byteStrmStart = broadwayCreateStream(&video->decoder, MAX_NAL_SIZE);

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Create an SDL window and renderer
	video->window = SDL_CreateWindow("YUV Rendering",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800,
		600, SDL_WINDOW_OPENGL);

	video->renderer = SDL_CreateRenderer(video->window, -1, 0);

	if (video->renderer == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
	}

	Report_Resize_Function_VIDEO_SERVICE(video);
	video->width = 800;
	video->height = 600;

	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,
		(void*) video, 0, Emscripten_HandleResize);
	
	return true;
}

void yuv_to_pixels(uint8_t *src, u32 src_width, u32 src_height,
	void *user_data) {

	VIDEO_SERVICE *video = (VIDEO_SERVICE*) user_data;
	
	if (video->decoder.decInfo.croppingFlag) {
		if (video->decoder.decInfo.cropParams.cropOutWidth != video->width ||
			video->decoder.decInfo.cropParams.cropOutHeight != video->height) {

			Actually_Resize_Window_VIDEO_SERVICE(video,
				video->decoder.decInfo.cropParams.cropOutWidth,
				video->decoder.decInfo.cropParams.cropOutHeight);
		}
	} else {
		if (video->decoder.decInfo.picWidth != video->width ||
			video->decoder.decInfo.picHeight != video->height) {

			Actually_Resize_Window_VIDEO_SERVICE(video,
				video->decoder.decInfo.picWidth,
				video->decoder.decInfo.picHeight);
		}
	}


	// Lock the texture to get a pointer to the texture pixels
	uint8_t *dst;
	int dst_width;
	int dst_height = video->height;
	SDL_LockTexture(video->texture, NULL, (void **)&dst, &dst_width);

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

	int iterator_height = MIN(video->height, src_height);
	int iterator_width = MIN(video->width, src_width);

	for (int i = 0; i < iterator_height; i++) {
		memcpy(dst_y, src_y, iterator_width);
		dst_y += dst_width;
		src_y += src_width;
	}

	for (int i = 0; i < iterator_height / 2; i++) {
		memcpy(dst_u, src_u, iterator_width / 2);
		dst_u += dst_width / 2;
		src_u += src_width / 2;
	}

	for (int i = 0; i < iterator_height / 2; i++) {
		memcpy(dst_v, src_v, iterator_width / 2);
		dst_v += dst_width / 2;
		src_v += src_width / 2;
	}

	// Unlock the texture
	SDL_UnlockTexture(video->texture);

	// Clear the renderer
	SDL_RenderClear(video->renderer);
	// Copy the texture to the renderer
	SDL_RenderCopy(video->renderer, video->texture, NULL, NULL);
	// Render the texture to the screen
	SDL_RenderPresent(video->renderer);
	// cout << "before" << endl;
}

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height,
	void *user_data) {
	
	yuv_to_pixels(buffer, width, height, user_data);
}

extern void broadwayOnHeadersDecoded() { printf("header decoded\n"); }

TIMER t;

void Decode_Buffer_VIDEO_SERVICE(VIDEO_SERVICE *video, char *buffer, int size) {
	video->decoder.streamStop = (u8 *)buffer + size;
	video->decoder.decInput.pStream = (u8 *)buffer;
	video->decoder.decInput.dataLen = size;

	
	//==============

	//============== < 5ms
	u32 i = 0;
	do {
		u8 *start = video->decoder.decInput.pStream;
		u32 ret = broadwayDecode(&video->decoder);
		//printf("Decoded Unit #%d, Size: %d, Result: %d\n", i++,
		//	   (video->decoder.decInput.pStream - start), ret);
	} while (video->decoder.decInput.dataLen > 0);
}

void Main_TCP_Loop_VIDEO_SERVICE(/*void *arg*/VIDEO_SERVICE *video) {
	//cout << "time: " << Stop_TIMER(&t) << endl;

	//============== < 1ms
	//VIDEO_SERVICE *video = (VIDEO_SERVICE *)arg;

	MOUSE_EVENT_T m_event;
	KEYBOARD_EVENT_T k_event;

	int temp_width;
	int temp_height;

	while (video->main_loop_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				m_event.x = event.button.x;
				m_event.y = event.button.y;
				m_event.clicked = false;
				m_event.state = MOUSE_ABS_COORD;
				m_event.event_index = video->mouse_count++;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
				//cout << "mouse motion event " << int(video->mouse_service->c->ws_client.client_id) << endl;
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_event.x = event.button.x;
				m_event.y = event.button.y;
				m_event.clicked = true;
				m_event.state = 1;	// pressed
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEBUTTONUP:
				m_event.x = event.button.x;
				m_event.y = event.button.y;
				m_event.clicked = true;
				m_event.state = 0;	// released
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEWHEEL:
				if (video->mouse_count++ % 8 != 0) break;
				m_event.x = event.wheel.x;
				m_event.y = event.wheel.y;
				m_event.clicked = true;
				m_event.state = 1;
				m_event.event_index = video->mouse_count++;
				m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;

				Send_CLIENT(video->mouse_service->c,
					(char*) &m_event,
					sizeof(MOUSE_EVENT_T));

				m_event.x = event.wheel.x;
				m_event.y = event.wheel.y;
				m_event.clicked = true;
				m_event.state = 0;
				m_event.event_index = video->mouse_count++;
				m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;

				Send_CLIENT(video->mouse_service->c,
					(char*) &m_event,
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
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					Report_Resize_Function_VIDEO_SERVICE(video);
				}
				break;
			}
		}
		//==============
		
		//============== < 10ms && < 70ms
		int size;
		if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(video->c, video->nal_buffer, size)) {
			//============== < 5ms
			Decode_Buffer_VIDEO_SERVICE(video, video->nal_buffer, size);
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
