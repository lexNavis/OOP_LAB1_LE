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

// Класс описывающий точку на экране
class Point : public Location {
protected:
	bool visible; // параметр видимости точки на экране
public:
	Point(int new_x, int new_y);
	~Point();
	void setVisible(bool new_visible);
	bool isVisible();

	void Show();
	void Hide();

	void moveTo(int new_x, int new_y);
	void drag(int step);

};

//// Абстрактный класс, описывающий препятствие на экране
//class Obstacle : public Point {
//protected:
//	// Препятствия будут вписаны в прямоугольник с длиной size_x и шириной size_y
//	int size_x; 
//	int size_y;	
//public:
//	Obstacle(int new_x, int new_y, int new_szX, int new_szY);
//	~Obstacle();
//	int  getsizeX();
//	void setsizeX(int new_szX);
//	int  getsizeY();
//	void setsizeY(int new_szY);
//	// Чистые виртуальные методы, переопределенные в потомках
//	virtual void Show() = 0;
//	virtual void Hide() = 0;
//};
//
//// Класс, описывающий препятствие - флаг
//class Flag : public Obstacle {
//protected:
//public:
//	Flag(int new_x, int new_y, int new_szX, int new_szY);
//	~Flag();
//	// Переопределеные методы Hide и Show для флага
//	virtual void Show();
//	virtual void Hide();
//};
//// Класс, описывающий препятствие - диск
//class Disc : public Obstacle {
//protected:
//public:
//	Disc(int new_x, int new_y, int new_szX, int new_szY);
//	~Disc();
//	// Переопределеные методы Hide и Show для диска
//	virtual void Show();
//	virtual void Hide();
//};

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
	void Show();
	void Hide();
	void moveTo(int new_x, int new_y);
	void drag(int step);
	// Для дальнейшего использования
	//virtual bool hasCollisionWith(Obstacle* obstacle);
	//virtual void react(Flag* flag);
};

// Производный класс, описывающий рыбу с шляпой
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();

	// Объявление уникальных методов - шляпы и поднятой шляпы
	void hat(int clr1, int clr2, int clr3);
	//void hat_up();

	void Show();
	void Hide();
	void moveTo(int new_x, int new_y);
	void drag(int step);
	//virtual bool hasCollisionWith(Obstacle* obstacle);
	//virtual void react(Flag* flag);
	// Добавить поведение при столкновении
	// Например, поднимать шляпу при столкновении с флагом 
	// и менять шляпу на корону при столкновении с диском
};

// Производный класс, описывающий рыбу с двумя глазами
class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();

	// Объявление уникальных методов - второго глаза и третьего глаза
	void second_eye(int clr1, int clr2, int clr3);
	//void third_eye();

	void Show();
	void Hide();
	void moveTo(int new_x, int new_y);
	void drag(int step);
	//virtual bool hasCollisionWith(Obstacle* obstacle);
	//virtual void react(Flag* flag);
	// Добавить поведение при столкновении 
	// Например, добавить третий глаз при столкновении с флагом 
	// и лишить глаз вообще при столкновении с диском
};

// Производный класс, описывающий рыбу - шар
class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	// Объявление уникальных методов - круглого, утонченного и утолщенного тел, иного положения плавников
	void body(int clr1, int clr2, int clr3);
	//void slim_body();
	//void fat_body();
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);

	void Show();
	void Hide();
	void moveTo(int new_x, int new_y);
	void drag(int step);
	//virtual bool hasCollisionWith(Obstacle* obstacle);
	//virtual void react(Flag* flag);

	// Добавить поведение при столкновении
	// Например, сдуться в эллипс при столкновении с флагом 
	// и стать вертикальным эллипсом при столкновении с диском
};


/**********************************************************
*					ПРОТОТИПЫ ФУНКЦИЙ					  *
**********************************************************/

//void OperateWith(Fish* fish);
void ClearScreen();
