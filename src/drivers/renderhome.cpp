#include "renderhome.h"
using namespace std;

RenderHome::RenderHome(){
	fb.init();
	scanFont();
}

void RenderHome::render(){
	fb.clearScreen();
	string text = "WELCOME TO";
	renderFont(text,90,250,3,79,0);	
	text = "UPNORMAL GALLERY";
	renderFont(text,135,150,3,79,0);
	text = "MENU";
	int menu_offset = 20;
	renderFont(text,195 + menu_offset,340,0,25,79);
	text = "1 FONT";
	renderFont(text,235 + menu_offset,270,0,25,79);
	text = "2 THEATER";
	renderFont(text,275 + menu_offset,270,0,25,79);
	text = "3 MAP";
	renderFont(text,315 + menu_offset,270,0,25,79);
}

void RenderHome::renderFont(string text, int idxBaris, int idxKolom, int red, int green, int blue){
	int i,x,y;
	int pjg = text.length();
    for(i=0;i<pjg;i++){
        char kar = text[i];
        int absis = (kar-'A')*32, ordinat = 0; //menentukan indeks baris dan kolom dari array of char
	if(kar == '1'){
		absis = 864;
	}else if(kar == '2'){
		absis = 896;	
	}else if(kar == '3'){
		absis = 928;
	}
        if(kar==' '){absis = 832;}
        
        for (y = idxBaris; y < idxBaris+32; y++){
            for (x = idxKolom; x < idxKolom+32; x++) {
		if(a[absis][ordinat] == '0'){
			fb.printPixel(x,y,255,255,255);
		}else{
	 		fb.printPixel(x,y,red,green,blue);
		}
		ordinat++;
            }
            absis++;
            ordinat = 0;
        }
        idxKolom+=32;
        if(idxKolom>=700){idxKolom = 0; idxBaris+=34;} //jika melebihi pixel column..lanjutkan di baris selanjutnya dan idxKolom direset jadi 0
    }

}

void RenderHome::scanFont(){
	FILE *fp;
	int i,j;
	fp = fopen("res/alphabet.txt","r");
	if(fp==NULL){
		printf("File tidak ada\n");
		return;
	}

	for(i=0;i<960;i++){
		fscanf(fp, "%s", a[i]);
	}
	fclose(fp);
}
