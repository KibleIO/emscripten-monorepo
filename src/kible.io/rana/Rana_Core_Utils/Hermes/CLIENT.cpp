#include "CLIENT.h"

bool Initialize_CLIENT(CLIENT *client, KCONTEXT *ctx, CLIENT_MASTER *master,
	int id) {

	client->ctx = ctx;
	client->type = master->type;
	client->master = master;

	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Initialize_WS_CLIENT(&client->ws_client, ctx,
				&master->ws_client_master, id);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

void Set_Name_CLIENT(CLIENT *client, char *name) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			Set_Name_WS_CLIENT(&client->ws_client, name);
			return;
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
}

bool Set_Recv_Timeout_CLIENT(CLIENT *client, int sec, int usec) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Set_Recv_Timeout_WS_CLIENT(&client->ws_client,
				sec, usec);

	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

bool Set_High_Priority_CLIENT(CLIENT *client) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Set_High_Priority_WS_CLIENT(
				&client->ws_client);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

bool Connect_CLIENT(CLIENT *client) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Connect_WS_CLIENT(&client->ws_client);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

bool Send_CLIENT(CLIENT *client, char *buffer, int size) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Send_WS_CLIENT(&client->ws_client, buffer,
				size);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

bool Receive_CLIENT(CLIENT *client, char *buffer, int size) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Receive_WS_CLIENT(&client->ws_client, buffer,
				size);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

int Receive_Unsafe_CLIENT(CLIENT *client, char *buffer) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			return Receive_Unsafe_WS_CLIENT(&client->ws_client,
				buffer);
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
	return false;
}

void Delete_CLIENT(CLIENT *client) {
	switch(client->type) {
		case NETWORK_TYPE_WS:
			Delete_WS_CLIENT(&client->ws_client);
			return;
	}
	LOG_ERROR_CTX((client->ctx)) {
		ADD_STR_LOG("message", "Unknown type");
		ADD_INT_LOG("net_type", client->type);
	}
}
