#include "WS_CLIENT_MASTER.h"
#include "../../Utilities/TIMER.h"

#ifdef __EMSCRIPTEN__

EM_BOOL On_Open_WS_CLIENT_MASTER(int eventType,
	const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData) {

	WS_CLIENT_MASTER *client = (WS_CLIENT_MASTER*) userData;

	client->socket = websocketEvent->socket;
	client->accept = true;

	return EM_TRUE;
}

EM_BOOL On_Error_WS_CLIENT_MASTER(int eventType,
	const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData) {

	WS_CLIENT_MASTER *client = (WS_CLIENT_MASTER*) userData;
	client->accept = false;
	return EM_TRUE;
}

EM_BOOL On_Close_WS_CLIENT_MASTER(int eventType,
	const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData) {

	WS_CLIENT_MASTER *client = (WS_CLIENT_MASTER*) userData;
	client->accept = false;

	return EM_TRUE;
}

EM_BOOL On_Message_WS_CLIENT_MASTER(int eventType,
	const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData) {

	WS_CLIENT_MASTER *client = (WS_CLIENT_MASTER*) userData;
	WEBSOCKET_ELEMENT *temp = NULL;
	uint32_t time_out = 0;

	int len = websocketEvent->numBytes;

	if (int(websocketEvent->data[0]) < 0 ||
		int(websocketEvent->data[0]) >= MAX_HOSTS) {
		//log_err("invalid host id.");
		return EM_FALSE;
	}

	if (client->consumers[int(websocketEvent->data[0])].user_ptr != NULL) {
		client->consumers[int(websocketEvent->data[0])].callback(
			client->consumers[int(websocketEvent->data[0])].user_ptr,
			(char*)websocketEvent->data + 1,
			len - 1);
	}

	return EM_TRUE;
}

#endif

bool Initialize_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client, KCONTEXT *ctx,
	int port, char *address) {
	
	//client->ctx = ctx;
	client->host_count = 0;
	client->accept = false;

	Set_Name_WS_CLIENT_MASTER(client, "unknown");

	for (int i = 0; i < MAX_HOSTS; i++) {
		client->consumers[i].user_ptr = NULL;
	}

	#ifdef __EMSCRIPTEN__

	if (!emscripten_websocket_is_supported()) {
		return false;
	}

	string url;

	if (port == DEFAULT_SSL_PORT) {
		url += "wss://";
	} else {
		url += "ws://";
	}

	url += address;
	url += ":";
	url += to_string(port);

	EmscriptenWebSocketCreateAttributes ws_attrs = {
		url.c_str(), NULL, EM_FALSE
	};

	EMSCRIPTEN_WEBSOCKET_T ws = emscripten_websocket_new(&ws_attrs);
	emscripten_websocket_set_onopen_callback(ws, (void*) client,
		On_Open_WS_CLIENT_MASTER);
	emscripten_websocket_set_onerror_callback(ws, (void*) client,
		On_Error_WS_CLIENT_MASTER);
	emscripten_websocket_set_onclose_callback(ws, (void*) client,
		On_Close_WS_CLIENT_MASTER);
	emscripten_websocket_set_onmessage_callback(ws, (void*) client,
		On_Message_WS_CLIENT_MASTER);
	
	while (!client->accept) {
		Sleep_Milli(WEB_SOCKET_SLEEP_TIME);
	}

	#endif

	return true;
}

bool Send_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client,
	uint8_t* bytes, uint32_t size, uint8_t client_index) {

	if (size > (MAX_WEBSOCKET_PACKET_SIZE - 1)) {
		//log_err("sending large packet. truncated.");
		size = MAX_WEBSOCKET_PACKET_SIZE - 1;
	}
	
	uint8_t	send_bytes[MAX_WEBSOCKET_PACKET_SIZE];

	send_bytes[0] = client_index;
	copy(bytes, bytes + size, &send_bytes[1]);

	#ifdef __EMSCRIPTEN__

	emscripten_websocket_send_binary(client->socket, send_bytes, size + 1);

	#endif

	return true;
}

uint8_t Register_Vhost_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client,
	Receive_Callback_SOCKET_CLIENT callback, void *user_ptr) {

	if (client->host_count >= MAX_HOSTS) {
		//log_err("Too many hosts.");
		return -1;
	}

	client->consumers[int(client->host_count)].user_ptr = user_ptr;
	client->consumers[int(client->host_count)].callback = callback;

	return client->host_count++;
}

void Set_Name_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client, char *name) {
	strcpy(client->name, name);
}

bool Set_Recv_Timeout_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client, int sec,
	int usec) {

	return true;
}

bool Set_High_Priority_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client) {
	return true;
}

void Delete_WS_CLIENT_MASTER(WS_CLIENT_MASTER *client) {
}
