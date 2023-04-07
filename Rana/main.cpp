#include "Rana_Core_Utils/Hermes/HERMES_CLIENT.h"
#include "Rana_Core_Utils/Hermes/CLIENT.h"
#include <iostream>

#define THEMIS_PORT 4460

using namespace std;

int main() {
	cout << "step 0" << endl;
	KCONTEXT ctx;
	HERMES_CLIENT hc;
	HERMES_TYPE themis_types[] = {HERMES_CLIENT_WS, HERMES_THEMIS_WS,
		HERMES_VIDEO_INIT_WS, HERMES_VIDEO_WS, HERMES_MOUSE_WS, HERMES_KEYBOARD_WS,
		HERMES_AUDIO_WS, HERMES_CLIPBOARD_WS, HERMES_NULL};

	if (!Initialize_HERMES_CLIENT(&hc, &ctx, "127.0.0.1", THEMIS_PORT)) {
		cout << "couldn't initialize hermes server" << endl;
		return 0;
	}

	if (!Connect_HERMES_CLIENT(&hc, themis_types)) {
		cout << "couldn't connect hermes server" << endl;
		return 0;
	}

	cout << "success" << endl;

	Sleep_Milli(10000);

	cout << "success2" << endl;

	return 0;
}
