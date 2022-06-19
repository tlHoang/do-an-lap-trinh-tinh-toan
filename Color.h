#include <windows.h>

#define blue 0x01
#define aqua 0x03
#define lightBlue 0x0b
#define red 0x0c
#define yellow 0x0e
#define white 0x0f

void set_color(int code) {
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, code);
}
