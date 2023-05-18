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

void Get_Cookie(char *str_out) {
	int characters_to_copy;

	char *str = (char*)EM_ASM_PTR({
		const value = `; ${document.cookie}`;
		const parts = value.split(`; alien_session=`);
		var jsString = 'nil';
		if (parts.length === 2) jsString = parts.pop().split(';').shift();
		return stringToNewUTF8(jsString);
	});

	characters_to_copy = strlen(str);
	if (characters_to_copy > MAX_COOKIE_SIZE) {
		characters_to_copy = MAX_COOKIE_SIZE;
	}

	strncpy(str_out, str, characters_to_copy);
  	str_out[characters_to_copy] = '\0';

	free(str);
}

void Get_Url(char *str_out) {
	int characters_to_copy;

	char *str = (char*)EM_ASM_PTR({
		return stringToNewUTF8(window.location.hostname);
	});

	characters_to_copy = strlen(str);
	if (characters_to_copy > MAX_COOKIE_SIZE) {
		characters_to_copy = MAX_COOKIE_SIZE;
	}

	strncpy(str_out, str, characters_to_copy);
  	str_out[characters_to_copy] = '\0';

	free(str);
}

int64_t getTime() {
	timeval start;
	gettimeofday(&start, NULL);
	return ((start.tv_sec) * 1000 + start.tv_usec / 1000.0) + 0.5;
}