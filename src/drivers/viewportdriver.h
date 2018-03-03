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
		bool bangunan_on;
		bool jalan_on;
		ViewportDriver(int, int,Canvas*, FramebufferDriver&);
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void renderCanvas();
		bool isXinViewport(int x);
		bool isYinViewport(int y);
		bool isPointInViewPort(int x, int y);
		void renderOptions();	
};


#endif