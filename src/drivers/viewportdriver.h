#ifndef _VIEWPORT_DRIVER
#define _VIEWPORT_DRIVER

#include<applib>
class ViewportDriver {
	private:
		Canvas* canvas;
		int x_offset;
		int y_offset;
		FramebufferDriver fb_driver;
	public:
		ViewportDriver(int, int,Canvas*);
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void renderCanvas();
		bool isXinViewport(int x);
		bool isYinViewport(int y);
		bool isPointInViewPort(int x, int y);	
};


#endif