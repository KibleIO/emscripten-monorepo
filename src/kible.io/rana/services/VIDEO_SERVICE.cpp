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
	video->ctrl_clicked = false;
	video->relative_mode = false;
	video->rendering = false;

	video->front_buffer = &video->buffer1;
	video->back_buffer = &video->buffer2;

	video->buffer1.width = 800;
	video->buffer1.height = 600;
	video->buffer1.buffer = new char[video->buffer1.width * video->buffer1.height * 4];

	video->buffer2.width = 800;
	video->buffer2.height = 600;
	video->buffer2.buffer = new char[video->buffer2.width * video->buffer2.height * 4];

	memset(video->nal_buffer, 0, MAX_NAL_SIZE);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&video->decoder, 0, 0, 0, 0, (void*) video);
	video->byteStrmStart = broadwayCreateStream(&video->decoder, MAX_NAL_SIZE);

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Create an SDL window and renderer
	video->window = SDL_CreateWindow("Portal",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800,
		600, SDL_WINDOW_OPENGL);

	video->renderer = SDL_CreateRenderer(video->window, -1, 0);

	if (video->renderer == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
	}

	Report_Resize_Function_VIDEO_SERVICE(video);
	Actually_Resize_Window_VIDEO_SERVICE(video,
				800,
				600);

	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,
		(void*) video, 0, Emscripten_HandleResize);
	
	return true;
}

void yuv_to_pixels(uint8_t *src, u32 src_width, u32 src_height,
	void *user_data) {

	VIDEO_SERVICE *video = (VIDEO_SERVICE*) user_data;
	FRAMEBUFFER *swapper;
	
	if (video->decoder.decInfo.croppingFlag) {
		if (video->back_buffer->width != video->decoder.decInfo.cropParams.cropOutWidth ||
			video->back_buffer->height != video->decoder.decInfo.cropParams.cropOutHeight) {
			
			video->back_buffer->width = video->decoder.decInfo.cropParams.cropOutWidth;
			video->back_buffer->height = video->decoder.decInfo.cropParams.cropOutHeight;
			int size = (video->back_buffer->width * video->back_buffer->height) + ((video->back_buffer->width * video->back_buffer->height) / 2);

			delete [] video->back_buffer->buffer;
			video->back_buffer->buffer = new char[size];
		}
		
	} else {
		if (video->back_buffer->width != video->decoder.decInfo.picWidth ||
			video->back_buffer->height != video->decoder.decInfo.picHeight) {

			video->back_buffer->width = video->decoder.decInfo.picWidth;
			video->back_buffer->height = video->decoder.decInfo.picHeight;
			int size = (video->back_buffer->width * video->back_buffer->height) + ((video->back_buffer->width * video->back_buffer->height) / 2);

			delete [] video->back_buffer->buffer;
			video->back_buffer->buffer = new char[size];
		}	
	}
	
	// Lock the texture to get a pointer to the texture pixels
	uint8_t *dst = (uint8_t*) video->back_buffer->buffer;
	int dst_width = video->back_buffer->width;
	int dst_height = video->back_buffer->height;

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

	int iterator_height = MIN(video->back_buffer->height, src_height);
	int iterator_width = MIN(video->back_buffer->width, src_width);
	
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

	if (!video->rendering) {
		swapper = video->back_buffer;
		video->back_buffer = video->front_buffer;
		video->front_buffer = swapper;
	}
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

void Main_TCP_Loop_VIDEO_SERVICE(void *arg/*VIDEO_SERVICE *video*/) {
	//cout << "time: " << Stop_TIMER(&t) << endl;

	//============== < 1ms
	VIDEO_SERVICE *video = (VIDEO_SERVICE *)arg;

	MOUSE_EVENT_T m_event;
	KEYBOARD_EVENT_T k_event;

	int temp_width;
	int temp_height;
	//TIMER t;

	//while (video->main_loop_running) {
		//cout << "starting main loop: " << getTime() << endl;
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEMOTION:
				if (video->relative_mode) {
					m_event.state = MOUSE_REL_COORD;
					m_event.x = event.motion.xrel;
					m_event.y = event.motion.yrel;
				} else {
					m_event.state = MOUSE_ABS_COORD;
					m_event.x = event.motion.x;
					m_event.y = event.motion.y;
				}
				m_event.clicked = false;
				m_event.event_index = video->mouse_count++;
				Send_CLIENT(video->mouse_service->c, (char *)&m_event,
							sizeof(MOUSE_EVENT_T));
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
				if (event.key.keysym.sym == SDLK_RCTRL ||
					event.key.keysym.sym == SDLK_LCTRL) {
					video->ctrl_clicked = true;
				}

				k_event.code = event.key.keysym.sym;
				k_event.value = 1;
				k_event.event_index = video->keyboard_count++;
				Send_CLIENT(video->keyboard_service->c,
					(char*) &k_event,
					sizeof(KEYBOARD_EVENT_T));
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_RCTRL ||
					event.key.keysym.sym == SDLK_LCTRL) {
					video->ctrl_clicked = false;
				} else if (video->ctrl_clicked) {
					switch (event.key.keysym.sym) {
						case SDLK_1:
							video->relative_mode =
								!video->relative_mode;
							SDL_SetRelativeMouseMode(
								(SDL_bool)video->relative_mode);
							break;
						case SDLK_2:
							Density_THEMIS_CLIENT(video->ctx->themis_api,
								kible::themis::PixelDensity::PIXELDENSITY_HIGH);
							break;
						case SDLK_3:
							Density_THEMIS_CLIENT(video->ctx->themis_api,
								kible::themis::PixelDensity::PIXELDENSITY_MEDIUM);
							break;
						case SDLK_4:
							Density_THEMIS_CLIENT(video->ctx->themis_api,
								kible::themis::PixelDensity::PIXELDENSITY_LOW);
							break;
						case SDLK_5:
							Density_THEMIS_CLIENT(video->ctx->themis_api,
								kible::themis::PixelDensity::PIXELDENSITY_PLACEBO);
							break;
					}
				}

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

		
		video->rendering = true;

		if (video->front_buffer->width != video->width ||
			video->front_buffer->height != video->height) {

			Actually_Resize_Window_VIDEO_SERVICE(video,
				video->front_buffer->width,
				video->front_buffer->height);
		}

		uint8_t *dst;
		int dst_width;
		int dst_height = video->front_buffer->height;
		SDL_LockTexture(video->texture, NULL, (void **)&dst, &dst_width);

		unsigned char *src_ptr = (unsigned char *) video->front_buffer->buffer;
		unsigned char *dst_ptr = dst;
		
		int size = (video->front_buffer->width * video->front_buffer->height) + ((video->front_buffer->width * video->front_buffer->height) / 2);

		memcpy(dst_ptr, src_ptr, size);
		
		// Unlock the texture
		SDL_UnlockTexture(video->texture);

		// Clear the renderer
		SDL_RenderClear(video->renderer);
		// Copy the texture to the renderer
		SDL_RenderCopy(video->renderer, video->texture, NULL, NULL);
		// Render the texture to the screen
		SDL_RenderPresent(video->renderer);

		video->rendering = false;
		//==============
		
		//============== < 10ms && < 70ms
		/*
		int size;
		Start_TIMER(&t);
		if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(video->c, video->nal_buffer, size)) {
			cout << "outer time: " << Stop_TIMER(&t) << " " << getTime() << endl;
			Start_TIMER(&t);
			//============== < 5ms
			Decode_Buffer_VIDEO_SERVICE(video, video->nal_buffer, size);
			cout << "render time: " << Stop_TIMER(&t) << " " << getTime() << endl;
			//==============
		}
		*/
		
		//==============
	//}

	//Start_TIMER(&t);
}

void render_thread(VIDEO_SERVICE *video) {
	//TIMER ttt;
	int size;
	while (true) {
		if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(video->c, video->nal_buffer, size)) {
			//cout << "outer time123: " << Stop_TIMER(&ttt) << " " << getTime() << endl;
			//============== < 5ms
			//Start_TIMER(&ttt);
			Decode_Buffer_VIDEO_SERVICE(video, video->nal_buffer, size);
			//cout << "render time: " << Stop_TIMER(&ttt) << " " << getTime() << " " << video->c->ws_client.ws_master->active_read[3]->size() << endl;
			//==============
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

	ASSERT_E_R(Receive_CLIENT(video_init, (char *)&size, sizeof(int)),
			   "Failed to receive headers", video->ctx);
	ASSERT_E_R(Receive_CLIENT(video_init, video->nal_buffer, size),
			   "Failed to receive headers", video->ctx);
	video->main_loop_running = true;

	//Main_TCP_Loop_VIDEO_SERVICE(video);
	video->main_loop = new thread(render_thread, video);
	emscripten_set_main_loop_arg(
		[](void *arg) { Main_TCP_Loop_VIDEO_SERVICE(arg); }, video, 0, 1);
	 //
	 //while (true) {
	//	Sleep_Milli(1000);
	//}

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
