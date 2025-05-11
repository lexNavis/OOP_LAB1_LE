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

// Интерфейс для иерархии рыб
class IFish {
public:
	// Чистая вирт. функция отрисовки тела рыбы
	virtual void body(int clr1, int clr2, int clr3) = 0;
	// Чистая вирт. функция отрисовки заднего плавника рыбы
	virtual void rear_fin(int clr1, int clr2, int clr3) = 0;
	// Чистая вирт. функция отрисовки нижнего плавника рыбы
	virtual void bottom_fin(int clr1, int clr2, int clr3) = 0;
	// Чистая вирт. функция отрисовки верхнего плавника рыбы
	virtual void top_fin(int clr1, int clr2, int clr3) = 0;
	// Чистая вирт. функция отрисовки глаза рыбы
	virtual void eye(int clr1, int clr2, int clr3) = 0;
	// Чистая вирт. функция отрисовки рта рыбы
	virtual void mouth(int clr1, int clr2, int clr3) = 0;
};

// Абстрактный базовый класс - объект
class ABC_Object : public Location {
protected:
	// Идентификатор объекта
	int id;
public:
	ABC_Object(int new_x, int new_y);
	~ABC_Object();
	int getID();
	// Чистая вирт. функция - показать объект
	virtual void Show() = 0;
	// Чистая вирт. функция - убрать объект
	virtual void Hide() = 0;
};

// Абстрактный класс, описывающий препятствие на экране
class ABC_Obstacle : public ABC_Object {
protected:
	// Препятствия будут вписаны в прямоугольник с длиной size_x и шириной size_y
	int size_x;
	int size_y;
	
public:
	ABC_Obstacle(int new_x, int new_y, int new_szX, int new_szY);
	~ABC_Obstacle();
	int  getsizeX();
	void setsizeX(int new_szX);
	int  getsizeY();
	void setsizeY(int new_szY);
	// Чистая вирутальная функция для отрисовки препятствия
	virtual void Show() = 0;
	// Чистая вирутальная функция для сокрытия препятствия
	virtual void Hide() = 0;
};

// Класс, описывающий препятствие - флаг
class Flag : public ABC_Obstacle {
protected:
public:
	Flag(int new_x, int new_y, int new_szX, int new_szY);
	~Flag();
	// Переопределеные методы Hide и Show для флага
	virtual void Show();
	virtual void Hide();
};

// Класс, описывающий препятствие - диск
class Disc : public ABC_Obstacle {
protected:
public:
	Disc(int new_x, int new_y, int new_szX, int new_szY);
	~Disc();
	// Переопределеные методы Hide и Show для диска
	virtual void Show();
	virtual void Hide();
};

class Brick : public ABC_Obstacle {
protected:
public:
	Brick(int new_x, int new_y, int new_szX, int new_szY);
	~Brick();
	// Переопределеные методы Hide и Show для кирпича
	virtual void Show();
	virtual void Hide();
};

// Класс описывающий точку на экране
class Point : public ABC_Object {
protected:
	bool visible; // параметр видимости точки на экране
public:
	Point(int new_x, int new_y);
	~Point();
	void setVisible(bool new_visible);
	bool isVisible();

	// Отображение точки
	virtual void Show();
	// Сокрытие точки
	virtual void Hide();
	// Перемещение точки
	void moveTo(int new_x, int new_y);
	int drag(int step);
	// Проверка на столкновение с препятствием
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
};




// Базовый класс, описывающий рыбу
class Fish : public Point, public IFish {
protected:
	// Параметры рыбы
	int BODY_FOCUS_X;
	// Ширина тела по У
	int BODY_FOCUS_Y;
	// Высота треугольника, задющего задний плавник
	int REAR_FIN_HEIGHT;
	// Основание треугольника, задющего задний плавник
	int REAR_FIN_BASE;
	// Высота треугольника, задющего верхний плавник
	int TOP_FIN_HEIGHT;
	// Основание треугольника, задющего верхний плавник
	int TOP_FIN_BASE;
	// Высота треугольника, задющего нижний плавник
	int BOTTOM_FIN_HEIGHT;
	// Описание треугольника, задющего нижний плавник
	int BOTTOM_FIN_BASE;
	// Радиус глаза
	int EYE_RADIUS;
	// Высота треугольника, задающего рот
	int MOUTH_HEIGHT;
	// Основание треугольника, задающего рот
	int MOUTH_BASE;
	// Идентификатор рыбы
	
public:
	Fish(int new_x, int new_y);
	~Fish();
	// Принудительное переопределение чистых виртуальных функций
	// Отрисовка тела рыбы
	void body(int clr1, int clr2, int clr3);
	// Отрисовка заднего плавника рыбы
	void rear_fin(int clr1, int clr2, int clr3);
	// Отрисовка нижнего плавника рыбы
	void bottom_fin(int clr1, int clr2, int clr3);
	// Отрисовка верхнего плавника рыбы
	void top_fin(int clr1, int clr2, int clr3);
	// Отрисовка глаза рыбы
	void eye(int clr1, int clr2, int clr3);
	// Отрисовка рта рыбы
	void mouth(int clr1, int clr2, int clr3);

	// Переопределеные методы Show для базовой рыбы
	virtual void Show();
	// Переопределеные методы Hide для базовой рыбы
	virtual void Hide();

	// Для дальнейшего использования
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};

// Производный класс, описывающий рыбу с шляпой
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();

	// Объявление уникальных методов - шляпы
	void hat(int clr1, int clr2, int clr3);

	// Переопределеные методы Show для базовой рыбы
	virtual void Show();
	// Переопределеные методы Hide для базовой рыбы
	virtual void Hide();
	// Проверка на столкновение с препятствием
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};

// Производный класс, описывающий рыбу с двумя глазами
class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();

	// Объявление уникальных методов - второго глаза
	void second_eye(int clr1, int clr2, int clr3);

	// Переопределеные методы Show для базовой рыбы
	virtual void Show();
	// Переопределеные методы Hide для базовой рыбы
	virtual void Hide();

	// Проверка на столкновение с препятствием
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);

};

// Производный класс, описывающий рыбу - квадрат
class SquareFish : public Fish {
public:
	SquareFish(int new_x, int new_y);
	~SquareFish();
	// Рисование квадратного тела
	void square_body(int clr1, int clr2, int clr3);
	// Переопределение рисования нижнего плавника
	void bottom_fin(int clr1, int clr2, int clr3);
	// Переопределение рисования верхнего плавника
	void top_fin(int clr1, int clr2, int clr3);
	// Переопределеные метод Show 
	virtual void Show();
	// Переопределеные метод Hide
	virtual void Hide();
	// Проверка на столкновение с препятствием
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
};

// Производный класс, описывающий рыбу - шар
class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	// Объявление уникальных методов - круглого, утонченного и утолщенного тел, иного положения плавников
	void body(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};




/**********************************************************
*					ПРОТОТИПЫ ФУНКЦИЙ					  *
**********************************************************/
// Работа с рыбой через указатель на базовый класс
void OperateWith(Fish* fish);
// Демонстрация столкновений
void demonstrate_collisions();
// Очистка экрана
void ClearScreen();
// Выбор рыбы из массива
Fish* setFish(Fish* fishes[5][4], int row, int col);
