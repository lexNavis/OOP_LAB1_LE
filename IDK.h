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

// ����� ����������� ����� �� ������
class Point : public Location {
protected:
	bool visible; // �������� ��������� ����� �� ������
public:
	Point(int new_x, int new_y);
	~Point();
	void setVisible(bool new_visible);
	bool isVisible();
	void moveTo(int new_x, int new_y);
	virtual void Show();
	virtual void Hide();
};

// ����������� �����, ����������� ����������� �� ������
class Obstacle : public Point {
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
	// ������ ����������� ������, ���������������� � ��������
	virtual void Show() = 0;
	virtual void Hide() = 0;
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

// ������� �����, ����������� ����
class Fish : public Point {
public:
	Fish(int new_x, int new_y);
	~Fish();
	// ������, ������������ �������� ����� ���� �� ������
	void body(); 
	void rear_fin();
	void bottom_fin();
	void top_fin();
	void eye();
	void mouth();

	// ��������������� ������ Hide � Show ��� ������� ����
	virtual void Show();
	virtual void Hide();

	// ��� ����������� �������������
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);

	void moveTo(int new_x, int new_y);
	void drag(int step);
};

// ����������� �����, ����������� ���� � ������
class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();
	
	// ���������� ���������� ������� - ����� � �������� �����
	void hat();
	void hat_up();

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
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
	void second_eye();
	void third_eye();

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
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
	void body();
	void slim_body();
	void fat_body();
	void bottom_fin();
	void top_fin();

	virtual void Show();
	virtual void Hide();

	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
	// �������� ��������� ��� ������������
	// ��������, ������� � ������ ��� ������������ � ������ 
	// � ����� ������������ �������� ��� ������������ � ������
};
