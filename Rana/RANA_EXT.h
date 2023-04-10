#ifndef RANA_EXT_H_
#define RANA_EXT_H_

#include "Rana_Core_Utils/Hermes/HERMES_CLIENT.h"
#include "Rana_Core_Utils/Utilities/ASSERT.h"
#include <sys/stat.h>

#include "LIMITS.h"
#include "STRINGS.h"
#include "services/AUDIO_SERVICE.h"
#include "services/CLIPBOARD_SERVICE.h"
#include "services/KEYBOARD_SERVICE.h"
#include "services/MOUSE_SERVICE.h"
#include "services/THEMIS_SERVICE.h"
#include "services/UI_SERVICE.h"
#include "services/VIDEO_SERVICE.h"

struct RANA_EXT {
	KCONTEXT *ctx;
	

	string themis_ip;
	string error_string;
	bool initialized_hermes;

	HERMES_CLIENT hermes_client;
	
	UI_SERVICE ui;
	VIDEO_SERVICE video;
	THEMIS_SERVICE themis;
	MOUSE_SERVICE mouse;
	KEYBOARD_SERVICE keyboard;
	AUDIO_SERVICE audio;
	CLIPBOARD_SERVICE clipboard;
};

bool Initialize_RANA_EXT(RANA_EXT*, KCONTEXT*);
bool Running_RANA_EXT(RANA_EXT*);
void Render_RANA_EXT(RANA_EXT*);
void Check_Lets_Go_RANA_EXT(RANA_EXT*);
bool Connect_To_Themis_RANA_EXT(RANA_EXT*);
uint8_t Themis_Status_RANA_EXT(RANA_EXT*);
void Show_Reconnect_UI_RANA_EXT(RANA_EXT *);
void Disconnect_From_Themis_RANA_EXT(RANA_EXT *, bool);
void Delete_RANA_EXT(RANA_EXT *);

#endif
