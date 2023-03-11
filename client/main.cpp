#include "WS_CLIENT_MASTER.h"
#include "WS_CLIENT.h"
#include <iostream>

using namespace std;

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

	char hello[100];
	strcpy(hello, "this is the first string");

	if (!Receive_WS_CLIENT(&client, hello, 34)) {
		cout << "coudn't receive6" << endl;
	} else {
		cout << hello << endl;
	}
	strcpy(hello, "this is the second string");



	if (!Send_WS_CLIENT(&client2, hello, 50)) {
		cout << "coudn't send" << endl;
	} else {
		cout << "sent" << endl;
	}
}
