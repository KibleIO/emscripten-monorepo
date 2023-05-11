#ifndef UI_SERVICE_H_
#define UI_SERVICE_H_

#include "../Rana_Core_Utils/Utilities/ASSERT.h"
#include "../Rana_Core_Utils/Utilities/KCONTEXT.h"

#include <thread>

#include "KEYBOARD_SERVICE.h"
#include "MOUSE_SERVICE.h"
#include "VIDEO_SERVICE.h"

struct UI_SERVICE {
	KCONTEXT *ctx;
	MOUSE_SERVICE *mouse_service;
	KEYBOARD_SERVICE *keyboard_service;
	VIDEO_SERVICE *video_service;
	bool initialized_grpc;

	volatile bool status;
};

bool Initialize_UI_SERVICE(UI_SERVICE *, KCONTEXT *, MOUSE_SERVICE *,
						   KEYBOARD_SERVICE *, VIDEO_SERVICE *);
bool Tick_UI_SERVICE(UI_SERVICE *);
bool Should_Connect_UI_SERVICE(UI_SERVICE *);
bool Connect_UI_SERVICE(UI_SERVICE *);
void Show_Reconnect_UI_SERVICE(UI_SERVICE *);
void Disconnect_UI_SERVICE(UI_SERVICE *);
void Create_Dialog_UI_SERVICE(UI_SERVICE *, string);
void Create_Email_Dialog_UI_SERVICE(UI_SERVICE *, int);
void Create_Error_Dialog_UI_SERVICE(UI_SERVICE *, string);
void Create_Update_Dialog_UI_SERVICE(UI_SERVICE *);
void Create_Trial_Dialog_UI_SERVICE(UI_SERVICE *);
void Hide_Files_Button_UI_SERVICE(UI_SERVICE *);
void Set_UUID_UI_SERVICE(UI_SERVICE *);
void On_Render_UI_SERVICE(UI_SERVICE *);
void Scale_UI_SERVICE(UI_SERVICE *);
bool Status_UI_SERVICE(UI_SERVICE *);
void Delete_UI_SERVICE(UI_SERVICE *);

#endif
