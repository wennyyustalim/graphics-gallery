#include "line.h"

Line::Line() {
	begin_point = Point();
	end_point = Point();
}
Line::Line(Point begin, Point end) {
	begin_point = begin;
	end_point = end;
}
Line::Line(const Line& line) {
	begin_point = line.begin_point;
	end_point = line.end_point;
}

Point Line::getBeginPoint(Line line) {
	return begin_point;

}
Point Line::getEndPoint(Line line) {
	return end_point;
}