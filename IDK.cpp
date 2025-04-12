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

// Параметры рыбы
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

// Продолжительное движение с заданным шагом
void Point::drag(int step) {
	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		// Обработка нажатия клавиш - выбор направления
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(200);
	}
}

/**********************************************************
*				МЕТОДЫ КЛАССА Fish						  *
**********************************************************/

Fish::Fish(int new_x, int new_y) : Point(new_x, new_y) {}
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
	//use an array of points to declare a triangle
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
void Fish::eye(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
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
void Fish::mouth(int clr1, int clr2, int clr3) {
	//mouth = new Triangle(new_x + f1 - 20, new_y + 10, -30, -8);
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

// Отображение и сокрытие рыбы
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
void Fish::moveTo(int new_x, int new_y) {
	Hide();
	// Перемещение "центра" в новое место
	setX(new_x);
	setY(new_y);
	Show();
}

// Продолжительное движение с заданным шагом
void Fish::drag(int step) {
	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		// Обработка нажатия клавиш - выбор направления
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(200);
	}
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
void Fish::moveTo(int new_x, int new_y) {
	Hide();
	// Перемещение "центра" в новое место
	setX(new_x);
	setY(new_y);
	Show();
}

// Продолжительное движение с заданным шагом
void Fish::drag(int step) {
	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		// Обработка нажатия клавиш - выбор направления
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(200);
	}
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
	Ellipse(hdc,	//focusX = 80, focusY = 15
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
void MutantFish::moveTo(int new_x, int new_y) {
	Hide();
	// Перемещение "центра" в новое место
	setX(new_x);
	setY(new_y);
	Show();
}

// Продолжительное движение с заданным шагом
void MutantFish::drag(int step) {
	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		// Обработка нажатия клавиш - выбор направления
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(200);
	}
}

/**********************************************************
*				МЕТОДЫ КЛАССА CircleFish				  *
**********************************************************/

CircleFish::CircleFish(int new_x, int new_y) : Fish(new_x, new_y) {}
CircleFish::~CircleFish() {}

// Переопределение метода тела рыбы, так как форма не эллипс, а шар
void CircleFish::body(int clr1, int clr2, int clr3) {
	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //салатовый
	SelectObject(hdc, hBrush);
	Ellipse(hdc,
		x - BODY_FOCUS_X,
		y - BODY_FOCUS_X,
		x + BODY_FOCUS_X,
		y + BODY_FOCUS_X
	);
	DeleteObject(hBrush);
}
// Вследствие иного тела, положение плавников меняется
void CircleFish::bottom_fin(int clr1, int clr2, int clr3) {
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
void CircleFish::top_fin(int clr1, int clr2, int clr3) {
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

// Переопределение родительских методов Show и Hide
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

void CircleFish::moveTo(int new_x, int new_y) {
	Hide();
	// Перемещение "центра" в новое место
	setX(new_x);
	setY(new_y);
	Show();
}

// Продолжительное движение с заданным шагом
void CircleFish::drag(int step) {
	while (1) {
		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
		// Обработка нажатия клавиш - выбор направления
		if (KEY_DOWN(VK_ESCAPE)) break;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		Sleep(200);
	}
}

/****************************************************************
*						ЗАГОТОВКИ ДЛЯ ВСТАВОК					*
****************************************************************/


/****************************************************************
*						Drag с препятствием						*
****************************************************************/
//void Fish::drag(int step) {
//	cout << "Enter coordinates of an obstacle (x, y):\n";
//	int new_x, new_y;
//	cin >> new_x >> new_y;
//	Flag AFlag(new_x, new_y, 80, 30);
//	AFlag.Show();
//	while (1) {
//		Hide(); //Затирка привязана к текущим координатам, заранее стираем объект до изменения координаты
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
//		Sleep(200);//чтоб эпилепсии не было
//	}
//}

/****************************************************************
*					Реакционные изменения рыб					*
****************************************************************/

//void HatFish::hat_up() {
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); //салатовый
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc,	//focusX = 80, focusY = 15
//		x - 80,
//		y - BODY_FOCUS_Y - 15 - 20, //подъем на 20 пикселей
//		x + 80,
//		y - BODY_FOCUS_Y + 15 - 20
//	);
//	DeleteObject(hBrush);
//}

//void MutantFish::third_eye() {
//	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); //салатовый
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc,	//focusX = 80, focusY = 15
//		x + BODY_FOCUS_X / 2 - EYE_RADIUS + 30 - 10,
//		y - EYE_RADIUS - 20,
//		x + BODY_FOCUS_X / 2 + EYE_RADIUS + 30 - 10,
//		y + EYE_RADIUS - 20
//	);
//	DeleteObject(hBrush);
//}

//void CircleFish::slim_body() {
//	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //салатовый
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc,
//		x - BODY_FOCUS_X,
//		y - BODY_FOCUS_Y,
//		x + BODY_FOCUS_X,
//		y + BODY_FOCUS_Y
//	);
//	DeleteObject(hBrush);
//}
//
//void CircleFish::fat_body() {
//	HBRUSH hBrush = CreateSolidBrush(RGB(127, 255, 0)); //салатовый
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc,
//		x - BODY_FOCUS_X,
//		y - BODY_FOCUS_X - 16,
//		x + BODY_FOCUS_X,
//		y + BODY_FOCUS_X + 16
//	);
//	DeleteObject(hBrush);
//}

/****************************************************************
*		Проверка на столкновение и реакция на него всех рыб		*
****************************************************************/

//bool Fish::hasCollisionWith(Obstacle* obstacle) {
//	// Параметры препятствия
//	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
//		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
//	// Коориднаты лев.верх и правого нижнего угла грани рыбы
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;
//
//	if (
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		)
//		return true;
//	else
//		return false;
//}
//
//void Fish::react(Flag* flag) {}

//bool HatFish::hasCollisionWith(Obstacle* obstacle) {
//	// Параметры препятствия
//	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
//		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
//	// Коориднаты лев.верх и правого нижнего угла грани рыбы
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;
//
//	if (
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		)
//		return true;
//	else
//		return false;
//}
//
//void HatFish::react(Flag* flag) {
//	Hide();
//	rear_fin();
//	top_fin();
//	bottom_fin();
//	body();
//	eye();
//	mouth();
//	hat_up();
//}

//bool MutantFish::hasCollisionWith(Obstacle* obstacle) {
//	// Параметры препятствия
//	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
//		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
//	// Коориднаты лев.верх и правого нижнего угла грани рыбы
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;
//
//	if (
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		)
//		return true;
//	else
//		return false;
//}
//
//void MutantFish::react(Flag* flag) {
//	Hide();
//	rear_fin();
//	top_fin();
//	bottom_fin();
//	body();
//	eye();
//	second_eye();
//	third_eye();
//	mouth();
//}

//void CircleFish::react(Flag* flag) {
//	Hide();
//	rear_fin();
//	top_fin();
//	bottom_fin();
//	slim_body();
//	eye();
//	mouth();
//}
//
//bool CircleFish::hasCollisionWith(Obstacle* obstacle) {
//	// Параметры препятствия
//	int obstacle_x = obstacle->getX(), obstacle_y = obstacle->getY(),
//		size_x = obstacle->getsizeX(), size_y = obstacle->getsizeY();
//	// Коориднаты лев.верх и правого нижнего угла грани рыбы
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_X - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_X + 15 + abs(BOTTOM_FIN_BASE) / 2;
//
//	if (
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x - size_x / 2 >= x1) && (obstacle_x - size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y - size_y / 2 >= y1) && (obstacle_y - size_y / 2 <= y2)))
//		||
//		(((obstacle_x + size_x / 2 >= x1) && (obstacle_x + size_x / 2 <= x2)) &&
//			((obstacle_y + size_y / 2 >= y1) && (obstacle_y + size_y / 2 <= y2)))
//		)
//		return true;
//	else
//		return false;
//}

/****************************************************************
*					Методы препятствий							*
****************************************************************/

////defining Obstacle methods
//Obstacle::Obstacle(int new_x, int new_y, int new_szX, int new_szY) : Point(new_x, new_y) {
//	size_x = new_szX;
//	size_y = new_szY;
//}
//Obstacle::~Obstacle() {}
////void Obstacle::Show() {}
////void Obstacle::Hide() {}
//int  Obstacle::getsizeX() { return size_x; }
//int  Obstacle::getsizeY() { return size_y; }
//void Obstacle::setsizeX(int new_szX) { size_x = new_szX; }
//void Obstacle::setsizeY(int new_szY) { size_y = new_szY; }

////defining Flag methods
//Flag::Flag(int new_x, int new_y, int new_szX, int new_szY) : Obstacle(new_x, new_y, new_szX, new_szY) {
//
//}
//Flag::~Flag() {}
//void Flag::Show() {
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(0, 0, 0));
//	SelectObject(hdc, hPen);
//	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
//	DeleteObject(hPen);
//
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y - size_y / 6);
//	hBrush = CreateSolidBrush(RGB(0, 0, 255));
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x - size_x / 2, y - size_y / 6, x + size_x / 2, y + size_y / 6);
//	hBrush = CreateSolidBrush(RGB(255, 0, 0));
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x - size_x / 2, y + size_y / 6, x + size_x / 2, y + size_y / 2);
//	DeleteObject(hBrush);
//
//	visible = true;
//}
//void Flag::Hide() {
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
//	SelectObject(hdc, hPen);
//	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
//	DeleteObject(hPen);
//
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x - size_x / 2, y - size_y / 2, x + size_x / 2, y + size_y / 2);
//	DeleteObject(hBrush);
//
//	visible = false;
//}

// Disc methods
//Disc::Disc(int new_x, int new_y, int new_szX, int new_szY) : Obstacle(new_x, new_y, new_szX, new_szY) {
//
//}
//Disc::~Disc() {}
//void Disc::Show() {
//
//	//border of first circle
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH + 1, RGB(0, 0, 0)); SelectObject(hdc, hPen);
//	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
//
//	//inner part of first circle
//	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 139));
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
//	DeleteObject(hBrush);
//
//	//border of second circle
//	SelectObject(hdc, hPen);
//	Ellipse(hdc, x - size_x / 4, y - size_y / 4, x + size_y / 4, y + size_y / 4);
//
//	//inner part of second circle
//	hBrush = CreateSolidBrush(RGB(90, 90, 90));
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc, x - size_x / 4, y - size_y / 4, x + size_y / 4, y + size_y / 4);
//	DeleteObject(hBrush);
//
//	//border of third circle
//	SelectObject(hdc, hPen);
//	Ellipse(hdc, x - size_x / 8, y - size_y / 8, x + size_y / 8, y + size_y / 8);
//
//	//inner part of third circle
//	hBrush = CreateSolidBrush(RGB(0, 0, 255));
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc, x - size_x / 8, y - size_y / 8, x + size_y / 8, y + size_y / 8);
//	DeleteObject(hBrush);
//
//	DeleteObject(hPen);
//
//	visible = true;
//
//}
//void Disc::Hide() {
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH + 1, RGB(255, 255, 255));
//	SelectObject(hdc, hPen);
//	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
//	DeleteObject(hPen);
//
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
//	SelectObject(hdc, hBrush);
//	Ellipse(hdc, x - size_x / 2, y - size_y / 2, x + size_y / 2, y + size_y / 2);
//	DeleteObject(hBrush);
//
//	visible = false;
//}

/****************************************************************
*							Остальное							*
****************************************************************/
// Если вдруг надо стирать более простым образом
//void HatFish::Hide() {
//	int x1 = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2,
//		x2 = x + BODY_FOCUS_X,
//		y1 = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2,
//		y2 = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2; //основание отрицательное, надо модуль брать
//	HPEN hPen = CreatePen(PS_SOLID, PEN_WIDTH, RGB(255, 255, 255));
//	SelectObject(hdc, hPen);
//	Rectangle(hdc, x1, y1, x2, y2); //границу закрасить
//
//	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); //для заливки
//	SelectObject(hdc, hBrush);
//	Rectangle(hdc, x1, y1, x2, y2); //внутренность закрасить
//	DeleteObject(hBrush);
//	DeleteObject(hPen);
//}