#include <iostream>
#include "utils.h"
#include "WS_SERVER_MASTER.h"
#include "WS_SERVER.h"

using namespace std;

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

	WS_SERVER server2;
	if (!Initialize_WS_SERVER(&server2, &master_server, 0)) {
		cout << "error4" << endl;
		return 0;
	}

	if (!Accept_WS_SERVER(&server2)) {
		cout << "error5" << endl;
		return 0;
	}

	char buffer[100];
	strcpy(buffer, "this is the third string");

	if (!Send_WS_SERVER(&server1, buffer, 34)) {
		cout << "error6" << endl;
	} else {
		cout << "sent bytes" << endl;
	}

	if (!Receive_WS_SERVER(&server2, buffer, 50)) {
		cout << "error7" << endl;
	} else {
		cout << buffer << endl;
	}

	return 0;
}