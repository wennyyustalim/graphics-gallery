#ifndef _CANVAS_MAP_
#define _CANVAS_MAP_
#include <applib>
class Canvas {
	private:
		std::vector<std::vector<RGB> > matrix;
		std::vector<Renderable *> renderables; 
	public:
		Canvas(std::vector<Renderable *> items);
		void fillMatrix();
		std::vector<std::vector<RGB>>& getMatrix();
		std::vector<Renderable *> getRenderables();
};

#endif