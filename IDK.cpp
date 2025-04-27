/**************************************************************
*                      Программирование                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: IDK.cpp				                          *
* Programmers:Шеин Д.К., Бригада 4, Группа 209Б               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: Реализация методов классов				          *
***************************************************************/

#include <iostream>
#include <Windows.h>
#include "IDK.h"

#define KEY_DOWN(vk) (GetAsyncKeyState(vk) & 0x8000)
using namespace std;

int PEN_WIDTH = 1;  // Толщина пера
int PPM = 40;		// Pixels Per Move

HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
HDC hdc = GetDC(hWnd);

/**********************************************************
*					РЕАЛИЗАЦИЯ ФУНКЦИЙ					  *
**********************************************************/

// Работа с любой рыбой через указатель на базовый класс
void OperateWith(Fish* fish) {
	bool EXIT_TASK = false;
	while (!EXIT_TASK) {
		cout << "Выберите действие над базовой рыбой: \n"
			<< "1 - Показать на экране\n"
			<< "2 - Скрыть\n"
			<< "3 - Перемещать\n"
			<< "Другая цифра - Вернуться на главную\n";
		int choice;
		cin >> choice;
		system("cls"); // очистка консоли
		switch (choice) {
		case 1: { fish->Show();    break; }
		case 2: { fish->Hide();    break; }
		case 3: { fish->drag(PPM); break; }
		default: {
			EXIT_TASK = true;
			cout << "Возращаем...\n";
			break;
		}
		}
	}
}

// Демонстрируем реакцию разных рыб на столкновение с препятствиями
void demonstrate_collisions() {

	// Число рыб для формирования массива рыб
	const int FISH_COUNT = 4;
	Fish		BFish(300, 300);
	HatFish		HFish(300, 300);
	MutantFish	MFish(300, 300);
	SquareFish	CFish(300, 300);

	// Массив со всеми существующими рыбами 
	Fish* fishes[FISH_COUNT] = { &BFish, &HFish, &MFish, &CFish };

	// Указатель на конкретную рыбу 
	Fish* pFish = nullptr;
	
	// Переменная нужна для смены рыбы в зависимости от раздражителя
	int k = 0;

	while(1) {
		pFish = setFish(fishes, k);
		k = pFish->drag(PPM);
	}

}

// Выбираем рыбу из массива рыб и присваиваем указателю
Fish* setFish(Fish** fishes, int pos) {
	return *(fishes + pos);
}

// Очистка экрана консоли
void ClearScreen() {
	HWND hWnd = FindWindowA("ConsoleWindowClass", NULL);
	HDC hdc = GetDC(hWnd);
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	Rectangle(hdc, 0, 0, 1920, 1080);
	DeleteObject(hBrush);
}

/**********************************************************
*				МЕТОДЫ КЛАССА Location					  *
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
*				МЕТОДЫ КЛАССА Point						  *
**********************************************************/

Point::Point(int new_x, int new_y) : Location(new_x, new_y) { visible = false; }
Point::~Point() {}

void Point::setVisible(bool new_visible) { visible = new_visible; }
bool Point::isVisible() { return visible; }

// Отображение точки на экране
void Point::Show() {
	visible = true;
	SetPixel(hdc, x, y, RGB(255, 0, 0));
}

// Сокрытие точки
void Point::Hide() {
	visible = false;
	SetPixel(hdc, x, y, RGB(255, 255, 255));
}

// Перемещение в указанные координаты new_x и new_y
void Point::moveTo(int new_x, int new_y) {
	Hide();
	// Перемещение "центра" в новое место
	setX(new_x);
	setY(new_y);
	Show();
}

bool Point::hasCollisionWith(Obstacle* obstacle) { return false; }


// Продолжительное движение с заданным шагом + обработка столкновений

int Point::drag(int step) {
	Flag AFlag(700, 700, 80, 30);
	Disc ADisc(300, 700, 80, 80);
	Brick ABrick(1000, 700, 80, 30);

	AFlag.Show();
	ADisc.Show();
	ABrick.Show();

	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		if (KEY_DOWN(VK_ESCAPE)) return 0;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		if (hasCollisionWith(&AFlag)) {
			cout << "Collided with Flag!\n";
			Sleep(200);
			ClearScreen();
			// Вернем рыбу на свое место
			setX(300); setY(300);
			return 1;
		}
		if (hasCollisionWith(&ADisc)) {
			cout << "Collided with Disc\n";
			Sleep(200);
			ClearScreen();
			setX(300); setY(300);
			return 2;
		}
		if (hasCollisionWith(&ABrick)) {
			cout << "Collided with Brick\n";
			Sleep(200);
			ClearScreen();
			setX(300); setY(300);
			return 3;
		}
		Sleep(200);//чтоб эпилепсии не было
	}
}




/**********************************************************
*				МЕТОДЫ КЛАССА Fish						  *
**********************************************************/

Fish::Fish(int new_x, int new_y) : Point(new_x, new_y) {
	// Параметры рыбы
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

// Рисование тела рыбы
void Fish::body(int clr1 = 127, int clr2 = 255, int clr3 = 0) {	//салатовый
	// Чтобы не обводило черным цветом
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

// Рисование заднего плавника
void Fish::rear_fin(int clr1, int clr2, int clr3) {
	// Массив вершин задает многоугольник(в нашем случае, треугольник)
	POINT* points = new POINT[3];
	points[0] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y - REAR_FIN_BASE / 2 };
	points[1] = { x - BODY_FOCUS_X - REAR_FIN_HEIGHT / 2, y + REAR_FIN_BASE / 2 };
	points[2] = { x - BODY_FOCUS_X + REAR_FIN_HEIGHT / 2, y };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Рисование нижнего плавника
void Fish::bottom_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_Y + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Рисование верхнего плавника
void Fish::top_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_Y - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_Y - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Рисование глаза
void Fish::eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30,
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}

// Рисование рта
void Fish::mouth(int clr1, int clr2, int clr3) {

	POINT* points = new POINT[3];
	points[0] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y - MOUTH_BASE / 2 + 10 };
	points[1] = { x - MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + MOUTH_BASE / 2 + 10 };
	points[2] = { x + MOUTH_HEIGHT / 2 + BODY_FOCUS_X - 20, y + 10 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Отображение рыбы
void Fish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	visible = true;
}

// Сокрытие рыбы
void Fish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	mouth(255, 255, 255);
	visible = false;
}

// Определяет, столкнулась ли рыба с препятствием
bool Fish::hasCollisionWith(Obstacle* obstacle) {
	// Параметры препятствия
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// Коориднаты лев.верх и правого нижнего угла грани рыбы
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

/**********************************************************
*				МЕТОДЫ КЛАССА HatFish					  *
**********************************************************/

HatFish::HatFish(int new_x, int new_y) : Fish(new_x, new_y) {}
HatFish::~HatFish() {}

// Рисование шляпы
void HatFish::hat(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //салатовый
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	//focusX = 80, focusY = 15
		x - 80,
		y - BODY_FOCUS_Y - 15,
		x + 80,
		y - BODY_FOCUS_Y + 15
	);
	DeleteObject(hBrush);
}

// Переопределение родительских методов Show и Hide
void HatFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	// Отличие предка от потомка - наличие шляпы
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
	// Параметры препятствия
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// Коориднаты лев.верх и правого нижнего угла грани рыбы
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


/**********************************************************
*				МЕТОДЫ КЛАССА MutantFish				  *
**********************************************************/

MutantFish::MutantFish(int new_x, int new_y) : Fish(new_x, new_y) {}
MutantFish::~MutantFish() {}

// Рисование второго глаза
void MutantFish::second_eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //салатовый
	SelectObject(hdc, hBrush);
	Ellipse(hdc,	
		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 20,
		y - EYE_RADIUS - 10,
		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 20,
		y + EYE_RADIUS - 10
	);
	DeleteObject(hBrush);
}

// Переопределение родительских методов Show и Hide
void MutantFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	// Отличие предка от потомка - наличие второго глаза
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
	// Параметры препятствия
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// Коориднаты лев.верх и правого нижнего угла грани рыбы
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


/**********************************************************
*				МЕТОДЫ КЛАССА SquareFish				  *
**********************************************************/

SquareFish::SquareFish(int new_x, int new_y) : Fish(new_x, new_y) {}
SquareFish::~SquareFish() {}

// Рисование нижнего плавника рыбы - квадрата
void SquareFish::bottom_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 25, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_X + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_X + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Рисование верхнего плавника рыбы - квадрата
void SquareFish::top_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - TOP_FIN_HEIGHT / 2 + 25, y - TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[1] = { x - TOP_FIN_HEIGHT / 2 + 25, y + TOP_FIN_BASE / 2 - BODY_FOCUS_X - 15 };
	points[2] = { x + TOP_FIN_HEIGHT / 2 + 25, y - BODY_FOCUS_X - 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Рисование тела рыбы - квадрата
void SquareFish::square_body(int clr1, int clr2, int clr3) {
	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
	SelectObject(hdc, hPen);
	Rectangle(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hPen);
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); 
	SelectObject(hdc, hBrush);
	Rectangle(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hBrush);
}

// Переопределение родительских методов Show и Hide
void SquareFish::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	square_body(126, 52, 26);
	eye(0, 0, 0);
	mouth(169, 169, 169);

	visible = true;
}
void SquareFish::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	square_body(255, 255, 255);
	eye(255, 255, 255);
	mouth(255, 255, 255);

	visible = false;
}

bool SquareFish::hasCollisionWith(Obstacle* obstacle) {
	// Параметры препятствия
	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
	// Коориднаты лев.верх и правого нижнего угла грани рыбы
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


/**********************************************************
*				МЕТОДЫ КЛАССА Obstacle				      *
**********************************************************/

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

/**********************************************************
*				МЕТОДЫ КЛАССА Flag   				      *
**********************************************************/

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

/**********************************************************
*				МЕТОДЫ КЛАССА Disc   				      *
**********************************************************/

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


/**********************************************************
*				МЕТОДЫ КЛАССА Brick   				      *
**********************************************************/

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

	
}


