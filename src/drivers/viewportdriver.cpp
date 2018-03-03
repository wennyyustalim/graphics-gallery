#include <applib>

ViewportDriver::ViewportDriver(int x, int y, Canvas* canvas, FramebufferDriver& fbDriver) {
	x_offset = x;
	y_offset = y;
	fb_driver = fbDriver;
    this->canvas = canvas;
    renderCanvas();
}
void ViewportDriver::moveLeft() {
	if(x_offset > 0) {
		x_offset--;
		renderCanvas();	
	}
}
void ViewportDriver::moveRight() {
	if(x_offset + VIEWPORT_WIDTH < CANVAS_WIDTH) {
		x_offset++;
		
		renderCanvas();
	}
}
void ViewportDriver::moveUp() {
	if(y_offset > 0) {
	y_offset--;
	renderCanvas();
	}
}
void ViewportDriver::moveDown() {
	if(y_offset + VIEWPORT_HEIGHT < CANVAS_HEIGHT)
	{	y_offset++;	
		renderCanvas();
	}
}

bool ViewportDriver::isXinViewport(int x) {
	return (x >= x_offset && x <= VIEWPORT_WIDTH+x_offset);
}

bool ViewportDriver::isYinViewport(int y) {
	return (y >= y_offset && y <= VIEWPORT_HEIGHT+y_offset);
}

bool ViewportDriver::isPointInViewPort(int x, int y) {
	return isXinViewport(x) && isYinViewport(y);
}

void ViewportDriver::renderCanvas() {
	// fb_driver.clearScreen();
	fprintf(stderr, "rendering canvas\n");
	std::vector<std::vector<RGB>> matrix = canvas->getMatrix();
	fprintf(stderr, "%d %d\n", matrix.size(), matrix[0].size());
	// for(int i=0;i<matrix.size();i++) {
	// 	for(int j=0; j<matrix[i].size();j++) {
	// 		fprintf(stderr, "%d %d:\n", i,j);
	// 		RGB rgb = matrix[i][j];
	// 	}
	// }
	for(int r = VIEWPORT_START_Y; r < VIEWPORT_HEIGHT+VIEWPORT_START_Y; r++) {
		for(int c = VIEWPORT_START_X; c < VIEWPORT_WIDTH+VIEWPORT_START_X;c++) {
			// fprintf(stderr, "rendering: %d %d %d %d\n",r,c,y_offset,x_offset);
			if(r+y_offset < CANVAS_HEIGHT && c+x_offset < CANVAS_WIDTH) {
				RGB curr = matrix[r+y_offset][c+x_offset];
			// fprintf(stderr, "rendering pixel\n");
				int re = curr.cR;
				int b = curr.cB;
				int g = curr.cG;
				fb_driver.printPixel(c,r,re,g,b);
			}
		}
	}
	
	//~ for(int r = VIEWPORT_START_Y; r < VIEWPORT_HEIGHT+VIEWPORT_START_Y; r++) {
			//~ for(int c = VIEWPORT_START_X; c < VIEWPORT_WIDTH+VIEWPORT_START_X;c++) {
			//~ for(Renderable* i: canvas->getRenderables()) {
				//~ int re = i->getRed();
				//~ int b = i->getBlue();
				//~ int g = i->getGreen();
				//~ bool f = i->getPixels()[r][c];
				//~ if(f) {
					//~ fb_driver.printPixel(r,c,re,g,b);

				//~ }
			//~ }
		//~ }
	//~ }
	// for(Renderable* i: canvas->getRenderables()) {
	// 	int r = i->getRed();
	// 	int b = i->getBlue();
	// 	int g = i->getGreen();
	// 	for(auto j : i->getPixels()) {
	// 		for(auto k : j.second) {
	// 			int x = j.first;
	// 			int y = k.first;
	// 			fprintf(stderr, "GetPixel in %d %d\n", x,y);
	// 				if(isPointInViewPort(j.first, k.first) && k.second) {
	// 					fb_driver.printPixel(x,y,r,g,b);
	// 				}
	// 		}
	// 	}
	// }

}

