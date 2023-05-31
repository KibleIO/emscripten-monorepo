#include "UTILS.h"

void Hide_Loading_Wheel() {
	EM_ASM(
		var loadingImage = document.getElementById('loadingimg');
		loadingImage.style.display = 'none';
	);
}

void Show_Error_Message(std::string str) {
	EM_ASM(
		var loadingImage = document.getElementById('loadingimg');
		var status = document.getElementById('status');
		loadingImage.style.display = 'none';
		status.innerHTML = UTF8ToString($0);
	, str.c_str());
}