#include "utils.h"

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
}

void log_dbg(char *hello) {
	cout << hello << endl;
}

void log_err(char *hello) {
	cout << hello << endl;
}