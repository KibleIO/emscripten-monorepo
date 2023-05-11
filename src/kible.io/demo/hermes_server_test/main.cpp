#include <Utilities/KCONTEXT.h>
#include <Hermes/HERMES_SERVER.h>
#include <Hermes/SERVER.h>
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

	SERVER *server = Get_HERMES_SERVER(&hs, HERMES_DEVICE_WS);
	if (server != NULL) {
		char hello[100];
		int size = 0;

		if (!Receive_SERVER(server, (char*) &size, sizeof(size))) {
			cout << "error receving" << endl;
		}

		if (!Receive_SERVER(server, hello, size)) {
			cout << "error receving" << endl;
		}

		cout << hello << " " << size << endl;

		strcpy(hello, "hello from server");
		size = strlen(hello);

		if (!Send_SERVER(server, (char*) &size, sizeof(size))) {
			cout << "error sending" << endl;
		}

		if (!Send_SERVER(server, hello, size)) {
			cout << "error sending" << endl;
		}

		cout << "test complete " << endl;
	}

	return 0;
}