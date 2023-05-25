//PLATFORMS: Linux, Windows, OSX (TODO)

#ifndef  MOUSE_H_
#define  MOUSE_H_

#include <stdint.h>

#define MOUSE_ABS_COORD				0
#define MOUSE_REL_COORD				1
#define MOUSE_BUTTON_LEFT			1
#define MOUSE_BUTTON_RIGHT			3
#define MOUSE_BUTTON_MIDDLE			2
#define MOUSE_BUTTON_SCROLL_UP		4
#define MOUSE_BUTTON_SCROLL_DOWN	5

struct MOUSE_EVENT_T {
	int16_t x;
	int16_t y;
	int8_t clicked;
	uint16_t button;
	uint16_t state;
	uint32_t event_index; //this will eventually have a wrap around problem
} __attribute__((packed));

#endif
