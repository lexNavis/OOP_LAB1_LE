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


//abstract obstacle class (ith rectangular border)
class Obstacle : public Location {
protected:
	int size_x; // len
	int size_y;	// width
public:
	Obstacle(int new_x, int new_y, int new_szX, int new_szY);
	~Obstacle();
	//will be overwritten in child class
	int  getsizeX();
	void setsizeX(int new_szX);
	int  getsizeY();
	void setsizeY(int new_szY);
	virtual void Show() = 0;
	virtual void Hide() = 0;
};

class Flag : public Obstacle {
protected:
public:
	Flag(int new_x, int new_y, int new_szX, int new_szY);
	~Flag();
	virtual void Show();
	virtual void Hide();
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
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag) = 0;
	void moveTo(int new_x, int new_y);
	void drag(int step);
};

class HatFish : public Fish {
public:
	HatFish(int new_x, int new_y);
	~HatFish();
	void hat();
	void hat_up();
	virtual void Show();
	virtual void Hide();
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
	// Добавить поведение при столкновении
	// Например, поднимать шляпу при столкновении с флагом 
	// и менять шляпу на корону при столкновении с денежкой
};

class MutantFish : public Fish {
public:
	MutantFish(int new_x, int new_y);
	~MutantFish();
	void second_eye();
	void third_eye();
	virtual void Show();
	virtual void Hide();
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
	// Добавить поведение при столкновении 
	// Например, добавить третий глаз при столкновении с флагом 
	// и добавить плавник при столкновении с денежкой
};

class CircleFish : public Fish {
public:
	CircleFish(int new_x, int new_y);
	~CircleFish();
	void body();
	void slim_body();
	void fat_body();
	void bottom_fin();
	void top_fin();
	virtual void Show();
	virtual void Hide();
	virtual bool hasCollisionWith(Obstacle* obstacle);
	virtual void react(Flag* flag);
	// Добавить поведение при столкновении
	// Например, сдуться в эллипс при столкновении с флагом 
	// и стать вертикальным эллипсом при столкновении с денежкой
};
