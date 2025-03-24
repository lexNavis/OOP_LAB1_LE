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
	/*virtual void Show();
	virtual void Hide();
	virtual void moveTo(int new_x, int new_y);
	void drag(int step);*/
};