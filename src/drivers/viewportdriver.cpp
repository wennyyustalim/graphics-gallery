#include <applib>

ViewportDriver::ViewportDriver(int x, int y, Canvas* canvas, FramebufferDriver& fbDriver) {
	x_offset = x;
	y_offset = y;
	fb_driver = fbDriver;
    this->canvas = canvas;
    bangunan_on = true;
    jalan_on = true;
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
				if(re==20&&g==20&&b==255) {
					if(bangunan_on) {
						fb_driver.printPixel(c,r,re,g,b);
					}	else {
						fb_driver.printPixel(c,r,255,255,255);
					}
				} else if(re==20&&g==20&&b==20) {
					if(jalan_on) {
						fb_driver.printPixel(c,r,re,g,b);
					}	else {
						fb_driver.printPixel(c,r,255,255,255);
					}
				}
				
			}
		}
	}
	renderOptions();
}

void ViewportDriver::renderOptions() {
	/* render bangunan button */
	for(int x=500; x<520;x++) {
		for(int y=50; y<70;y++) {
			if(bangunan_on || x==500 || x==519 || y==50 || y==69)
				fb_driver.printPixel(x,y,20,20,20);
			else {

			}
		}
	}

	/* render jalan button */
	for(int x=500; x<520;x++) {
		for(int y=90; y<110;y++) {
			if(jalan_on || x==500 || x==519 || y==90 || y==109)
				fb_driver.printPixel(x,y,20,20,20);
			else {

			}
		}
	}

	/* render bangunan font */
	
	fb_driver.renderFont("BGN", 45, 550, 0, 25, 79);
	fb_driver.renderFont("JLN", 85, 550, 0, 25, 79);
}