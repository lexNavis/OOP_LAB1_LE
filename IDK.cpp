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
	const int FISH_COUNT = 5;
	Fish		BFish(700, 300);
	HatFish		HFish(700, 300);
	MutantFish	MFish(700, 300);
	MutantFish1	M1Fish(700, 300);
	SquareFish	SFish(700, 300);
	CircleFish	CFish(700, 300);

	// Массив со всеми существующими рыбами 
	Fish* fishes[6][4] = {
		{ &BFish, &HFish, &M1Fish, &SFish },
		{ &BFish, &SFish, &MFish, &CFish },
		{ &BFish, &CFish, &HFish, &BFish },
		{ &BFish, &BFish, &CFish, &MFish },
		{ &BFish, &MFish, &BFish, &HFish },
		{ &BFish,& MFish,& BFish,& HFish }
	};



	// Указатель на конкретную рыбу 
	Fish* pFish = &BFish;
	
	// Переменная нужна для смены рыбы в зависимости от раздражителя
	int k = 0;

	while(1) {
		// Получение индекса столбца для выбора рыбы
		k = pFish->drag(PPM);
		// Выбираем рыбу из массива
		pFish = setFish(fishes, pFish->getID(), k);
	}

}

// Выбираем рыбу из массива рыб и присваиваем указателю
Fish* setFish(Fish* fishes[5][4], int row, int col) {
	return fishes[row][col];
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

Point::Point(int new_x, int new_y) : ABC_Object(new_x, new_y) { visible = false; }
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
// Проверка на столкновение с препятствием
bool Point::hasCollisionWith(ABC_Obstacle* obstacle) { return false; }


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
		// Обработка нажатия клавиш
		if (KEY_DOWN(VK_ESCAPE)) return 0;
		else if (KEY_DOWN(VK_LEFT))  x = x - step;
		else if (KEY_DOWN(VK_RIGHT)) x = x + step;
		else if (KEY_DOWN(VK_UP))    y = y - step;
		else if (KEY_DOWN(VK_DOWN))  y = y + step;
		moveTo(x, y);
		// Ветка столкновения с флагом
		if (hasCollisionWith(&AFlag)) {
			cout << "Collided with Flag!\n";
			Sleep(200);
			ClearScreen();
			// Вернем рыбу на свое место
			setX(700); setY(300);
			return AFlag.getID();
		}
		// Ветка столкновения с диском
		if (hasCollisionWith(&ADisc)) {
			cout << "Collided with Disc\n";
			Sleep(200);
			ClearScreen();
			setX(700); setY(300);
			return ADisc.getID();
		}
		// Ветка столкновения с кирпичиком
		if (hasCollisionWith(&ABrick)) {
			cout << "Collided with Brick\n";
			Sleep(200);
			ClearScreen();
			setX(700); setY(300);
			return ABrick.getID();
		}
		Sleep(100);
	}
}

/**********************************************************
*				МЕТОДЫ КЛАССА ABC_Object				  *
**********************************************************/

ABC_Object::ABC_Object(int new_x, int new_y) : Location(new_x, new_y) {}
ABC_Object::~ABC_Object() {}
//int ABC_Object::GetLeftTopX()		{ return left_top_x; }
//int ABC_Object::GetLeftTopY()		{ return left_top_y; }
//int ABC_Object::GetRightBottomX()	{ return right_bottom_x; }
//int ABC_Object::GetRightBottomY()	{ return right_bottom_y; }
int ABC_Object::getID()				{ return id; }

/**********************************************************
*				МЕТОДЫ КЛАССА Fish						  *
**********************************************************/

Fish::Fish(int new_x, int new_y) : Point(new_x, new_y) {
	id = 0;

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

	/*left_top_x = x - BODY_FOCUS_X - abs(REAR_FIN_HEIGHT) / 2;
	left_top_y = y - BODY_FOCUS_Y - 15 - abs(TOP_FIN_BASE) / 2;
	right_bottom_x = x + BODY_FOCUS_X;
	right_bottom_y = y + BODY_FOCUS_Y + 15 + abs(BOTTOM_FIN_BASE) / 2;*/
}
Fish::~Fish() {}


// Рисование тела рыбы
void Fish::body(int clr1 = 127, int clr2 = 255, int clr3 = 0) {	//салатовый
	// Белое перо для обводки
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

// Проверка на столкновение с препятствием
bool Fish::hasCollisionWith(ABC_Obstacle* obstacle) {
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

HatFish::HatFish(int new_x, int new_y) : Fish(new_x, new_y) {
	id = 1;
}
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
// Проверка на столкновение с препятствием
bool HatFish::hasCollisionWith(ABC_Obstacle* obstacle) {
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

MutantFish::MutantFish(int new_x, int new_y) : Fish(new_x, new_y) {
	id = 2;
}
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
// Проверка на столкновение с препятствием
bool MutantFish::hasCollisionWith(ABC_Obstacle* obstacle) {
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

MutantFish1::MutantFish1(int new_x, int new_y) : MutantFish(new_x, new_y) {
	id = 5;
}
MutantFish1::~MutantFish1() {}

// Рисование второго глаза
void MutantFish1::second_fin(int clr1, int clr2, int clr3) {
	POINT* points = new POINT[3];
	points[0] = { x - BOTTOM_FIN_HEIGHT / 2 - 50, y - BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[1] = { x - BOTTOM_FIN_HEIGHT / 2 - 30, y + BOTTOM_FIN_BASE / 2 + BODY_FOCUS_Y + 15 };
	points[2] = { x + BOTTOM_FIN_HEIGHT / 2 - 25, y + BODY_FOCUS_Y + 15 };

	HBRUSH hBrush = CreateSolidBrush(RGB(clr1, clr2, clr3)); //для заливки
	SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	DeleteObject(hBrush);
	delete[] points;
}

// Переопределение родительских методов Show и Hide
void MutantFish1::Show() {
	rear_fin(169, 169, 169);
	top_fin(169, 169, 169);
	bottom_fin(169, 169, 169);
	body(127, 255, 0);
	eye(0, 0, 0);
	// Отличие предка от потомка - наличие второго глаза
	second_eye(0, 0, 0);
	second_fin(169, 169, 169);
	mouth(169, 169, 169);

	visible = true;
}
void MutantFish1::Hide() {
	rear_fin(255, 255, 255);
	top_fin(255, 255, 255);
	bottom_fin(255, 255, 255);
	body(255, 255, 255);
	eye(255, 255, 255);
	second_eye(255, 255, 255);
	second_fin(255, 255, 255);
	mouth(255, 255, 255);

	visible = false;
}
// Проверка на столкновение с препятствием
bool MutantFish1::hasCollisionWith(ABC_Obstacle* obstacle) {
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

SquareFish::SquareFish(int new_x, int new_y) : Fish(new_x, new_y) {
	id = 3;
}
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

// Проверка на столкновение с препятствием
bool SquareFish::hasCollisionWith(ABC_Obstacle* obstacle) {
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
*				МЕТОДЫ КЛАССА CircleFish				  *
**********************************************************/

CircleFish::CircleFish(int new_x, int new_y) : Fish(new_x, new_y) {
	id = 4;
}
CircleFish::~CircleFish() {}

// Переопределение метода тела рыбы, так как форма не эллипс, а шар
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

bool CircleFish::hasCollisionWith(ABC_Obstacle* obstacle) {
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
*				МЕТОДЫ КЛАССА ABC_Obstacle				  *
**********************************************************/

ABC_Obstacle::ABC_Obstacle(int new_x, int new_y, int new_szX, int new_szY) : ABC_Object(new_x, new_y) {
	size_x = new_szX;
	size_y = new_szY;
}
ABC_Obstacle::~ABC_Obstacle() {}
int  ABC_Obstacle::getsizeX() { return size_x; }
int  ABC_Obstacle::getsizeY() { return size_y; }
void ABC_Obstacle::setsizeX(int new_szX) { size_x = new_szX; }
void ABC_Obstacle::setsizeY(int new_szY) { size_y = new_szY; }

/**********************************************************
*				МЕТОДЫ КЛАССА Flag   				      *
**********************************************************/

Flag::Flag(int new_x, int new_y, int new_szX, int new_szY) : ABC_Obstacle(new_x, new_y, new_szX, new_szY) {
	id = 1;
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

Disc::Disc(int new_x, int new_y, int new_szX, int new_szY) : ABC_Obstacle(new_x, new_y, new_szX, new_szY) {
	id = 2;
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

Brick::Brick(int new_x, int new_y, int new_szX, int new_szY) : ABC_Obstacle(new_x, new_y, new_szX, new_szY) {
	id = 3;
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


