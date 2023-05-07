#include <iostream>
#include "client/HELLO_WORLD_CLIENT.h"
#include "gen/test.pb.h"

using namespace std;

int main() {
	HELLO_WORLD_CLIENT client;
	gaia::HelloReq request;
	gaia::HelloResp response;

	Initialize_HELLO_WORLD_CLIENT(&client, "http://localhost:3434");

	request.set_subject("hello world123");

	if (Hello_HELLO_WORLD_CLIENT(&client, &request, &response)) {
		cout << response.text() << endl;
	} else {
		cout << "failed!" << endl;
	}

	return 0;
}
