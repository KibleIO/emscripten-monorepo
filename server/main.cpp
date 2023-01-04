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

	WS_SERVER server2;
	if (!Initialize_WS_SERVER(&server2, &master_server, 0)) {
		cout << "error3" << endl;
		return 0;
	}

	Sleep_Milli(2000);

	cout << "successfully connected" << endl;

	Sleep_Milli(2000);


	return 0;
}