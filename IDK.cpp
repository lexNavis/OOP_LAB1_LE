#include <iostream>
#include <Windows.h>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)

using namespace std;

HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
HDC hdc = GetDC(hWnd);
int PEN_WIDTH = 1;

//defining Location methods

Location::Location(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}
Location::~Location() {}

int  Location::getX() { return x; }
int  Location::getY() { return y; }
void Location::setX(int new_x) { x = new_x; }
void Location::setY(int new_y) { y = new_y; }

//defining Fish methods

Fish::Fish(int new_x, int new_y) : Location(new_x, new_y) {}
void Fish::body() {

}
void Fish::rear_fin() {

}
void Fish::bottom_fin() {

}
void Fish::top_fin() {

}
void Fish::eye() {

}
void Fish::mouth() {

}