//PLATFORMS: Linux, Windows, OSX (TODO)

#ifndef  KEYBOARD_H_
#define  KEYBOARD_H_

#include <stdint.h>

#define KEYBOARD_ABS_COORD				0
#define KEYBOARD_REL_COORD				1
#define KEYBOARD_BUTTON_LEFT			1
#define KEYBOARD_BUTTON_RIGHT			3
#define KEYBOARD_BUTTON_MIDDLE			2
#define KEYBOARD_BUTTON_SCROLL_UP		4
#define KEYBOARD_BUTTON_SCROLL_DOWN	5

struct KEYBOARD_EVENT_T {
	uint32_t code;
	int32_t value;
	uint32_t event_index; //this will eventually have a wrap around problem
} __attribute__((packed));

#endif
