#ifndef _RENDERHOME_
#define _RENDERHOME_

#include <applib>
using namespace std;

class RenderHome{
	public:
		RenderHome();
		void render();
		void scanFont();
		void renderFont(string text, int idxBaris, int idxKolom, int red, int green, int blue);
	private:
		FramebufferDriver fb;
		char a[960][35];
};

#endif
