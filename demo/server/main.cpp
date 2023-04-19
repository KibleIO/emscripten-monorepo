#include <iostream>
#include "utils.h"
#include "WS_SERVER_MASTER.h"
#include "WS_SERVER.h"

using namespace std;

int64_t getTime() {
	timeval start;
	gettimeofday(&start, NULL);
	return ((start.tv_sec) * 1000 + start.tv_usec / 1000.0) + 0.5;
}

int main() {
	WS_SERVER_MASTER master_server;
	if (!Initialize_WS_SERVER_MASTER(&master_server, 4000)) {
		cout << "error1" << endl;
		return 0;
	}

	WS_SERVER server1;
	if (!Initialize_WS_SERVER(&server1, &master_server, 0)) {
		cout << "error2" << endl;
		return 0;
	}

	if (!Accept_WS_SERVER(&server1)) {
		cout << "error3" << endl;
		return 0;
	}

	//while (true) {
		int64_t time = getTime();

		if (!Send_WS_SERVER(&server1, (char*) &time, sizeof(int64_t))) {
			cout << "error6" << endl;
		}

		if (!Receive_WS_SERVER(&server1, (char*) &time, sizeof(int64_t))) {
			cout << "error7" << endl;
		}

		cout << (getTime() - time) << endl;
	//}

	

	return 0;
}