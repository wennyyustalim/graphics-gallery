#pragma once
#include <applib>

class Circle {
	private:
		Point center_point;
		int radius;
	public:
		Circle();
		Circle(Point center_point, int radius);
		Circle(const Circle&);
		Point getCenterPoint(Circle circle);
		int getRadius(Circle circle);
};

// void gambarLingkaran(int a, int b, int r) {
// 	//persamaan lingkaran: (x-a)^2 + (y-b)^2 - r^2 = 0
// 	int hasil;
// 	for (int x = a-r; x <= a+r; x++) {
// 		for (int y = b-r; y <= b+r; y++) {
// 			hasil = ((x-a)*(x-a)) + ((y-b)*(y-b)) - (r*r);
// 			if (hasil < 0) {
// 				gambarPoint(x, y);
// 			}
// 		}
// 	}
// }