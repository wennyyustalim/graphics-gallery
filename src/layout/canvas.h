#ifndef _CANVAS_MAP_
#define _CANVAS_MAP_
#include <applib>
class Canvas {
	private:
		std::vector<Renderable *> renderables; 
	public:
		Canvas(std::vector<Renderable *> items);
		std::vector<Renderable *> getRenderables();
};

#endif