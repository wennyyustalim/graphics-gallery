#ifndef _POINT_
#define _POINT_

class Point {
	private:
		int x;
		int y;
	public:
		Point();
		Point(int x, int y);
		Point (const Point&);
		int getX();
		int getY();
		void setX(int x);
		void setY(int y);
};

#endif