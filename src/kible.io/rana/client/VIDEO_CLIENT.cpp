#include "VIDEO_CLIENT.h"
#include "komihash.h"

void Recv_Callback_VIDEO_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	/*
	if (buffer_size == 0) {
		return;
	}

	std::cout << "welp, in Recv_Callback_VIDEO_CLIENT" << std::endl;


	

	std::cout << "pushed " << buffer_size << std::endl;
	*/

	VIDEO_CLIENT *client = (VIDEO_CLIENT*) user_ptr;
	VIDEO_ELEMENT *element = new VIDEO_ELEMENT;

	element->bytes = new uint8_t[buffer_size];
	element->size = buffer_size;

	memcpy(element->bytes, buffer, buffer_size);

	//delete [] element->bytes;
	//delete element;

	client->pool->push(element);
}

static EM_BOOL Emscripten_HandleResize(int eventType, const EmscriptenUiEvent *uiEvent, void *userData) {
	VIDEO_CLIENT *video = (VIDEO_CLIENT*) userData;

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

void Actually_Resize_Window_VIDEO_CLIENT(VIDEO_CLIENT *video, int width, int height) {
	video->width = width;
	video->height = height;

	SDL_SetWindowSize(video->window, video->width, video->height);

	if (video->texture != NULL) {
		SDL_DestroyTexture(video->texture);
	}

	video->texture = SDL_CreateTexture(video->renderer, SDL_PIXELFORMAT_IYUV,
		SDL_TEXTUREACCESS_STREAMING, video->width, video->height);
}

bool VIDEO_CLIENT::Initialize(KCONTEXT *ctx, SERVICE_CLIENT_REGISTRY *registry) {
	ctx = ctx;
	main_loop = NULL;
	main_loop_running = false;
	mouse_count = 1;
	keyboard_count = 1;
	texture = NULL;
	ctrl_clicked = false;
	relative_mode = false;
	pool = new Queue<VIDEO_ELEMENT*>;

	//memset(nal_buffer, 0, MAX_NAL_SIZE);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&decoder, 0, 0, 0, 0, (void*) this);
	byteStrmStart = broadwayCreateStream(&decoder, 1920*1080*4);

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	// Create an SDL window and renderer
	window = SDL_CreateWindow("Portal",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		ctx->screen_dim.bw, ctx->screen_dim.h, SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
	}

	Actually_Resize_Window_VIDEO_CLIENT(this, ctx->screen_dim.bw,
		ctx->screen_dim.h);

	if (!Initialize_SOCKET_CLIENT(&socket_client,
		Recv_Callback_VIDEO_CLIENT, &registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW,
		(void*) this, 0, Emscripten_HandleResize);
	
	emscripten_set_main_loop_arg(
		[](void *arg) { Main_TCP_Loop_VIDEO_CLIENT(arg); }, this, 0, 0);
	
	return true;
}

void VIDEO_CLIENT::Delete() {}

void yuv_to_pixels(uint8_t *src, u32 src_width, u32 src_height,
	void *user_data) {

	VIDEO_CLIENT *video = (VIDEO_CLIENT*) user_data;
	
	/*
	if (video->decoder.decInfo.croppingFlag) {
		if (video->decoder.decInfo.cropParams.cropOutWidth != video->width ||
			video->decoder.decInfo.cropParams.cropOutHeight != video->height) {

			Actually_Resize_Window_VIDEO_CLIENT(video,
				video->decoder.decInfo.cropParams.cropOutWidth,
				video->decoder.decInfo.cropParams.cropOutHeight);
		}
	} else {
		if (video->decoder.decInfo.picWidth != video->width ||
			video->decoder.decInfo.picHeight != video->height) {

			Actually_Resize_Window_VIDEO_CLIENT(video,
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
	*/
}

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height,
	void *user_data) {
	
	std::cout << "in broadwayOnPictureDecoded new" << std::endl;

	//yuv_to_pixels(buffer, width, height, user_data);
}

extern void broadwayOnHeadersDecoded() { printf("header decoded\n"); }

TIMER t;

void Decode_Buffer_VIDEO_CLIENT(VIDEO_CLIENT *video, char *buffer, int size) {

	std::cout << "in Decode_Buffer_VIDEO_CLIENT" << std::endl;

	video->decoder.streamStop = (u8 *)buffer + size;
	video->decoder.decInput.pStream = (u8 *)buffer;
	video->decoder.decInput.dataLen = size;
	
	//==============

	//============== < 5ms
	u32 i = 0;
	do {
		u8 *start = video->decoder.decInput.pStream;
		std::cout << "starting Decode_Buffer_VIDEO_CLIENT" << std::endl;
		u32 ret = broadwayDecode(&video->decoder);
		std::cout << "ending Decode_Buffer_VIDEO_CLIENT" << std::endl;
		//printf("Decoded Unit #%d, Size: %d, Result: %d\n", i++,
		//	   (video->decoder.decInput.pStream - start), ret);
	} while (video->decoder.decInput.dataLen > 0);

	std::cout << "super ending" << std::endl;
}

void Main_TCP_Loop_VIDEO_CLIENT(void *arg/*VIDEO_CLIENT *video*/) {
	//cout << "time: " << Stop_TIMER(&t) << endl;

	//============== < 1ms
	VIDEO_CLIENT *video = (VIDEO_CLIENT *)arg;

	MOUSE_EVENT_T m_event;
	KEYBOARD_EVENT_T k_event;
	VIDEO_ELEMENT *element;

	int temp_width;
	int temp_height;
	//TIMER t;

	//std::cout << "starting loop" << std::endl;

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
				//Send_CLIENT(video->mouse_service->c, (char *)&m_event,
				//			sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_event.x = event.button.x;
				m_event.y = event.button.y;
				m_event.clicked = true;
				m_event.state = 1;	// pressed
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				//Send_CLIENT(video->mouse_service->c, (char *)&m_event,
				//			sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEBUTTONUP:
				m_event.x = event.button.x;
				m_event.y = event.button.y;
				m_event.clicked = true;
				m_event.state = 0;	// released
				m_event.event_index = video->mouse_count++;
				m_event.button = event.button.button;
				//Send_CLIENT(video->mouse_service->c, (char *)&m_event,
				//			sizeof(MOUSE_EVENT_T));
				break;
			case SDL_MOUSEWHEEL:
				if (video->mouse_count++ % 8 != 0) break;
				m_event.x = event.wheel.x;
				m_event.y = event.wheel.y;
				m_event.clicked = true;
				m_event.state = 1;
				m_event.event_index = video->mouse_count++;
				m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;

				//Send_CLIENT(video->mouse_service->c,
				//	(char*) &m_event,
				//	sizeof(MOUSE_EVENT_T));

				m_event.x = event.wheel.x;
				m_event.y = event.wheel.y;
				m_event.clicked = true;
				m_event.state = 0;
				m_event.event_index = video->mouse_count++;
				m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;

				//Send_CLIENT(video->mouse_service->c,
				//	(char*) &m_event,
				//	sizeof(MOUSE_EVENT_T));
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_RCTRL ||
					event.key.keysym.sym == SDLK_LCTRL) {
					video->ctrl_clicked = true;
				}

				k_event.code = event.key.keysym.sym;
				k_event.value = 1;
				k_event.event_index = video->keyboard_count++;
				//Send_CLIENT(video->keyboard_service->c,
				//	(char*) &k_event,
				//	sizeof(KEYBOARD_EVENT_T));
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
							//Density_THEMIS_CLIENT(video->ctx->themis_api,
							//	kible::themis::PixelDensity::PIXELDENSITY_HIGH);
							break;
						case SDLK_3:
							//Density_THEMIS_CLIENT(video->ctx->themis_api,
							//	kible::themis::PixelDensity::PIXELDENSITY_MEDIUM);
							break;
						case SDLK_4:
							//Density_THEMIS_CLIENT(video->ctx->themis_api,
							//	kible::themis::PixelDensity::PIXELDENSITY_LOW);
							break;
						case SDLK_5:
							//Density_THEMIS_CLIENT(video->ctx->themis_api,
							//	kible::themis::PixelDensity::PIXELDENSITY_PLACEBO);
							break;
					}
				}

				k_event.code = event.key.keysym.sym;
				k_event.value = 0;
				k_event.event_index = video->keyboard_count++;
				//Send_CLIENT(video->keyboard_service->c,
				//	(char*) &k_event,
				//	sizeof(KEYBOARD_EVENT_T));
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					//Report_Resize_Function_VIDEO_CLIENT(video);
				}
				break;
			}
		}

		if (video->pool->size() > 0) {
			video->pool->pop(element);

			//uint64_t HashVal = komihash( (char*) element->bytes, element->size, 0);

			//std::cout << "poped " << element->size << " " << video->pool->size() << " " << HashVal << std::endl;
			//Decode_Buffer_VIDEO_CLIENT(video, (char*) element->bytes, element->size);


			//memcpy(video->nal_buffer, element->bytes, element->size);

			Decode_Buffer_VIDEO_CLIENT(video, (char*) element->bytes, element->size);

			//std::cout << "ok?!?!" << std::endl;

			delete [] element->bytes;
			delete element;
		}

		/*
		if (video->pool->size() > 0) {
			video->pool->pop(element);

			uint64_t HashVal = komihash( (char*) element->bytes, element->size, 0);

			//std::cout << "poped " << element->size << " " << video->pool->size() << " " << HashVal << std::endl;
			//Decode_Buffer_VIDEO_CLIENT(video, (char*) element->bytes, element->size);


			//memcpy(video->nal_buffer, element->bytes, element->size);

			//Decode_Buffer_VIDEO_CLIENT(video, (char*) element->bytes, element->size);

			//std::cout << "ok?!?!" << std::endl;

			delete [] element->bytes;
			delete element;
		}

		//std::cout << "loop has come to an end" << std::endl;
		*/

		/*
		int size;
		if (Receive_CLIENT(video->c, (char *)&size, sizeof(int)) &&
			Receive_CLIENT(video->c, video->nal_buffer, size)) {
			//============== < 5ms
			Decode_Buffer_VIDEO_CLIENT(video, video->nal_buffer, size);
			//==============
		}
		*/
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
			Decode_Buffer_VIDEO_CLIENT(video, video->nal_buffer, size);
			cout << "render time: " << Stop_TIMER(&t) << " " << getTime() << endl;
			//==============
		}
		*/
		
		//==============
	//}

	//Start_TIMER(&t);
}

bool Resize_VIDEO_CLIENT(VIDEO_CLIENT *video, int width, int height) {
	return true;
}

bool Status_VIDEO_CLIENT(VIDEO_CLIENT *video) {
	return video->main_loop_running;
}

void Disconnect_VIDEO_CLIENT(VIDEO_CLIENT *video) {
	video->main_loop_running = false;
	if (video->main_loop != NULL) {
		video->main_loop->join();
		delete video->main_loop;
		video->main_loop = NULL;
	}
}
