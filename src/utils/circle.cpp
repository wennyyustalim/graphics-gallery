#include "circle.h"

Circle::Circle() {
	center_point = Point();
	radius = 0;
}
Circle::Circle(Point center, int radius) {
	center_point = center;
	radius = radius;
}
Circle::Circle(const Circle& circle) {
	center_point = circle.center_point;
	radius = circle.radius;
}

Point Circle::getCenterPoint(Circle circle) {
	return center_point;

}
int Circle::getRadius(Circle circle) {
	return radius;
}