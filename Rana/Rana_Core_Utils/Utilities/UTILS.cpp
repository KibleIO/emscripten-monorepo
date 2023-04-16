#include "UTILS.h"

void Sleep_Milli(unsigned int milli) {
#ifdef __linux__
    usleep(milli * 1000);
#endif
#ifdef __APPLE__
usleep(milli * 1000);
#endif
#ifdef _WIN64
    Sleep(milli);
#endif
#ifdef __EMSCRIPTEN__
emscripten_sleep(milli);
#endif
}

void log_dbg(char *hello) {
	cout << hello << endl;
}

void log_err(char *hello) {
	cout << hello << endl;
}

void get_screen_width_height(int *width, int *height) {
	double canvasWidth;
	double canvasHeight;
	EMSCRIPTEN_RESULT res = emscripten_get_element_css_size("#canvas",
		&canvasWidth, &canvasHeight);
	
	*width = canvasWidth;
	*height = canvasHeight;
}

