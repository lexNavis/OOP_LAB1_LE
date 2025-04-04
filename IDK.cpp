#include <iostream>
#include <Windows.h>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
using namespace std;

HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
HDC hdc = GetDC(hWnd);
int PEN_WIDTH = 1;
//��������� ����
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

//�������� ��������� ����� � �������
//bool IsInArea(int x, int y, int x1, int x2, int y1, int y2) {
//
//}


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
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //���������
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

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
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

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
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

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void Fish::eye() {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); //��� �������
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

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
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
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //�� �������������, ���� ������ �����
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //������� ���������

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //��� �������
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //������������ ���������
	DeleteObject(hBrush);
	DeleteObject(hPen);
}
// ���������� ������������ ��������������, 
// � ������� ��������� ������, � �������������� �����������
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
void Fish::moveTo(int new_x, int new_y) { //����������� ��� �������
	Hide();
	setX(new_x);
	setY(new_y);
	Show();
}

void Fish::drag(int step) {
	cout << "Enter (x, y):\n";
	int new_x, new_y;
	cin >> new_x >> new_y;
	Flag AFlag(new_x,new_y, 80, 30);
	AFlag.Show();
	while (1) {
		Hide(); //������� ��������� � ������� �����������, ������� ������� ������ �� ��������� ����������
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		if (hasCollisionWith(&AFlag)) {
			cout << "Collided!\n";
			react(&AFlag);
			cout << "Enter (x, y):\n";
			int new_x, new_y;
			cin >> new_x >> new_y;
			AFlag.Hide();
			AFlag.setX(new_x);
			AFlag.setY(new_y);
			AFlag.Show();
			//break;
		}
		Sleep(200);//���� ��������� �� ����
	}
}

//HatFish
HatFish::HatFish(int new_x, int new_y) : Fish(new_x, new_y) {}
HatFish::~HatFish() {}

void HatFish::hat() {
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80,
		y - BODY_FOCUS_Y - 15,
		x + 80,
		y - BODY_FOCUS_Y + 15
	);
	DeleteObject(hBrush);
}

void HatFish::hat_up() {
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80, 
		y - BODY_FOCUS_Y - 15 - 20, //������ �� 20 ��������
		x + 80,
		y - BODY_FOCUS_Y + 15 - 20
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
//��� �������, ��� ������� �� ���������� :(
void HatFish::Hide() {
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //�� ������������� ���� �� -1 ���������
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //������� ���������

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //��� �������
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //������������ ���������
	DeleteObject(hBrush);
	DeleteObject(hPen);
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
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	mouth();
	hat_up();
}

//MutantFish
MutantFish::MutantFish(int new_x, int new_y) : Fish(new_x, new_y) {}
MutantFish::~MutantFish() {}

void MutantFish::second_eye() {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 20,
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 20,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}
void MutantFish::third_eye() {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 10,
		y - EYE_RADIUS - 20,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 10,
		y + EYE_RADIUS - 20
	);
	DeleteObject(hBrush);
}

void MutantFish::Show() {
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	second_eye();
	mouth();
}
//��� �������, ��� ������� �� ���������� :(
void MutantFish::Hide() {
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //�� ������������� ���� �� -1 ���������
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //������� ���������

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //��� �������
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //������������ ���������
	DeleteObject(hBrush);
	DeleteObject(hPen);
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
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	second_eye();
	third_eye();
	mouth();
}

//CircleFish
CircleFish::CircleFish(int new_x, int new_y) : Fish(new_x, new_y) {}
CircleFish::~CircleFish() {}

void CircleFish::body() {
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hBrush);
}

void CircleFish::slim_body() {
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_Y,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_Y
	);
	DeleteObject(hBrush);
}

void CircleFish::fat_body() {
	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //���������
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X - 16,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X + 16
	);
	DeleteObject(hBrush);
}

void CircleFish::bottom_fin() {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_X + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}
void CircleFish::top_fin() {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_X - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(169, 169, 169)); //��� �������
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

void CircleFish::Show() {
	rear_fin();
	top_fin();
	bottom_fin();
	body();
	eye();
	mouth();
}
void CircleFish::Hide() {
	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
		x2 = x + BODY_FOCUS_X,
		y1 = y - BODY_FOCUS_X - 15 - abs(TOP_FIN_BASE) / 2,
		y2 = y + BODY_FOCUS_X + 15 + abs(BOTTOM_FIN_BASE) / 2; //�� ������������� ���� �� -1 ���������
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc, x1, y1, x2, y2); //������� ���������

	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //��� �������
	SelectObject(hdc, hBrush);
	Rectangle(hdc, x1, y1, x2, y2); //������������ ���������
	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void CircleFish::react(Flag* flag) {
	Hide();
	rear_fin();
	top_fin();
	bottom_fin();
	slim_body();
	eye();
	mouth();
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

//defining Obstacle methods
Obstacle::Obstacle(int new_x, int new_y, int new_szX, int new_szY) : Location(new_x, new_y) {
	size_x = new_szX;
	size_y = new_szY;
}
Obstacle::~Obstacle() {}
//void Obstacle::Show() {}
//void Obstacle::Hide() {}
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
}

