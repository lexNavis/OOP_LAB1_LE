#pragma once
//coordinates (x,y); center of a figure
class Location {	
protected:
	int x, y;

public:
	Location(int new_x, int new_y);
	~Location();
	int  getX();
	void setX(int new_x);
	int  getY();
	void setY(int new_y);
};

class Fish : public Location {
public:
	Fish(int new_x, int new_y);
	~Fish();
	void body(); 
	void rear_fin();
	void bottom_fin();
	void top_fin();
	void eye();
	void mouth();
	virtual void Show();
	virtual void Hide();
	void moveTo(int new_x, int new_y);
	void drag(int step);
};

class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();
	void hat();
	virtual void Show();
	virtual void Hide();
	// �������� ��������� ��� ������������
	// ��������, ��������� ����� ��� ������������ � ������ 
	// � ������ ����� �� ������ ��� ������������ � ��������
};

class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();
	void second_eye();
	virtual void Show();
	virtual void Hide();
	// �������� ��������� ��� ������������ 
	// ��������, ������ ����� ��� ������������ � ������ 
	// � �������� ������� ��� ������������ � ��������
};

class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	void body();
	void bottom_fin();
	void top_fin();
	virtual void Show();
	virtual void Hide();
	// �������� ��������� ��� ������������
	// ��������, ������� � ������ ��� ������������ � ������ 
	// � ����� ������������ �������� ��� ������������ � ��������
};