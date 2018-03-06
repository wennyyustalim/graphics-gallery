#include "xrenderhome.h"
using namespace std;

RenderHome::RenderHome(FramebufferDriver& fb){
	fbd=fb;
}

void RenderHome::render(){
	fbd.clearScreen();
	string text = "WELCOME TO";
	fbd.renderFont(text,90,250,3,79,0);	
	text = "UPNORMAL GALLERY";
	fbd.renderFont(text,135,150,3,79,0);
	text = "MENU";
	int menu_offset = 20;
	fbd.renderFont(text,195 + menu_offset,340,0,25,79);
	text = "1 FONT";
	fbd.renderFont(text,235 + menu_offset,270,0,25,79);
	text = "2 THEATER";
	fbd.renderFont(text,275 + menu_offset,270,0,25,79);
	text = "3 MAP";
	fbd.renderFont(text,315 + menu_offset,270,0,25,79);
	text = "4 GAMES";
	fbd.renderFont(text,355 + menu_offset,270,0,25,79);
}
