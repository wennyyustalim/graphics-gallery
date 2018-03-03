#include<applib>

Canvas::Canvas(std::vector<Renderable *> items) {
	renderables = items;
	fillMatrix();
}

std::vector<Renderable *> Canvas::getRenderables() {
	return renderables;
}

void Canvas::fillMatrix() {
	for(int r=0;r<CANVAS_HEIGHT;r++) {
		std::vector<RGB> rVect;
		for(int c=0; c<CANVAS_WIDTH;c++) {
			RGB x;
			x.cR = 255;
			x.cG = 255;
			x.cB = 255;
			rVect.push_back(x);
		}
		matrix.push_back(rVect);
	}
	for(Renderable* i: renderables) {
	 	int r = i->getRed();
	 	int b = i->getBlue();
	 	int g = i->getGreen();
		for(auto j : i->getPixels()) {
	 		for(auto k : j.second) {
	 			int x = j.first;
	 			int y = k.first;
	 			//~ fprintf(stderr, "GetPixel in %d %d\n", x,y);
	 			if(k.second) {
	 				matrix[y][x].cR = r;
	 				matrix[y][x].cG = g;
	 				matrix[y][x].cB = b;
	 			}
	 		}
	 	}
	 }
}

std::vector<std::vector<RGB>>& Canvas::getMatrix() {
	return matrix;
}