#include <iostream>
#include <Windows.h>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
using namespace std;

HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
HDC hdc = GetDC(hWnd);
//ѕараметры рыбы
int PEN_WIDTH = 1;
int BODY_FOCUS_X = 100;
int BODY_FOCUS_Y = 40;
int REAR_FIN_HEIGHT = 70;
int REAR_FIN_BASE = 100;
int TOP_FIN_HEIGHT = 50;
int TOP_FIN_BASE = 60;
int BOTTOM_FIN_HEIGHT = -50;
int BOTTOM_FIN_BASE = -60;
int EYE_RADIUS = 7;
int MOUTH_HEIGHT = -30;
int MOUTH_BASE = -8;

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
Fish::~Fish() {}

void Fish::body() {
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //салатовый
	SelectObject(hdc, hBrush);
	Ellipse(hdc, 
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_Y,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_Y
	);
	DeleteObject(hBrush);
}
void Fish::rear_fin() {
	//use an array of points to declare a triangle
	POINT* points = new POINT[3];
	points[0] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y - REAR_FIN_BASE / 2 };
	points[1] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y + REAR_FIN_BASE / 2 };
	points[2] = { x - BODY_FOCUS_X + REAR_FIN_HEIGHT / 2, y };

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void Fish::bottom_fin() {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_Y + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void Fish::top_fin() {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_Y - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void Fish::eye() {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); //дл€ заливки
	SelectObject(hdc, hBrush);
	//new_x + f1 / 2 + 30, new_y - 10, 7
	//Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	Ellipse(hdc, 
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 , 
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}
void Fish::mouth() {
	//mouth = new Triangle(new_x + f1 - 20, new_y + 10, -30, -8);
	POINT* points = new POINT[3];
	points[0] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y - MOUTH_BASE / 2 + 10 };
	points[1] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + MOUTH_BASE / 2 + 10 };
	points[2] = { x + MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + 10};

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void Fish::Show() {
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	mouth();
}
void Fish::Hide() {
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //он отрицательный надо на -1 домножить
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //границу закрасить

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //внутренность закрасить
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void Fish::moveTo(int new_x, int new_y) { //перемещение без обводки
	Hide();
	setX(new_x);
	setY(new_y);
	Show();
}

void Fish::drag(int step) {
	while (1) {
		Hide(); //«атирка прив€зана к текущим координатам, заранее стираем объект до изменени€ координаты
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(100);//чтоб эпилепсии не было
	}
}

//HatFish
HatFish::HatFish(int new_x, int new_y) : Fish(new_x, new_y) {}
HatFish::~HatFish() {}

void HatFish::hat() {
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); //салатовый
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80,
		y - BODY_FOCUS_Y - 15,
		x + 80,
		y - BODY_FOCUS_Y + 15
	);
	DeleteObject(hBrush);
}

void HatFish::Show() {
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	mouth();
	hat();
}
//“ак выходит, что функци€ то одинакова€ :(
void HatFish::Hide() {
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //он отрицательный надо на -1 домножить
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //границу закрасить

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //дл€ заливки
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //внутренность закрасить
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
