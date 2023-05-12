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

void get_cookie(char *str_out) {
	int characters_to_copy;

	char *str = (char*)EM_ASM_PTR({
		const value = `; ${document.cookie}`;
		const parts = value.split(`; ${name}=`);
		var jsString = 'nil';
		if (parts.length === 2) jsString = parts.pop().split(';').shift();

		var lengthBytes = lengthBytesUTF8(jsString)+1;

		return stringToNewUTF8(jsString);
	});
	printf("UTF8 string says: %s\n", str);

	characters_to_copy = strlen(str);
	if (characters_to_copy > MAX_COOKIE_SIZE) {
		characters_to_copy = MAX_COOKIE_SIZE;
	}

	strncpy(str_out, str, characters_to_copy);
  	str_out[MAX_HTTP_RESPONSE_SIZE - 1] = '\0';

	free(str);
}