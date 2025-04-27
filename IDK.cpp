/**************************************************************
*                      ����������������                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: IDK.cpp				                          *
* Programmers:���� �.�., ������� 4, ������ 209�               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: ���������� ������� �������				          *
***************************************************************/

#include <iostream>
#include <Windows.h>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
using namespace std;

int PEN_WIDTH = 1;  // ������� ����
int PPM = 40;		// Pixels Per Move

HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
HDC hdc = GetDC(hWnd);

/**********************************************************
*					���������� �������					  *
**********************************************************/

// ������ � ����� ����� ����� ��������� �� ������� �����
void OperateWith(Fish* fish) {
	bool EXIT_TASK = false;
	while (!EXIT_TASK) {
		cout << "�������� �������� ��� ������� �����: \n"
			<< "1 - �������� �� ������\n"
			<< "2 - ������\n"
			<< "3 - ����������\n"
			<< "������ ����� - ��������� �� �������\n";
		int choice;
		cin >> choice;
		system("cls"); // ������� �������
		switch (choice) {
		case 1: { fish->Show();    break; }
		case 2: { fish->Hide();    break; }
		case 3: { fish->drag(PPM); break; }
		default: {
			EXIT_TASK = true;
			cout << "���������...\n";
			break;
		}
		}
	}
}

// ������������� ������� ������ ��� �� ������������ � �������������
void demonstrate_collisions() {

	const int FISH_COUNT = 3;

	HatFish		HFish(300, 300);
	MutantFish	MFish(700, 300);
	CircleFish	CFish(1000, 300);

	// ������ �� ����� ������������� ������ 
	Fish* Fishes[FISH_COUNT] = { &HFish, &MFish, &CFish };
	// ��������� �� ���������� ���� 
	Fish* pFish = nullptr;

	for (int i = 0; i < FISH_COUNT; i++) {
		pFish = *(Fishes + i);
		pFish->drag(PPM);
	}

}

// ������� ������ �������
void ClearScreen() {
	HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, 0, 0, 1920, 1080);
	DeleteObject(hBrush);
}

/**********************************************************
*				������ ������ Location					  *
**********************************************************/

Location::Location(int new_x, int new_y) {
	x = new_x;
	y = new_y;
}
Location::~Location() {}

int  Location::getX() { return x; }
int  Location::getY() { return y; }
void Location::setX(int new_x) { x = new_x; }
void Location::setY(int new_y) { y = new_y; }

/**********************************************************
*				������ ������ Point						  *
**********************************************************/

Point::Point(int new_x, int new_y) : Location(new_x, new_y) { visible = false; }
Point::~Point() {}

void Point::setVisible(bool new_visible) { visible = new_visible; }
bool Point::isVisible() { return visible; }

// ����������� ����� �� ������
void Point::Show() {
	visible = true;
	SetPixel(hdc, x, y, RGB(255, 0, 0));
}

// �������� �����
void Point::Hide() {
	visible = false;
	SetPixel(hdc, x, y, RGB(255, 255, 255));
}

// ����������� � ��������� ���������� new_x � new_y
void Point::moveTo(int new_x, int new_y) {
	Hide();
	// ����������� "������" � ����� �����
	setX(new_x);
	setY(new_y);
	Show();
}

bool Point::hasCollisionWith(Obstacle* obstacle) { return false; }

void Point::react(Flag* flag) {}
void Point::react(Disc* Disc) {}
void Point::react(Brick* brick) {}

// ��������������� �������� � �������� �����
//void Point::drag(int step) {
//	while (1) {
//		Hide(); //������� ��������� � ������� �����������, ������� ������� ������ �� ��������� ����������
//		// ��������� ������� ������ - ����� �����������
//		if (KEY_DOWN(VK_ESCAPE)) break;
//		else if (KEY_DOWN(VK_LEFT))  x = x - step;
//		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
//		else if (KEY_DOWN(VK_UP))    y = y - step;
//		else if (KEY_DOWN(VK_DOWN))  y = y + step;
//		moveTo(x, y);
//		Sleep(200);
//	}
//}

void Point::drag(int step) {
	Flag AFlag(700, 700, 80, 30);
	Disc ADisc(300, 700, 80, 80);
	Brick ABrick(1000, 700, 80, 30);

	AFlag.Show();
	ADisc.Show();
	ABrick.Show();

	while (1) {
		Hide(); //������� ��������� � ������� �����������, ������� ������� ������ �� ��������� ����������
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		if (hasCollisionWith(&AFlag)) {
			cout << "Collided with Flag!\n";
			react(&AFlag);
			Sleep(2000);
			ClearScreen();
			break;
		}
		if (hasCollisionWith(&ADisc)) {
			cout << "Collided with Disc\n";
			react(&ADisc);
			Sleep(2000);
			ClearScreen();
			break;
		}
		if (hasCollisionWith(&ABrick)) {
			cout << "Collided with Brick\n";
			react(&ABrick);
			Sleep(2000);
			ClearScreen();
			break;
		}
		Sleep(200);//���� ��������� �� ����
	}
}




/**********************************************************
*				������ ������ Fish						  *
**********************************************************/

Fish::Fish(int new_x, int new_y) : Point(new_x, new_y) {
	// ��������� ����
	BODY_FOCUS_X = 100;
	BODY_FOCUS_Y = 40;
	REAR_FIN_HEIGHT = 70;
	REAR_FIN_BASE = 100;
	TOP_FIN_HEIGHT = 50;
	TOP_FIN_BASE = 60;
	BOTTOM_FIN_HEIGHT = -50;
	BOTTOM_FIN_BASE = -60;
	EYE_RADIUS = 7;
	MOUTH_HEIGHT = -30;
	MOUTH_BASE = -8;
}
Fish::~Fish() {}

// ��������� ���� ����
void Fish::body(int clr1 = 127, int clr2 = 255, int clr3 = 0) {	//���������
	// ����� �� �������� ������ ������
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_Y,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_Y
	);
	DeleteObject(hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3));
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_Y,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_Y
	);
	DeleteObject(hBrush);
}

// ��������� ������� ��������
void Fish::rear_fin(int clr1, int clr2, int clr3) {
	//use an array of points to declare a triangle
	POINT* points = new POINT[3];
	points[0] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y - REAR_FIN_BASE / 2 };
	points[1] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y + REAR_FIN_BASE / 2 };
	points[2] = { x - BODY_FOCUS_X + REAR_FIN_HEIGHT / 2, y };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// ��������� ������� ��������
void Fish::bottom_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_Y + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// ��������� �������� ��������
void Fish::top_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_Y - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// ��������� �����
void Fish::eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	//new_x + f1 / 2 + 30, new_y - 10, 7
	//Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
	Ellipse(hdc,
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30,
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}

// ��������� ���
void Fish::mouth(int clr1, int clr2, int clr3) {
	
	POINT* points = new POINT[3];
	points[0] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y - MOUTH_BASE / 2 + 10 };
	points[1] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + MOUTH_BASE / 2 + 10 };
	points[2] = { x + MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + 10 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// ����������� � �������� ����
void Fish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	visible = true;
}
void Fish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	mouth(255, 255, 255);
	visible = false;
}

bool Fish::hasCollisionWith(Obstacle* obstacle) {
	// ��������� �����������
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// ���������� ���.���� � ������� ������� ���� ����� ����
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;

	if (
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		)
		return true;
	else
		return false;
}

void Fish::react(Flag* flag) {}
void Fish::react(Disc* disc) {}
void Fish::react(Brick* brick) {}

/**********************************************************
*				������ ������ HatFish					  *
**********************************************************/

HatFish::HatFish(int new_x, int new_y) : Fish(new_x, new_y) {}
HatFish::~HatFish() {}

// ��������� �����
void HatFish::hat(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80,
		y - BODY_FOCUS_Y - 15,
		x + 80,
		y - BODY_FOCUS_Y + 15
	);
	DeleteObject(hBrush);
}

// ��������� �������� �����
void HatFish::hat_up(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80,
		y - BODY_FOCUS_Y - 15 - 40, //������ �� 20 ��������
		x + 80,
		y - BODY_FOCUS_Y + 15 - 40
	);
	DeleteObject(hBrush);
}

void HatFish::crown(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Rectangle(
		hdc, 
		x - 80,
		y - BODY_FOCUS_Y - 15,
		x + 80,
		y - BODY_FOCUS_Y + 15
	);
	Rectangle(
		hdc,
		x - 80,
		y - BODY_FOCUS_Y - 60,
		x - 40,
		y - BODY_FOCUS_Y - 15
	);
	Rectangle(
		hdc,
		x - 20,
		y - BODY_FOCUS_Y - 60,
		x + 20,
		y - BODY_FOCUS_Y - 15
	);
	Rectangle(
		hdc, 
		x + 40,
		y - BODY_FOCUS_Y - 60,
		x + 80,
		y - BODY_FOCUS_Y - 15
	);
	DeleteObject(hBrush);
}

// ��������������� ������������ ������� Show � Hide
void HatFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	// ������� ������ �� ������� - ������� �����
	hat(255, 0, 0);

	visible = true;
}
void HatFish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	mouth(255, 255, 255);
	hat(255, 255, 255);

	visible = false;
}

bool HatFish::hasCollisionWith(Obstacle* obstacle) {
	// ��������� �����������
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// ���������� ���.���� � ������� ������� ���� ����� ����
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;

	if (
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		)
		return true;
	else
		return false;
}

void HatFish::react(Flag* flag) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);
	hat_up(255, 0, 0);
}

void HatFish::react(Disc* disc) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);
	crown(255, 255, 0);
}

void HatFish::react(Brick* brick) {
	rear_fin(126, 52, 26);
	top_fin(126, 52, 26);
	bottom_fin(126, 52, 26);
	body(126, 52, 26);
	eye(0, 0, 0);
	mouth(126, 52, 26);
	// ������� ������ �� ������� - ������� �����
	hat(126, 52, 26);
}

/**********************************************************
*				������ ������ MutantFish				  *
**********************************************************/

MutantFish::MutantFish(int new_x, int new_y) : Fish(new_x, new_y) {}
MutantFish::~MutantFish() {}

// ��������� ������� �����
void MutantFish::second_eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 20,
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 20,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}

void MutantFish::third_eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 10,
		y - EYE_RADIUS - 20,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 10,
		y + EYE_RADIUS - 20
	);
	DeleteObject(hBrush);
}

// ��������������� ������������ ������� Show � Hide
void MutantFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	// ������� ������ �� ������� - ������� ������� �����
	second_eye(0, 0, 0);
	mouth(169, 169, 169);

	visible = true;
}
void MutantFish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	second_eye(255, 255, 255);
	mouth(255, 255, 255);

	visible = false;
}

bool MutantFish::hasCollisionWith(Obstacle* obstacle) {
	// ��������� �����������
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// ���������� ���.���� � ������� ������� ���� ����� ����
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;

	if (
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		)
		return true;
	else
		return false;
}

void MutantFish::react(Flag* flag) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	// ������� ������ �� ������� - ������� ������� �����
	second_eye(0, 0, 0);
	third_eye(0, 0, 0);
	mouth(169, 169, 169);
}

void MutantFish::react(Disc* disc) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	mouth(169, 169, 169);
}

void MutantFish::react(Brick* brick) {
	Hide();
	body(127, 255, 0);
	eye(0, 0, 0);
	// ������� ������ �� ������� - ������� ������� �����
	second_eye(0, 0, 0);
	mouth(169, 169, 169);
}

/**********************************************************
*				������ ������ CircleFish				  *
**********************************************************/

CircleFish::CircleFish(int new_x, int new_y) : Fish(new_x, new_y) {}
CircleFish::~CircleFish() {}

// ��������������� ������ ���� ����, ��� ��� ����� �� ������, � ���
void CircleFish::body(int clr1, int clr2, int clr3) {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hBrush);
}
// ���������� ����� ����, ��������� ��������� ��������
void CircleFish::bottom_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_X + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void CircleFish::top_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_X - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

void CircleFish::slim_body(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_Y,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_Y
	);
	DeleteObject(hBrush);
}
void CircleFish::fat_body(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X - 16,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X + 16
	);
	DeleteObject(hBrush);
}
void CircleFish::square_body(int clr1, int clr2, int clr3) {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //���������
	SelectObject(hdc, hBrush);
	Rectangle(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hBrush);
}

// ��������������� ������������ ������� Show � Hide
void CircleFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	visible = true;
}
void CircleFish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	mouth(255, 255, 255);

	visible = false;
}

void CircleFish::react(Flag* flag) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	slim_body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);
}
void CircleFish::react(Disc* disc) {
	Hide();
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	fat_body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);
}
void CircleFish::react(Brick* brick) {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	square_body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);
}

bool CircleFish::hasCollisionWith(Obstacle* obstacle) {
	// ��������� �����������
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// ���������� ���.���� � ������� ������� ���� ����� ����
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_X - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_X + 15 + abs(BOTTOM_FIN_BASE) / 2;

	if (
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
		||
		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
		)
		return true;
	else
		return false;
}



/****************************************************************
*						��������� ��� �������					*
****************************************************************/


/****************************************************************
*						Drag � ������������						*
****************************************************************/
//void Fish::drag(int step) {
//	cout << "Enter coordinates of an obstacle (x, y):\n";
//	int new_x, new_y;
//	cin >> new_x >> new_y;
//	Flag AFlag(new_x, new_y, 80, 30);
//	AFlag.Show();
//	while (1) {
//		Hide(); //������� ��������� � ������� �����������, ������� ������� ������ �� ��������� ����������
//		if (KEY_DOWN(VK_ESCAPE)) break;
//		else if (KEY_DOWN(VK_LEFT))  x = x - step;
//		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
//		else if (KEY_DOWN(VK_UP))    y = y - step;
//		else if (KEY_DOWN(VK_DOWN))  y = y + step;
//		moveTo(x, y);
//		if (hasCollisionWith(&AFlag)) {
//			cout << "Collided!\n";
//			react(&AFlag);
//			cout << "Enter (x, y):\n";
//			int new_x, new_y;
//			cin >> new_x >> new_y;
//			AFlag.moveTo(new_x, new_y);
//			//break;
//		}
//		Sleep(200);//���� ��������� �� ����
//	}
//}

/****************************************************************
*					����������� ��������� ���					*
****************************************************************/



/****************************************************************
*		�������� �� ������������ � ������� �� ���� ���� ���		*
****************************************************************/



/****************************************************************
*					������ �����������							*
****************************************************************/

//defining Obstacle methods
Obstacle::Obstacle(int new_x, int new_y, int new_szX, int new_szY) : Location(new_x, new_y) {
	size_x = new_szX;
	size_y = new_szY;
}
Obstacle::~Obstacle() {}
void Obstacle::Show() {}
void Obstacle::Hide() {}
int  Obstacle::getsizeX() { return size_x; }
int  Obstacle::getsizeY() { return size_y; }
void Obstacle::setsizeX(int new_szX) { size_x = new_szX; }
void Obstacle::setsizeY(int new_szY) { size_y = new_szY; }

//defining Flag methods
Flag::Flag(int new_x, int new_y, int new_szX, int new_szY) : Obstacle(new_x, new_y, new_szX, new_szY) {

}
Flag::~Flag() {}
void Flag::Show() {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y - size_y / 6);
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y - size_y / 6, x + size_x / 2, y + size_y / 6);
	hBrush = CreateSolidBrush(RGB(255, 0, 0));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y + size_y / 6, x + size_x / 2, y + size_y / 2);
	DeleteObject(hBrush);

	//visible = true;
}
void Flag::Hide() {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hBrush);

	//visible = false;
}

//Disc methods
Disc::Disc(int new_x, int new_y, int new_szX, int new_szY) : Obstacle(new_x, new_y, new_szX, new_szY) {

}
Disc::~Disc() {}
void Disc::Show() {

	//border of first circle
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH + 1, RGB(0, 0, 0)); SelectObject(hdc, hPen);
	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);

	//inner part of first circle
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 139));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
	DeleteObject(hBrush);

	//border of second circle
	SelectObject(hdc, hPen);
	Ellipse(hdc, x - size_x / 4, y - size_y / 4, x + size_y / 4, y + size_y / 4);

	//inner part of second circle
	hBrush = CreateSolidBrush(RGB(90, 90, 90));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, x - size_x / 4, y - size_y / 4, x + size_y / 4, y + size_y / 4);
	DeleteObject(hBrush);

	//border of third circle
	SelectObject(hdc, hPen);
	Ellipse(hdc, x - size_x / 8, y - size_y / 8, x + size_y / 8, y + size_y / 8);

	//inner part of third circle
	hBrush = CreateSolidBrush(RGB(0, 0, 255));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, x - size_x / 8, y - size_y / 8, x + size_y / 8, y + size_y / 8);
	DeleteObject(hBrush);

	DeleteObject(hPen);
}
void Disc::Hide() {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH + 1, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
	DeleteObject(hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
	DeleteObject(hBrush);
}


// Brick methods
Brick::Brick(int new_x, int new_y, int new_szX, int new_szY) : Obstacle(new_x, new_y, new_szX, new_szY) {

}
Brick::~Brick() {}
void Brick::Show() {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	hBrush = CreateSolidBrush(RGB(126, 52, 26));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hBrush);

	//visible = true;
}
void Brick::Hide() {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
	DeleteObject(hBrush);

	//visible = false;
}

/****************************************************************
*							���������							*
****************************************************************/
// ���� ����� ���� ������� ����� ������� �������
//void HatFish::Hide() {
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //��������� �������������, ���� ������ �����
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
//	SelectObject(hdc, hPen);
//	Rectangle(hdc, x1, y1, x2, y2); //������� ���������
//
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //��� �������
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x1, y1, x2, y2); //������������ ���������
//	DeleteObject(hBrush);
//	DeleteObject(hPen);
//}

