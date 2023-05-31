#include "VIDEO_CLIENT.h"

void Decode_Buffer_VIDEO_CLIENT(VIDEO_CLIENT *video, char *buffer, int size) {
	video->decoder.streamStop = (u8 *)buffer + size;
	video->decoder.decInput.pStream = (u8 *)buffer;
	video->decoder.decInput.dataLen = size;
	
	u32 i = 0;
	do {
		u8 *start = video->decoder.decInput.pStream;
		u32 ret = broadwayDecode(&video->decoder);
		//printf("Decoded Unit #%d, Size: %d, Result: %d\n", i++,
		//	   (video->decoder.decInput.pStream - start), ret);
	} while (video->decoder.decInput.dataLen > 0);
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

void Recv_Callback_VIDEO_CLIENT(void *user_ptr, char *buffer, int buffer_size) {
	VIDEO_CLIENT *client = (VIDEO_CLIENT*) user_ptr;

	if (client->pool->size() > MAX_ACCUMULATED_FRAMES) {
		std::cout << "dropping frame" << std::endl;
		return;
	}

	VIDEO_ELEMENT *element = new VIDEO_ELEMENT;

	element->size = buffer_size;
	element->bytes = new uint8_t[element->size];

	memcpy(element->bytes, buffer, buffer_size);

	client->pool->push(element);
}

bool VIDEO_CLIENT::Initialize(KCONTEXT *ctx_in, SERVICE_CLIENT_REGISTRY *registry) {
	ctx = ctx_in;
	//main_loop = NULL;
	main_loop_running = false;
	mouse_count = 1;
	keyboard_count = 1;
	texture = NULL;
	ctrl_clicked = false;
	relative_mode = false;
	fullscreen = false;
	mouse = Get_Instance_Of_SERVICE_CLIENT_REGISTRY<MOUSE_CLIENT*>(registry);
	keyboard = Get_Instance_Of_SERVICE_CLIENT_REGISTRY<KEYBOARD_CLIENT*>(registry);

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	broadwayInit(&decoder, 0, 0, 0, 0, (void*) this);

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
		Recv_Callback_VIDEO_CLIENT, registry->socket_client_registry,
		ctx, this)) {
		
		return false;
	}

	emscripten_set_main_loop_arg(
		[](void *arg) { Main_TCP_Loop_VIDEO_CLIENT(arg); }, this, 0, 0);
	
	return true;
}

void VIDEO_CLIENT::Delete() {}

void yuv_to_pixels(uint8_t *src, u32 src_width, u32 src_height,
	void *user_data) {

	VIDEO_CLIENT *video = (VIDEO_CLIENT*) user_data;

	if (!video->main_loop_running) return;
	
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
}

extern void broadwayOnPictureDecoded(u8 *buffer, u32 width, u32 height,
	void *user_data) {

	yuv_to_pixels(buffer, width, height, user_data);
}

extern void broadwayOnHeadersDecoded() { printf("header decoded\n"); }

void Main_TCP_Loop_VIDEO_CLIENT(void *arg) {
	VIDEO_CLIENT *video = (VIDEO_CLIENT *)arg;

	MOUSE_EVENT_T m_event;
	KEYBOARD_EVENT_T k_event;
	VIDEO_ELEMENT *element;

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
			Send_Event_MOUSE_CLIENT(video->mouse, &m_event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_event.x = event.button.x;
			m_event.y = event.button.y;
			m_event.clicked = true;
			m_event.state = 1;	// pressed
			m_event.event_index = video->mouse_count++;
			m_event.button = event.button.button;
			Send_Event_MOUSE_CLIENT(video->mouse, &m_event);
			break;
		case SDL_MOUSEBUTTONUP:
			m_event.x = event.button.x;
			m_event.y = event.button.y;
			m_event.clicked = true;
			m_event.state = 0;	// released
			m_event.event_index = video->mouse_count++;
			m_event.button = event.button.button;
			Send_Event_MOUSE_CLIENT(video->mouse, &m_event);
			break;
		case SDL_MOUSEWHEEL:
			if (video->mouse_count++ % 8 != 0) break;
			m_event.x = event.wheel.x;
			m_event.y = event.wheel.y;
			m_event.clicked = true;
			m_event.state = 1;
			m_event.event_index = video->mouse_count++;
			m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;
			Send_Event_MOUSE_CLIENT(video->mouse, &m_event);

			m_event.x = event.wheel.x;
			m_event.y = event.wheel.y;
			m_event.clicked = true;
			m_event.state = 0;
			m_event.event_index = video->mouse_count++;
			m_event.button = (event.wheel.preciseY > 0) ? 4 : 5;
			Send_Event_MOUSE_CLIENT(video->mouse, &m_event);
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_RCTRL ||
				event.key.keysym.sym == SDLK_LCTRL) {
				video->ctrl_clicked = true;
			}

			k_event.code = event.key.keysym.sym;
			k_event.value = 1;
			k_event.event_index = video->keyboard_count++;
			Send_Event_KEYBOARD_CLIENT(video->keyboard, &k_event);
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
						Density_THEMIS_CLIENT(video->ctx,
							kible::themis::PixelDensity::PIXELDENSITY_HIGH);
						break;
					case SDLK_3:
						Density_THEMIS_CLIENT(video->ctx,
							kible::themis::PixelDensity::PIXELDENSITY_MEDIUM);
						break;
					case SDLK_4:
						Density_THEMIS_CLIENT(video->ctx,
							kible::themis::PixelDensity::PIXELDENSITY_LOW);
						break;
					case SDLK_5:
						Density_THEMIS_CLIENT(video->ctx,
							kible::themis::PixelDensity::PIXELDENSITY_PLACEBO);
						break;
					case SDLK_6:
						video->fullscreen =
							!video->fullscreen;

						if (video->fullscreen) {
							EmscriptenFullscreenStrategy s;
							memset(&s, 0, sizeof(s));
							s.scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_DEFAULT;
							s.canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF;
							s.filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT;
							emscripten_request_fullscreen_strategy("#canvas", 1, &s);
						} else {
							emscripten_exit_fullscreen();
						}
						break;
					case SDLK_7:
						FPS_THEMIS_CLIENT(video->ctx, 30);
						break;
					case SDLK_8:
						FPS_THEMIS_CLIENT(video->ctx, 60);
						break;
				}
			}

			k_event.code = event.key.keysym.sym;
			k_event.value = 0;
			k_event.event_index = video->keyboard_count++;
			Send_Event_KEYBOARD_CLIENT(video->keyboard, &k_event);
			break;
		}
	}

	
	while (video->pool->size() > 0) {
		video->pool->pop(element);

		video->main_loop_running = (video->pool->size() == 0);

		Decode_Buffer_VIDEO_CLIENT(video, (char*) element->bytes, element->size);

		delete [] element->bytes;
		delete element;
	}
}

/*

why does this have to be defined here? https://stackoverflow.com/questions/56392506/compiler-optimization-removes-implicit-template-instantiation-leading-to-linker

*/
template<typename T>
T Get_Instance_Of_SERVICE_CLIENT_REGISTRY(SERVICE_CLIENT_REGISTRY *registry) {
	T return_type = NULL;
	for (int i = 0; i < registry->service_count; i++) {
		return_type = dynamic_cast<const T>(registry->service_client[i]);
		if (return_type != NULL) {
			return return_type;
		}
	}
	return return_type;
}