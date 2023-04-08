#include <SDL.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "Broadway-H.264-decoder/Decoder/src/Decoder.h"
#include "Rana_Core_Utils/Hermes/CLIENT.h"
#include "Rana_Core_Utils/Hermes/HERMES_CLIENT.h"

// using namespace std;

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
int width = 1920;
int height = 816;

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

Decoder decoder;
u8 *byteStrmStart;
u32 strmLen;

int stop = 0;
int counter = 0;

void render_frame() {
	// Clear the renderer
	SDL_RenderClear(renderer);
	// Copy the texture to the renderer
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	// Render the texture to the screen
	SDL_RenderPresent(renderer);

	if (!stop) {
		// playStream(&dec, &dec.broadwayStream);
		u8 *start = decoder.decInput.pStream;
		u32 ret = broadwayDecode(&decoder);
		printf("Decoded Unit #%d, Size: %d, Result: %d\n", counter++,
			   (decoder.decInput.pStream - start), ret);
	}

	if (decoder.decInput.dataLen <= 0) {
		stop = 1;
	}
}

int main() {
	cout << "step 0" << endl;
	KCONTEXT ctx;
	HERMES_CLIENT hc;
	HERMES_TYPE themis_types[] = {HERMES_CLIENT_WS, HERMES_AUDIO_WS,
								  HERMES_VIDEO_WS, HERMES_DEVICE_WS,
								  HERMES_NULL};

	FILE *finput;

	DEBUG(("H.264 Decoder API v%d.%d\n", broadwayGetMajorVersion(),
		   broadwayGetMinorVersion()));

	/* open input file for reading, file name given by user. If file open
	 * fails -> exit */
	finput = fopen("test2.h264", "rb");
	if (finput == NULL) {
		DEBUG(("UNABLE TO OPEN INPUT FILE\n"));
		return -1;
	}

	/* check size of the input file -> length of the stream in bytes */
	fseek(finput, 0L, SEEK_END);
	strmLen = (u32)ftell(finput);
	rewind(finput);

	broadwayInit(&decoder, 1, 0, 0, 0);
	byteStrmStart = broadwayCreateStream(&decoder, strmLen);

	/* read input stream from file to buffer and close input file */
	fread(byteStrmStart, sizeof(u8), strmLen, finput);
	fclose(finput);

	decoder.streamStop =
		decoder.broadwayStream.buffer + decoder.broadwayStream.length;
	decoder.decInput.pStream = decoder.broadwayStream.buffer;
	decoder.decInput.dataLen = decoder.broadwayStream.length;
	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Create an SDL window and renderer
	window = SDL_CreateWindow("YUV Rendering", SDL_WINDOWPOS_UNDEFINED,
							  SDL_WINDOWPOS_UNDEFINED, width, height,
							  SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (renderer == NULL) {
		printf("Failed to create renderer: %s\n", SDL_GetError());
	}

	// Create an SDL texture
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV,
								SDL_TEXTUREACCESS_STREAMING, width, height);
	// Set the render loop
	emscripten_set_main_loop(render_frame, 30, 1);

	broadwayExit(&decoder);

	if (!Initialize_HERMES_CLIENT(&hc, &ctx, "127.0.0.1", 4512)) {
		cout << "couldn't initialize hermes server" << endl;
		return 0;
	}

	if (!Connect_HERMES_CLIENT(&hc, themis_types)) {
		cout << "couldn't connect hermes server" << endl;
		return 0;
	}

	cout << "success" << endl;

	CLIENT *client = Get_HERMES_CLIENT(&hc, HERMES_DEVICE_WS);
	if (client != NULL) {
		char hello[100];
		int size = 0;

		strcpy(hello, "hello from client");
		size = strlen(hello);

		if (!Send_CLIENT(client, (char *)&size, sizeof(size))) {
			cout << "error sending" << endl;
		}

		if (!Send_CLIENT(client, hello, size)) {
			cout << "error sending" << endl;
		}

		if (!Receive_CLIENT(client, (char *)&size, sizeof(size))) {
			cout << "error receving" << endl;
		}

		if (!Receive_CLIENT(client, hello, size)) {
			cout << "error receving" << endl;
		}

		cout << "test complete " << hello << " " << size << endl;
	}

	return 0;
}
