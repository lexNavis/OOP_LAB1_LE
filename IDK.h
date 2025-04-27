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

// ����������� �����, ����������� ����������� �� ������
class Obstacle : public Location {
protected:
	// ����������� ����� ������� � ������������� � ������ size_x � ������� size_y
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

// �����, ����������� ����������� - ����
class Flag : public Obstacle {
protected:
public:
	Flag(int new_x, int new_y, int new_szX, int new_szY);
	~Flag();
	// ��������������� ������ Hide � Show ��� �����
	virtual void Show();
	virtual void Hide();
};

// �����, ����������� ����������� - ����
class Disc : public Obstacle {
protected:
public:
	Disc(int new_x, int new_y, int new_szX, int new_szY);
	~Disc();
	// ��������������� ������ Hide � Show ��� �����
	virtual void Show();
	virtual void Hide();
};

class Brick : public Obstacle {
protected:
public:
	Brick(int new_x, int new_y, int new_szX, int new_szY);
	~Brick();
	// ��������������� ������ Hide � Show ��� �����
	virtual void Show();
	virtual void Hide();
};

// ����� ����������� ����� �� ������
class Point : public Location {
protected:
	bool visible; // �������� ��������� ����� �� ������
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
	virtual void react(Flag* flag);
	virtual void react(Disc* disc);
	virtual void react(Brick* brick);

};

// ������� �����, ����������� ����
class Fish : public Point {
protected:
	// ��������� ����
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
	// ������, ������������ �������� ����� ���� �� ������
	void body(int clr1, int clr2, int clr3);
	void rear_fin(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);
	void eye(int clr1, int clr2, int clr3);
	void mouth(int clr1, int clr2, int clr3);

	// ��������������� ������ Hide � Show ��� ������� ����
	virtual void Show();
	virtual void Hide();

	// ��� ����������� �������������
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
	virtual void react(Disc* disc);
	virtual void react(Brick* brick);
};

// ����������� �����, ����������� ���� � ������
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();

	// ���������� ���������� ������� - ����� � �������� �����
	void hat(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	
	// �������� ��������� ��� ������������
	// ��������, ��������� ����� ��� ������������ � ������ 
	// � ������ ����� �� ������ ��� ������������ � ������
};

// ����������� �����, ����������� ���� � ����� �������
class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();

	// ���������� ���������� ������� - ������� ����� � �������� �����
	void second_eye(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);

	// �������� ��������� ��� ������������ 
	// ��������, �������� ������ ���� ��� ������������ � ������ 
	// � ������ ���� ������ ��� ������������ � ������
};

// ����������� �����, ����������� ���� - ���
class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	// ���������� ���������� ������� - ��������, ����������� � ����������� ���, ����� ��������� ���������
	void square_body(int clr1, int clr2, int clr3);
	void bottom_fin(int clr1, int clr2, int clr3);
	void top_fin(int clr1, int clr2, int clr3);

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	

	// �������� ��������� ��� ������������
	// ��������, ������� � ������ ��� ������������ � ������ 
	// � ����� ������������ �������� ��� ������������ � ������
};


/**********************************************************
*					��������� �������					  *
**********************************************************/

void OperateWith(Fish* fish);
void demonstrate_collisions();
void ClearScreen();

Fish* setFish(Fish** fishes, int pos);
