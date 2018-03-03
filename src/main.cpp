#include <applib>

using namespace std;

// Mouse input initialization
#define HEIGHT 500
#define WIDTH 800
#define INIT_HEIGHT 100
#define INIT_WIDTH 100
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

char *fbp = 0;
int fbfd = 0;
long int screensize = 0;
long int location = 0;
const char *mDevice = "/dev/input/mice";    //~ input mouse driver
int fd;

typedef struct{
    Point coorMouse;
    int clicked;
}Mice;
// typedef struct {
//     int cR;
//     int cG;
//     int cB;
// } RGB;

typedef struct {
    int fd;
    Mice mice;
} FdMice;

void swap(int* a, int* b);
void printPixel(int x, int y, int colorR, int colorG, int colorB);
void drawLine(Point p1, Point p2, int thickness, int colorR, int colorG, int colorB);
Mice mouseController();
void setPointer(int x, int y);
void startDevice();
void getPixelColor(int x, int y, int *rColor, int *gColor, int *bColor);
void clearScreen();

RGB color_map[HEIGHT][WIDTH];
// End of Mouse input initialization

void initCaptureKeyboard();
void garbageCaptureKeyboard();
void startKeystrokeThread();
ViewportDriver *vDriver;
Point p(0,0);
const struct timespec* delay = (const struct timespec[]){{0,2*16666667L}};
FramebufferDriver fbDriver;
int menu = 0;

int main() {
	RenderHome home;
	home.render();
	startKeystrokeThread();
	
    vector<Renderable * > bangunan = Renderable::parseFile("res/bangunanitb.txt", 1,20,20,255);
    vector<Renderable * > jalan = Renderable::parseFile("res/jalan2.txt", 1.37, 20,20,20);
    jalan.insert(jalan.end(),bangunan.begin(), bangunan.end());

    Canvas* canvas = new Canvas(jalan);
    
    fbDriver.init();
    fbDriver.clearScreen();
	
	
	int dum = 9;
	//cin >> dum;
    
    startDevice();
    int xMouse = 0, yMouse = 0;
    startKeystrokeThread();


	
	while(1){
		//cout << menu << endl;
        	if(menu == 3){//print viewport di sini
			vDriver = new ViewportDriver(0,0,canvas,fbDriver);        		
			while(1){			
				//cout << p.getX() << " " << p.getY() << endl;
                    Mice pMouse = mouseController();
        // cout << "test1" << endl;
        // fbDriver.clearScreen();
        vDriver->renderCanvas();
        // cout << "test2" << endl;
        xMouse = (xMouse+pMouse.coorMouse.getX() > WIDTH) ?WIDTH :((xMouse+pMouse.coorMouse.getX() < 0) ?0 :(xMouse+pMouse.coorMouse.getX()));
        yMouse = (yMouse+pMouse.coorMouse.getY() > HEIGHT) ?HEIGHT :((yMouse+pMouse.coorMouse.getY() < 0) ?0 :(yMouse+pMouse.coorMouse.getY()));
        setPointer(xMouse, yMouse);
        if(pMouse.clicked==1 && yMouse >=0 && yMouse< HEIGHT && xMouse >=0 && yMouse < WIDTH) {
            color_map[yMouse][xMouse].cR = 0;
            color_map[yMouse][xMouse].cG = 0;
            color_map[yMouse][xMouse].cB = 0;
        }

    
        			nanosleep(delay,NULL);
				if(menu != 3){
					break;				
				}
			}
		}else if(menu == 0){
			home.render();
			nanosleep(delay,NULL);		
		}
    }

   while(1){
        // test
        // cout << "test0" << endl;
        //print viewport di sini
        //cout << p.getX() << " " << p.getY() << endl;
        nanosleep(delay,NULL);
    }


	garbageCaptureKeyboard();


	// int dum;
 //    cin >> dum;


}

void initCaptureKeyboard() {
    // Input keyboard device file
    const char *dev = "/dev/input/event3";
    struct input_event ev;
    ssize_t n;
    int fd;

    // Open device for reference
    fd = open(dev, O_RDONLY);

    // Check if device is opened
    if (fd == -1) {
        fprintf(stderr, "Cannot open %s: %s.\n", dev, strerror(errno));
        // return EXIT_FAILURE;
    }


    // Main loop, waiting for keystroke
    while (1) {
        // get stored value on keyboard device reference file
        n = read(fd, &ev, sizeof ev);

        // check if no stored value on device file
        if (n == (ssize_t)-1) {
            if (errno == EINTR)
                continue;
            else
                break;
        } else

            // another empty
        if (n != sizeof ev) {
            errno = EIO;
            break;
        }

        // if keystroke is stored on keyboard device reference file
        if (ev.type == EV_KEY && ev.value >= 0 && ev.value <= 2){

            // if(ev.value == 0) // when it released

            if(ev.value == 1) // when it pressed

                switch(ev.code){
                    case 2:
			//1 pressed
                        // 12 Minus triggered
                        //if(scale > 1) scale--;
                        cout<<"you choose 1"<<endl;
                        break;
                    case 3:
                        // 13 plus triggered
                        //if(scale < 10) scale++;
                        cout<<"you choose 2"<<endl;
                        break;
		    case 4:
                        // 13 plus triggered
                        //if(scale < 10) scale++;
                        //cout<<"you choose 3"<<endl;
			menu = 3;                        
			break;
		    case 11:
                        // 13 plus triggered
                        //if(scale < 10) scale++;
                        //cout<<"you choose 3"<<endl;
			menu = 0;                        
			break;
                    case 25:
                        // P trigger
                        //p ^= 1;
                        p.setY(p.getY()-1);
                        break;
                    case 36:
                        // J trigger
                        //j ^= 1;
                        break;
                    case 48:
                        // B trigger
                        //b ^= 1;
                        break;
                    case 57:
                        // Space trigger
                        break;
                    case 103:
                        // Up arrow trigger
                        //P1.setY(P1.getY() - 1);
                        if(menu==3)
                        vDriver->moveUp();
                        break;
                    case 105:
                        // Left arrow trigger
                        //P1.setX(P1.getX() - 1);
			//cout << "leftarrow" << endl;
			         if(menu==3)
                     vDriver->moveLeft();
				
                        break;

                    case 106:
                        // Right arrow trigger
                        //P1.setX(P1.getX() + 1);
                    if(menu==3)
			vDriver->moveRight();
			//cout << "right" << endl;
                        break;
                    case 108:
                        // Down arrow trigger
                        //P1.setY(P1.getY() + 1);
                    if(menu==3)
			vDriver->moveDown();
                        break;

                    default:
                        break;

                }

            // if(ev.value == 2) // when it repeated

            // More event code
            // https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
        }

    }
}

void garbageCaptureKeyboard() {
    // garbaging
    fflush(stdout);
    fprintf(stderr, "%s.\n", strerror(errno));
}

void startKeystrokeThread() {
    // Constructs the new thread and runs it. Does not block execution.
    std::thread t1(initCaptureKeyboard);

    // Makes the main thread wait for the new thread to finish execution, therefore blocks its own execution.
    t1.detach();
}

void setPointer(int x, int y){
    Point p1x((x <= 5) ?0 :(x-5), y);
    Point p2x((x >= WIDTH-5) ?WIDTH :(x+5), y);
    Point p1y(x, (y <= 5) ?0 :(y-5));
    Point p2y(x, (y >= HEIGHT-5) ?HEIGHT :(y+5));
    
    drawLine(p1x, p2x, 2, 0, 0, 0);
    drawLine(p1y, p2y, 2, 0, 0, 0);
}

Mice mouseController(){
    int bytes;
    unsigned char data[3];
    fd = open(mDevice, O_RDWR);
    if (fd == -1){
        printf("Mouse Error");
    }
    int click;
    signed char x, y;
    
    bytes = read(fd, data, sizeof(data));

    if (bytes > 0){
        click = data[0] & 0x1;              //~ Left click
        x = data[1];                        //~ x coordinate
        y = data[2];                        //~ y coordinate
        //~ printf("x=%d, y=%d, click=%d\n", x, y, click);
    }
    Mice M;
    // cout << "x,y: " << x << "," << y << endl;

    Point p(x, -y);
    M.coorMouse = p;
    M.clicked = click;
    return (M);
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void startDevice(){
    std::thread tMouse (mouseController);
    tMouse.detach();
}

void drawLine(Point p1, Point p2, int thickness, int colorR, int colorG, int colorB){//Bresenham
    int x1 = p1.getX(), y1 = p1.getY(), x2 = p2.getX(), y2 = p2.getY();
    int steep = 0;
    if(abs(x1-x2) < abs(y1-y2)){
        swap(&x1, &y1);
        swap(&x2, &y2);
        steep = 1;
    }
    if(x1 > x2){
        swap(&x1,&x2);
        swap(&y1,&y2);
    }
    int dx = x2-x1;
    int dy = y2-y1;
    int derr = 2 * abs(dy);
    int err = 0;
    int y = y1;
    for(int x = x1; x <= x2; x++){
        if(steep){
            fbDriver.printPixel((y > WIDTH) ?WIDTH-1 :((y < 0) ?0 :y),(x > HEIGHT) ?HEIGHT-1 :((x < 0) ?0 :x),colorR,colorG,colorB);
        }else{
            fbDriver.printPixel((x > WIDTH) ?WIDTH-1 :((x < 0) ?0 :x),(y > HEIGHT) ?HEIGHT-1 :((y < 0) ?0 :y),colorR,colorG,colorB);
        }

        err+=derr;
        if(err > dx){
            if (y >= 0 && y < HEIGHT){
                y += (y2>y1)?1:-1;
            }
            err -= 2 * dx;
        }
    }
}

void clearScreen() {
    for (int h = 0; h < HEIGHT; h++){
        for (int w = 0; w < WIDTH; w++) {
            printPixel(w,h,color_map[h][w].cR,color_map[h][w].cG,color_map[h][w].cB);
        }
    }
}

void getPixelColor(int x, int y, int *rColor, int *gColor, int *bColor) {
      location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *bColor = *(fbp+location);
            *gColor = *(fbp+location+1);
            *rColor = *(fbp+location+2);
}

void printPixel(int x, int y, int colorR, int colorG, int colorB){  //Print Pixel Color using RGB
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = colorB;
        *(fbp + location + 1) = colorG;
        *(fbp + location + 2) = colorR;
        *(fbp + location + 3) = 0;
    }
}
