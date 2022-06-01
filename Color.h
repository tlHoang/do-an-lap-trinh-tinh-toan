#include <windows.h>

#define red 0x0c
#define yellow 0x0e
#define white 0x0f
#define lightBlue 0x0b

void set_color(int code) {
	HANDLE color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, code);
}
