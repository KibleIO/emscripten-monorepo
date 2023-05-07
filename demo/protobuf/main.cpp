#include <iostream>
#include "gen/test.pb.h"
#include <google/protobuf/util/json_util.h>

using namespace std;

int main() {
	std::string json_string;

	gaia::HelloReq request;
	request.set_subject("hello world");

	google::protobuf::util::JsonPrintOptions options;
	options.add_whitespace = false;
	options.always_print_primitive_fields = true;
	MessageToJsonString(request, &json_string, options);

	cout << json_string << endl;
	return 0;
}
