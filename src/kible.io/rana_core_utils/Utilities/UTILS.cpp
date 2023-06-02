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

	if (*width < MINIMUM_WIDTH) {
		*width = MINIMUM_WIDTH;
	}

	if (*height < MINIMUM_HEIGHT) {
		*height = MINIMUM_HEIGHT;
	}

	if (*width > MAXIMUM_WIDTH) {
		*width = MAXIMUM_WIDTH;
	}

	if (*height > MAXIMUM_HEIGHT) {
		*height = MAXIMUM_HEIGHT;
	}

	if (*width % 32 != 0) {
		*width = (*width / 32) * 32;
	}

	if (*height % 32 != 0) {
		*height = (*height / 32) * 32;
	}
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

void generate_uuid(char *str_out) {
	int characters_to_copy;

	char *str = (char*)EM_ASM_PTR({
		return stringToNewUTF8(crypto.randomUUID());
	});

	characters_to_copy = strlen(str);
	if (characters_to_copy > UUID_STR_SIZE) {
		characters_to_copy = UUID_STR_SIZE;
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

void get_current_time(char *str) {
	timeval tv;
	time_t nowtime;
	tm *nowtm;
	char tmbuf[64], buf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = gmtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%dT%H:%M:%S", nowtm);
	snprintf(str, 64, "%s.%09ldZ", tmbuf, tv.tv_usec);
}

unsigned int getTotalMemory()
{
	return EM_ASM_INT(return HEAP8.length);
}

unsigned int getFreeMemory()
{
	s_mallinfo i = mallinfo();
	unsigned int totalMemory = getTotalMemory();
	unsigned int dynamicTop = (unsigned int)sbrk(0);
	return totalMemory - dynamicTop + i.fordblks;
}