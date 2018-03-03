#include "point.h"

Point::Point() {
	setX(0);
	setY(0);
}

Point::Point(int x, int y) {
	setX(x);
	setY(y);
}

Point::Point (const Point& point) {
	setX(point.x);
	setY(point.y);
}
int Point::getX() {
	return x;
}
int Point::getY() {
	return y;
}
void Point::setX(int _x) {
	x = _x;
}
void Point::setY(int _y) {
	y = _y;
}