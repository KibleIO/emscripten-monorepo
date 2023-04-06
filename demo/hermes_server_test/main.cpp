#include <Utilities/KCONTEXT.h>
#include <Hermes/HERMES_SERVER.h>
#include <iostream>

using namespace std;

int main() {
	KCONTEXT ctx;
	HERMES_SERVER hs;
	HERMES_TYPE themis_types[] = {HERMES_SERVER_WS, HERMES_AUDIO_WS,
		HERMES_VIDEO_WS, HERMES_DEVICE_WS, HERMES_NULL};

	if (!Initialize_HERMES_SERVER(&hs, &ctx, 4512)) {
		cout << "couldn't initialize hermes server" << endl;
		return 0;
	}

	if (!Connect_HERMES_SERVER(&hs, themis_types)) {
		cout << "couldn't connect hermes server" << endl;
		return 0;
	}

	cout << "success" << endl;
	return 0;
}