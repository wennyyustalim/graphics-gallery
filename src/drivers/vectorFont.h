#ifndef _VECTOR_FONT_
#define _VECTOR_FONT_

#include <applib>

using namespace std;

class VFont {
	private:
		struct fb_var_screeninfo vinfo;
		struct fb_fix_screeninfo finfo;

		typedef struct{
			int r1,c1; //koord. titik awal
			int r2,c2; //koord. titik akhir
		}line;

		typedef struct{
			char font; //huruf apa
			int nline; //jumlah garis
			line border[40];
		}letter;
		
		letter alphabet[26];
		char bg[1000][1000];
		
	public:
		char *fbp = 0;
		int fbfd = 0;
		long int screensize = 0;
		long int location = 0;
		void init();
		void getPixelColor(int x, int y, int *rColor, int *gColor, int *bColor);
		int isBlack(int x, int y);
		void printPixel(int x, int y, int color);
		void scanFont();
		void drawLetter(int roffset, int coffset, letter x);
		void swap(int* a, int* b);
		void rasterizeFont(int roffset, int coffset, char font);
		void printpixelBG(int x, int y, int colorR, int colorG, int colorB);
		void clearScreen();
		void bresLine(int x_1, int y_1, int x_2, int y_2, int thickness);
};

#endif
