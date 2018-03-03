#include <applib>

ViewportDriver::ViewportDriver(int x, int y, Canvas* canvas) {
	x_offset = x;
	y_offset = y;
    fb_driver.init();
    fb_driver.clearScreen();
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
	return (y >= y_offset && y <= VIEWPORT_WIDTH+y_offset);
}

bool ViewportDriver::isPointInViewPort(int x, int y) {
	return isXinViewport(x) && isYinViewport(y);
}

void ViewportDriver::renderCanvas() {
	fb_driver.clearScreen();
	// for(int r = VIEWPORT_START_Y; r < VIEWPORT_HEIGHT+VIEWPORT_START_Y; r++) {
	// 	for(int c = VIEWPORT_START_X; c < VIEWPORT_WIDTH+VIEWPORT_START_X;c++) {
	// 		for(Renderable* i: canvas->getRenderables()) {
	// 			int re = i->getRed();
	// 			int b = i->getBlue();
	// 			int g = i->getGreen();
	// 			bool f = i->getPixels()[r][c];
	// 			if(f) {
	// 				fb_driver.printPixel(r,c,re,g,b);

	// 			}
	 for(Renderable* i: canvas->getRenderables()) {
	 	int r = i->getRed();
	 	int b = i->getBlue();
	 	int g = i->getGreen();
		for(auto j : i->getPixels()) {
	 		for(auto k : j.second) {
	 			int x = j.first;
	 			int y = k.first;
	 			//~ fprintf(stderr, "GetPixel in %d %d\n", x,y);
	 				if(isPointInViewPort(j.first, k.first) && k.second) {
	 					fb_driver.printPixel(x-x_offset,y-y_offset,r,g,b);
	 				}
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

