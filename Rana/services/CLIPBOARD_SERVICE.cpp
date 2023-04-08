#include "CLIPBOARD_SERVICE.h"

bool Initialize_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE *clipboard, KCONTEXT *ctx) {
	clipboard->ctx = ctx;

	clipboard->main_loop = NULL;
	clipboard->main_loop_running = false;

	return true;
}

void Main_Loop_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE *clipboard) {
	string value;
	char curr_clipboard_str[MAX_CLIPBOARD_SIZE];
	char temp_clipboard_str[MAX_CLIPBOARD_SIZE];

	//probably redundant
	memset(curr_clipboard_str, 0, MAX_CLIPBOARD_SIZE);
	memset(temp_clipboard_str, 0, MAX_CLIPBOARD_SIZE);

	while (clipboard->main_loop_running) {
		if (Receive_CLIENT(clipboard->c, curr_clipboard_str, MAX_CLIPBOARD_SIZE)) {
		}
	}
}

bool Connect_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE *clipboard, CLIENT *c) {
	ASSERT_E_R(c != NULL, "Client is NULL", clipboard->ctx);

	clipboard->c = c;

	clipboard->main_loop_running = true;
	clipboard->main_loop = new thread(Main_Loop_CLIPBOARD_SERVICE, clipboard);

	return true;
}

void Disconnect_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE *clipboard) {
	clipboard->main_loop_running = false;
	if (clipboard->main_loop != NULL) {
		clipboard->main_loop->join();
		delete clipboard->main_loop;
		clipboard->main_loop = NULL;
	}
}

void Delete_CLIPBOARD_SERVICE(CLIPBOARD_SERVICE *clipboard) {

}
