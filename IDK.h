/**************************************************************
*                      ����������������                       *
* ----------------------------------------------------------  *
* Project Type: Win64 Console Application                     *
* Project Name: OOP_LAB1_LE                                   *
* File Name: IDK.h					                          *
* Programmers:���� �.�., ������� 4, ������ 209�               *
* Modified -                                                  *
* Created: 23.03.25                                           *
* Last Revision: 12.04.24                                     *
* Comment: ���������� �������						          *
***************************************************************/

#pragma once

/**********************************************************
*					���������� �������					  *
**********************************************************/

// ����� ����������� ���� ��������� � � �
class Location {
protected:		// ����������� ������� ��� ������� ������������� ���������
	int x, y;

public:
	Location(int new_x, int new_y);
	~Location();
	int  getX();
	void setX(int new_x);
	int  getY();
	void setY(int new_y);
};

// ��������� ��� �������� ���
class IFish {
public:
	// ������ ����. ������� ��������� ���� ����
	virtual void body(int clr1, int clr2, int clr3) = 0;
	// ������ ����. ������� ��������� ������� �������� ����
	virtual void rear_fin(int clr1, int clr2, int clr3) = 0;
	// ������ ����. ������� ��������� ������� �������� ����
	virtual void bottom_fin(int clr1, int clr2, int clr3) = 0;
	// ������ ����. ������� ��������� �������� �������� ����
	virtual void top_fin(int clr1, int clr2, int clr3) = 0;
	// ������ ����. ������� ��������� ����� ����
	virtual void eye(int clr1, int clr2, int clr3) = 0;
	// ������ ����. ������� ��������� ��� ����
	virtual void mouth(int clr1, int clr2, int clr3) = 0;
};

// ����������� ������� ����� - ������
class ABC_Object : public Location {
protected:
	// ������������� �������
	int id;
public:
	ABC_Object(int new_x, int new_y);
	~ABC_Object();
	int getID();
	// ������ ����. ������� - �������� ������
	virtual void Show() = 0;
	// ������ ����. ������� - ������ ������
	virtual void Hide() = 0;
};

// ����������� �����, ����������� ����������� �� ������
class ABC_Obstacle : public ABC_Object {
protected:
	// ����������� ����� ������� � ������������� � ������ size_x � ������� size_y
	int size_x;
	int size_y;
	
public:
	ABC_Obstacle(int new_x, int new_y, int new_szX, int new_szY);
	~ABC_Obstacle();
	int  getsizeX();
	void setsizeX(int new_szX);
	int  getsizeY();
	void setsizeY(int new_szY);
	// ������ ����������� ������� ��� ��������� �����������
	virtual void Show() = 0;
	// ������ ����������� ������� ��� �������� �����������
	virtual void Hide() = 0;
};

// �����, ����������� ����������� - ����
class Flag : public ABC_Obstacle {
protected:
public:
	Flag(int new_x, int new_y, int new_szX, int new_szY);
	~Flag();
	// ��������������� ������ Hide � Show ��� �����
	virtual void Show();
	virtual void Hide();
};

// �����, ����������� ����������� - ����
class Disc : public ABC_Obstacle {
protected:
public:
	Disc(int new_x, int new_y, int new_szX, int new_szY);
	~Disc();
	// ��������������� ������ Hide � Show ��� �����
	virtual void Show();
	virtual void Hide();
};

class Brick : public ABC_Obstacle {
protected:
public:
	Brick(int new_x, int new_y, int new_szX, int new_szY);
	~Brick();
	// ��������������� ������ Hide � Show ��� �������
	virtual void Show();
	virtual void Hide();
};

// ����� ����������� ����� �� ������
class Point : public ABC_Object {
protected:
	bool visible; // �������� ��������� ����� �� ������
public:
	Point(int new_x, int new_y);
	~Point();
	void setVisible(bool new_visible);
	bool isVisible();

	// ����������� �����
	virtual void Show();
	// �������� �����
	virtual void Hide();
	// ����������� �����
	void moveTo(int new_x, int new_y);
	int drag(int step);
	// �������� �� ������������ � ������������
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
};




// ������� �����, ����������� ����
class Fish : public Point, public IFish {
protected:
	// ��������� ����
	int BODY_FOCUS_X;
	// ������ ���� �� �
	int BODY_FOCUS_Y;
	// ������ ������������, �������� ������ �������
	int REAR_FIN_HEIGHT;
	// ��������� ������������, �������� ������ �������
	int REAR_FIN_BASE;
	// ������ ������������, �������� ������� �������
	int TOP_FIN_HEIGHT;
	// ��������� ������������, �������� ������� �������
	int TOP_FIN_BASE;
	// ������ ������������, �������� ������ �������
	int BOTTOM_FIN_HEIGHT;
	// �������� ������������, �������� ������ �������
	int BOTTOM_FIN_BASE;
	// ������ �����
	int EYE_RADIUS;
	// ������ ������������, ��������� ���
	int MOUTH_HEIGHT;
	// ��������� ������������, ��������� ���
	int MOUTH_BASE;
	// ������������� ����
	
public:
	Fish(int new_x, int new_y);
	~Fish();
	// �������������� ��������������� ������ ����������� �������
	// ��������� ���� ����
	void body(int clr1, int clr2, int clr3);
	// ��������� ������� �������� ����
	void rear_fin(int clr1, int clr2, int clr3);
	// ��������� ������� �������� ����
	void bottom_fin(int clr1, int clr2, int clr3);
	// ��������� �������� �������� ����
	void top_fin(int clr1, int clr2, int clr3);
	// ��������� ����� ����
	void eye(int clr1, int clr2, int clr3);
	// ��������� ��� ����
	void mouth(int clr1, int clr2, int clr3);

	// ��������������� ������ Show ��� ������� ����
	virtual void Show();
	// ��������������� ������ Hide ��� ������� ����
	virtual void Hide();

	// ��� ����������� �������������
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};

// ����������� �����, ����������� ���� � ������
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();

	// ���������� ���������� ������� - �����
	void hat(int clr1, int clr2, int clr3);

	// ��������������� ������ Show ��� ������� ����
	virtual void Show();
	// ��������������� ������ Hide ��� ������� ����
	virtual void Hide();
	// �������� �� ������������ � ������������
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};

// ����������� �����, ����������� ���� � ����� �������
class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();

	// ���������� ���������� ������� - ������� �����
	void second_eye(int clr1, int clr2, int clr3);

	// ��������������� ������ Show ��� ������� ����
	virtual void Show();
	// ��������������� ������ Hide ��� ������� ����
	virtual void Hide();

	// �������� �� ������������ � ������������
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);

};

// ����������� �����, ����������� ���� - �������
class SquareFish : public Fish {
public:
	SquareFish(int new_x, int new_y);
	~SquareFish();
	// ��������� ����������� ����
	void square_body(int clr1, int clr2, int clr3);
	// ��������������� ��������� ������� ��������
	void bottom_fin(int clr1, int clr2, int clr3);
	// ��������������� ��������� �������� ��������
	void top_fin(int clr1, int clr2, int clr3);
	// ��������������� ����� Show 
	virtual void Show();
	// ��������������� ����� Hide
	virtual void Hide();
	// �������� �� ������������ � ������������
	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
};

// ����������� �����, ����������� ���� - ���
class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	// ���������� ���������� ������� - ��������, ����������� � ����������� ���, ����� ��������� ���������
	void body(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(ABC_Obstacle* obstacle);
	
};




/**********************************************************
*					��������� �������					  *
**********************************************************/
// ������ � ����� ����� ��������� �� ������� �����
void OperateWith(Fish* fish);
// ������������ ������������
void demonstrate_collisions();
// ������� ������
void ClearScreen();
// ����� ���� �� �������
Fish* setFish(Fish* fishes[5][4], int row, int col);
