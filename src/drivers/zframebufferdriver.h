#ifndef _FBDRIVER_
#define _FBDRIVER_
#include <applib>

class FramebufferDriver {
	public:
		struct fb_var_screeninfo vinfo;
		struct fb_fix_screeninfo finfo;
		char *fbp;
		int fbfd;
		long int screensize;
		long int location;
		char a[960][35];

		FramebufferDriver();
		void init();
		void printPixel(int x, int y, int colorR, int colorG, int colorB);
		void clearScreen();
		void scanFont();
		void renderFont(std::string text, int idxBaris, int idxKolom, int red, int green, int blue);
};



#endif
