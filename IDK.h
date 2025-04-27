/**************************************************************
*                      Программирование                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: IDK.h					                          *
* Programmers:Шеин Д.К., Бригада 4, Группа 209Б               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: Объявление классов						          *
***************************************************************/

#pragma once

/**********************************************************
*					ОБЪЯВЛЕНИЕ КЛАССОВ					  *
**********************************************************/

// Класс описывающий пару координат х и у
class Location {
protected:		// модификатор доступа для прямого использования потомками
	int x, y;

public:
	Location(int new_x, int new_y);
	~Location();
	int  getX();
	void setX(int new_x);
	int  getY();
	void setY(int new_y);
};

// Класс, описывающий препятствие на экране
class Obstacle : public Location {
protected:
	// Препятствия будут вписаны в прямоугольник с длиной size_x и шириной size_y
	int size_x;
	int size_y;
public:
	Obstacle(int new_x, int new_y, int new_szX, int new_szY);
	~Obstacle();
	int  getsizeX();
	void setsizeX(int new_szX);
	int  getsizeY();
	void setsizeY(int new_szY);

	virtual void Show();
	virtual void Hide();
};

// Класс, описывающий препятствие - флаг
class Flag : public Obstacle {
protected:
public:
	Flag(int new_x, int new_y, int new_szX, int new_szY);
	~Flag();
	// Переопределеные методы Hide и Show для флага
	virtual void Show();
	virtual void Hide();
};

// Класс, описывающий препятствие - диск
class Disc : public Obstacle {
protected:
public:
	Disc(int new_x, int new_y, int new_szX, int new_szY);
	~Disc();
	// Переопределеные методы Hide и Show для диска
	virtual void Show();
	virtual void Hide();
};

class Brick : public Obstacle {
protected:
public:
	Brick(int new_x, int new_y, int new_szX, int new_szY);
	~Brick();
	// Переопределеные методы Hide и Show для диска
	virtual void Show();
	virtual void Hide();
};

// Класс описывающий точку на экране
class Point : public Location {
protected:
	bool visible; // параметр видимости точки на экране
public:
	Point(int new_x, int new_y);
	~Point();
	void setVisible(bool new_visible);
	bool isVisible();

	virtual void Show();
	virtual void Hide();

	void moveTo(int new_x, int new_y);
	int drag(int step);

	virtual bool hasCollisionWith(Obstacle* obstacle);

};

// Базовый класс, описывающий рыбу
class Fish : public Point {
protected:
	// Параметры рыбы
	int BODY_FOCUS_X;
	int BODY_FOCUS_Y;
	int REAR_FIN_HEIGHT;
	int REAR_FIN_BASE;
	int TOP_FIN_HEIGHT;
	int TOP_FIN_BASE;
	int BOTTOM_FIN_HEIGHT;
	int BOTTOM_FIN_BASE;
	int EYE_RADIUS;
	int MOUTH_HEIGHT;
	int MOUTH_BASE;
public:
	Fish(int new_x, int new_y);
	~Fish();
	// Методы, отображающие основные части рыбы на экране
	void body(int clr1, int clr2, int clr3);
	void rear_fin(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);
	void eye(int clr1, int clr2, int clr3);
	void mouth(int clr1, int clr2, int clr3);

	// Переопределеные методы Hide и Show для базовой рыбы
	virtual void Show();
	virtual void Hide();

	// Для дальнейшего использования
	virtual bool hasCollisionWith(Obstacle* obstacle);
	
};

// Производный класс, описывающий рыбу с шляпой
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();

	// Объявление уникальных методов - шляпы и поднятой шляпы
	void hat(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	
};

// Производный класс, описывающий рыбу с двумя глазами
class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();

	// Объявление уникальных методов - второго глаза
	void second_eye(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);

};

// Производный класс, описывающий рыбу - квадрат
class SquareFish : public Fish {
public:
	SquareFish(int new_x, int new_y);
	~SquareFish();
	// Объявление уникальных методов -  иных плавников и квадратного тела
	void square_body(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
};


/**********************************************************
*					ПРОТОТИПЫ ФУНКЦИЙ					  *
**********************************************************/

void OperateWith(Fish* fish);
void demonstrate_collisions();
void ClearScreen();
Fish* setFish(Fish** fishes, int pos);
