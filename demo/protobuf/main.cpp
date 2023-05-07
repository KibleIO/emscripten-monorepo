#include <iostream>
#include "client/HELLOWORLD_CLIENT.h"
#include "gen/test.pb.h"

using namespace std;

int main() {
	HELLOWORLD_CLIENT client;
	std::string response;

	Initialize_HELLOWORLD_CLIENT(&client, "http://localhost:3434");

	if (Hello_HELLOWORLD_CLIENT(&client, "hello world", &response)) {
		cout << response << endl;
	} else {
		cout << "failed!" << endl;
	}

	return 0;
}
