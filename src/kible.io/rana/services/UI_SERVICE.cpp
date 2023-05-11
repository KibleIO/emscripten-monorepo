#include "UI_SERVICE.h"

bool Initialize_UI_SERVICE(UI_SERVICE *ui, KCONTEXT *ctx,
						   MOUSE_SERVICE *mouse_service,
						   KEYBOARD_SERVICE *keyboard_service,
						   VIDEO_SERVICE *video_service) {
	ui->ctx = ctx;
	ui->mouse_service = mouse_service;
	ui->keyboard_service = keyboard_service;
	ui->video_service = video_service;
	ui->status = true;
	ui->initialized_grpc = false;

	return true;
}

void Scale_UI_SERVICE(UI_SERVICE *ui) {
}

bool Tick_UI_SERVICE(UI_SERVICE *ui) {
}

bool Status_UI_SERVICE(UI_SERVICE *ui) { return ui->status; }

bool Connect_UI_SERVICE(UI_SERVICE *ui) {
	return true;
}

bool Should_Connect_UI_SERVICE(UI_SERVICE *ui) {
	return true;
}

void Disconnect_UI_SERVICE(UI_SERVICE *ui) {
}

void Show_Reconnect_UI_SERVICE(UI_SERVICE *ui) {
}

void Create_Dialog_UI_SERVICE(UI_SERVICE *ui, string message) {
}

void Create_Email_Dialog_UI_SERVICE(UI_SERVICE *ui, int emailState) {
}

void Create_Error_Dialog_UI_SERVICE(UI_SERVICE *ui, string message) {
}

void Create_Update_Dialog_UI_SERVICE(UI_SERVICE *ui) {
}

void Create_Trial_Dialog_UI_SERVICE(UI_SERVICE *ui) {
}

void Hide_Files_Button_UI_SERVICE(UI_SERVICE *ui) {
}

void Set_UUID_UI_SERVICE(UI_SERVICE *ui) {
}

void On_Render_UI_SERVICE(UI_SERVICE *ui) { }

void Delete_UI_SERVICE(UI_SERVICE *ui) { }
