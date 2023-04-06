#include "Rana_Core_Utils/Hermes/HERMES_CLIENT.h"
#include <iostream>

using namespace std;

int main() {
	cout << "step 0" << endl;
	KCONTEXT ctx;
	HERMES_CLIENT hc;
	HERMES_TYPE themis_types[] = {HERMES_CLIENT_WS, HERMES_AUDIO_WS,
		HERMES_VIDEO_WS, HERMES_DEVICE_WS, HERMES_NULL};

	if (!Initialize_HERMES_CLIENT(&hc, &ctx, "127.0.0.1", 4512)) {
		cout << "couldn't initialize hermes server" << endl;
		return 0;
	}

	cout << "initialized client" << endl;

	if (!Connect_HERMES_CLIENT(&hc, themis_types)) {
		cout << "couldn't connect hermes server" << endl;
		return 0;
	}

	cout << "success" << endl;
}
