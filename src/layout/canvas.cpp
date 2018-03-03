#include<applib>
Canvas::Canvas(std::vector<Renderable *> items) {
	renderables = items;
}

std::vector<Renderable *> Canvas::getRenderables() {
	return renderables;
}