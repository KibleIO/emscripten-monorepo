#include "RANA_EXT.h"

bool Initialize_RANA_EXT(RANA_EXT *rana_ext, KCONTEXT *ctx) {
	rana_ext->ctx = ctx;
	rana_ext->error_string = "";
	rana_ext->initialized_hermes = false;

    	ASSERT_E_R((Initialize_MOUSE_SERVICE(&rana_ext->mouse,
		rana_ext->ctx)), "Couldn't initialize mouse service",
		rana_ext->ctx);
	ASSERT_E_R((Initialize_KEYBOARD_SERVICE(&rana_ext->keyboard,
		rana_ext->ctx)), "Couldn't initialize keyboard service",
		rana_ext->ctx);
	ASSERT_E_R((Initialize_VIDEO_SERVICE(&rana_ext->video,
		rana_ext->ctx, &rana_ext->mouse, &rana_ext->keyboard)), "Couldn't initialize video service",
		rana_ext->ctx);
	ASSERT_E_R((Initialize_UI_SERVICE(&rana_ext->ui,
		rana_ext->ctx, &rana_ext->mouse, &rana_ext->keyboard,
		&rana_ext->video)),
		"Couldn't initialize UI service",
		rana_ext->ctx);
        ASSERT_E_R((Initialize_AUDIO_SERVICE(&rana_ext->audio, rana_ext->ctx)),
		"Couldn't initialize audio service",
		rana_ext->ctx);
	ASSERT_E_R((Initialize_CLIPBOARD_SERVICE(&rana_ext->clipboard, rana_ext->ctx)),
		"Couldn't initialize clipboard service",
		rana_ext->ctx);
	ASSERT_E_R((Initialize_THEMIS_SERVICE(&rana_ext->themis,
		rana_ext->ctx)), "Couldn't initialize Themis service",
		rana_ext->ctx);

	return true;
}

bool Connect_To_Themis_RANA_EXT(RANA_EXT *rana_ext) {
	HERMES_TYPE themis_types[] = {HERMES_CLIENT_WS, HERMES_THEMIS_WS,
		HERMES_VIDEO_INIT_WS, HERMES_VIDEO_WS, HERMES_MOUSE_WS, HERMES_KEYBOARD_WS,
		HERMES_AUDIO_WS, HERMES_CLIPBOARD_WS, HERMES_NULL};

	TIMER t;

	LOG_INFO_CTX(rana_ext->ctx) {
		ADD_STR_LOG("message", "Attempting signin go.");
	}

	std::string themis_url;
	int port;

	#ifdef TESTING_BUILD

	port = THEMIS_PORT;
	themis_url = "localhost";
	std::string themis_api = "http://localhost:4461";

	if (!Launch_THEMIS_CLIENT(themis_api)) {
		LOG_ERROR_CTX(rana_ext->ctx) {
			ADD_STR_LOG("message", "Signin failed.");
			ADD_STR_LOG("error", "Couldn't launch themis server");
		}
		return false;
	}

	#else

	port = 443;
	if (!Themis_EDGE_CLIENT(std::string("https://") + rana_ext->ctx->url,
		rana_ext->ctx->uuid, &themis_url)) {
		
		LOG_ERROR_CTX(rana_ext->ctx) {
			ADD_STR_LOG("message", "Signin failed.");
			ADD_STR_LOG("error", "Couldn't query for edge server");
		}

		cout << "failed to query edge" << endl;
		return false;
	}

	std::string delimiter = "alienhub.xyz";
	themis_url = themis_url.substr(0, themis_url.find(delimiter)) +
		rana_ext->ctx->url + themis_url.substr(
		themis_url.find(delimiter) + delimiter.length());

	if (!Launch_THEMIS_CLIENT(std::string("https://") + themis_url)) {
		LOG_ERROR_CTX(rana_ext->ctx) {
			ADD_STR_LOG("message", "Signin failed.");
			ADD_STR_LOG("error", "Couldn't launch themis server");
		}
		return false;
	}

	#endif

	rana_ext->error_string = SKIPPED_INIT_STRINGS;
	if (!rana_ext->initialized_hermes) {
		if (!Initialize_HERMES_CLIENT(
				&rana_ext->hermes_client, rana_ext->ctx,
				(char*) themis_url.c_str(),
				port)) {
			LOG_ERROR_CTX(rana_ext->ctx) {
				ADD_STR_LOG("message", "Signin failed.");
				ADD_STR_LOG("error", "Couldn't initialize hermes server");
			}

			return false;
		}

		rana_ext->initialized_hermes = true;
	}

	// connect to themis
	rana_ext->error_string = NO_THEMIS_STRINGS;

	if (!Connect_HERMES_CLIENT(&rana_ext->hermes_client, themis_types)) {
		LOG_ERROR_CTX(rana_ext->ctx) {
			ADD_STR_LOG("message", "Signin failed.");
			ADD_STR_LOG("error", "Could not connect hermes");
		}

		return false;
	}

	ASSERT_E_R((Connect_THEMIS_SERVICE(
				   &rana_ext->themis,
				   Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_THEMIS_WS),
				   &rana_ext->hermes_client)),
			   "Could not connect themis service", rana_ext->ctx);

	ASSERT_E_R((Connect_MOUSE_SERVICE(
				   &rana_ext->mouse,
				   Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_MOUSE_WS))),
			   "Could not connect mouse service", rana_ext->ctx);

	ASSERT_E_R(
		(Connect_KEYBOARD_SERVICE(
			&rana_ext->keyboard,
			Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_KEYBOARD_WS))),
		"Could not connect keyboard service", rana_ext->ctx);

	ASSERT_E_R((Connect_AUDIO_SERVICE(
				   &rana_ext->audio,
				   Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_AUDIO_WS))),
			   "Could not connect audio service", rana_ext->ctx);

	ASSERT_E_R(
		(Connect_VIDEO_SERVICE(
			&rana_ext->video,
			Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_VIDEO_INIT_WS),
			Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_VIDEO_WS))),
		"Could not connect video service", rana_ext->ctx);

	ASSERT_E_R(
		(Connect_CLIPBOARD_SERVICE(
			&rana_ext->clipboard,
			Get_HERMES_CLIENT(&rana_ext->hermes_client, HERMES_CLIPBOARD_WS))),
		"Could not connect clipboard service", rana_ext->ctx);

	ASSERT_E_R(Connect_UI_SERVICE(&rana_ext->ui), "Could not connect the UI",
			   rana_ext->ctx);
	// end connect to themis

	rana_ext->error_string = "";

	LOG_INFO_CTX(rana_ext->ctx) {
		ADD_STR_LOG("message", "Successful signin go.");
	}

	return true;
}

void Disconnect_From_Themis_RANA_EXT(RANA_EXT *rana_ext, bool skip_ui_disconnect) {
	// check if the is a regular disconnect or an error
	if (rana_ext->error_string != "" && !skip_ui_disconnect) {
		LOG_ERROR_CTX((rana_ext->ctx)) {
			ADD_STR_LOG("message", "Rana failed to connect");
			ADD_STR_LOG("error", rana_ext->error_string.c_str());
		}
		Create_Dialog_UI_SERVICE(&rana_ext->ui, rana_ext->error_string);

		rana_ext->error_string = "";
	}

	if (rana_ext->initialized_hermes) {
		Disconnect_HERMES_CLIENT(&rana_ext->hermes_client);
	}
	Disconnect_VIDEO_SERVICE(&rana_ext->video);
	Disconnect_KEYBOARD_SERVICE(&rana_ext->keyboard);
	Disconnect_MOUSE_SERVICE(&rana_ext->mouse);
	Disconnect_AUDIO_SERVICE(&rana_ext->audio);
	Disconnect_CLIPBOARD_SERVICE(&rana_ext->clipboard);
	if (!skip_ui_disconnect) {
		Disconnect_UI_SERVICE(&rana_ext->ui);
	}
	Disconnect_THEMIS_SERVICE(&rana_ext->themis);
}

void Delete_RANA_EXT(RANA_EXT *rana_ext) {
	Delete_UI_SERVICE(&rana_ext->ui);
	Delete_KEYBOARD_SERVICE(&rana_ext->keyboard);
	Delete_MOUSE_SERVICE(&rana_ext->mouse);
	Delete_VIDEO_SERVICE(&rana_ext->video);
	Delete_AUDIO_SERVICE(&rana_ext->audio);
	Delete_CLIPBOARD_SERVICE(&rana_ext->clipboard);
	Delete_THEMIS_SERVICE(&rana_ext->themis);
	Delete_HERMES_CLIENT(&rana_ext->hermes_client);
}

uint8_t Themis_Status_RANA_EXT(RANA_EXT *rana_ext) {
	uint8_t status = Themis_Status_THEMIS_SERVICE(&rana_ext->themis);

	if (status == HERMES_STATUS_NORMAL &&
		(!Status_VIDEO_SERVICE(&rana_ext->video) ||
		 !Status_UI_SERVICE(&rana_ext->ui))) {
		// okay so what happened here? Basically if Status_VIDEO_SERVICE
		// is false then the video service went down signaling an issue
		// with the video... probably a header or something
		status = HERMES_STATUS_UNEXPECTED_DISCONNECT;
	}
	return status;
}

void Render_RANA_EXT(RANA_EXT *rana_ext) {
	if (!Tick_UI_SERVICE(&rana_ext->ui)) {
		Scale_UI_SERVICE(&rana_ext->ui);
	}
}

bool Running_RANA_EXT(RANA_EXT *rana_ext) {
	return true;  // lol
}