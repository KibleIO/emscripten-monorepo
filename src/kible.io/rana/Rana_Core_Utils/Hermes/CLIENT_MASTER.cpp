#include "CLIENT_MASTER.h"

bool Initialize_CLIENT_MASTER(CLIENT_MASTER *client, KCONTEXT *ctx, int type,
	int port, char *ip) {

	client->ctx = ctx;
	client->type = type;

	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Initialize_WS_CLIENT_MASTER(
				&client->ws_client_master, ctx, port, ip);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

void Set_Name_CLIENT_MASTER(CLIENT_MASTER *client, char *name) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			Set_Name_WS_CLIENT_MASTER(&client->ws_client_master,
				name);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
}

bool Set_Recv_Timeout_CLIENT_MASTER(CLIENT_MASTER *client, int sec, int usec) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Set_Recv_Timeout_WS_CLIENT_MASTER(
				&client->ws_client_master, sec, usec);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

bool Set_High_Priority_CLIENT_MASTER(CLIENT_MASTER *client) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Set_High_Priority_WS_CLIENT_MASTER(
				&client->ws_client_master);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

void Delete_CLIENT_MASTER(CLIENT_MASTER *client) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			Delete_WS_CLIENT_MASTER(&client->ws_client_master);
			return;
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
}
