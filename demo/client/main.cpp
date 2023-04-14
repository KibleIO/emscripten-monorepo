#include "WS_CLIENT_MASTER.h"
#include "WS_CLIENT.h"
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

using namespace std;

int64_t getTime() {
	timeval start;
	gettimeofday(&start, NULL);
	return ((start.tv_sec) * 1000 + start.tv_usec / 1000.0) + 0.5;
}

int main() {
	cout << "step 0" << endl;

	WS_CLIENT_MASTER client_master;
	if (!Initialize_WS_CLIENT_MASTER(&client_master, 4000, "127.0.0.1")) {
		cout << "couldn't connect1" << endl;
		return 0;
	}

	cout << "step 1" << endl;

	WS_CLIENT client;
	if (!Initialize_WS_CLIENT(&client, &client_master, 0)) {
		cout << "couldn't connect2" << endl;
		return 0;
	}

	cout << "step 2" << endl;

	if (!Connect_WS_CLIENT(&client)) {
		cout << "couldn't connect3" << endl;
		return 0;
	}

	cout << "step 3" << endl;

	WS_CLIENT client2;
	if (!Initialize_WS_CLIENT(&client2, &client_master, 0)) {
		cout << "couldn't connect4" << endl;
		return 0;
	}

	cout << "step 4" << endl;

	if (!Connect_WS_CLIENT(&client2)) {
		cout << "couldn't connect5" << endl;
		return 0;
	}

	int64_t time, time2;

	if (!Receive_WS_CLIENT(&client, (char*) &time, sizeof(int64_t))) {
		cout << "coudn't receive6" << endl;
	}

	time2 = getTime();

	if (!Send_WS_CLIENT(&client2, (char*) &time2, sizeof(int64_t))) {
		cout << "coudn't send" << endl;
	}

	cout << (time2 - time) << endl;
}
