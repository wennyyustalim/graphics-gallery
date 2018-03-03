#ifndef _RENDERHOME_
#define _RENDERHOME_

#include <applib>
using namespace std;

class RenderHome{
	public:
		RenderHome(FramebufferDriver&);
		void render();
	private:
		FramebufferDriver fbd;	
};

#endif
