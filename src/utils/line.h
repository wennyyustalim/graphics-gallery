#ifndef _LINE_
#define _LINE_
#include <applib>

class Line {
	private:
		Point begin_point;
		Point end_point;
	public:
		Line();
		Line(Point begin, Point end);
		Line(const Line&);
		Point getBeginPoint(Line line);
		Point getEndPoint(Line line);
};

#endif