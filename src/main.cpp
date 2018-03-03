#include <applib>

using namespace std;

void initCaptureKeyboard();
void garbageCaptureKeyboard();
void startKeystrokeThread();
ViewportDriver *vDriver;
Point p(2,5);
const struct timespec* delay = (const struct timespec[]){{0,2*16666667L}};

int main() {
	RenderHome home;
	home.render();

/*

    vector<Renderable * > bangunan = Renderable::parseFile("res/bangunanitb.txt", 1,20,20,255);
    vector<Renderable * > jalan = Renderable::parseFile("res/jalan2.txt", 1.37, 20,20,20);
    jalan.insert(jalan.end(),bangunan.begin(), bangunan.end());

    Canvas* canvas = new Canvas(jalan);
	vDriver = new ViewportDriver(0,0,canvas);
    startKeystrokeThread();
	int dum;
    while(1){
	cin >> dum;
        //print viewport di sini
        //cout << p.getX() << " " << p.getY() << endl;
        nanosleep(delay,NULL);
    }

    garbageCaptureKeyboard();
*/
}

void initCaptureKeyboard() {
    // Input keyboard device file
    const char *dev = "/dev/input/event2";
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
                    case 12:
                        // Minus triggered
                        //if(scale > 1) scale--;
                        cout<<"Minus"<<endl;
                        break;
                    case 13:
                        // plus triggered
                        //if(scale < 10) scale++;
                        cout<<"Plus"<<endl;
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
                        vDriver->moveUp();
                        break;
                    case 105:
                        // Left arrow trigger
                        //P1.setX(P1.getX() - 1);
			//cout << "leftarrow" << endl;
			vDriver->moveLeft();
				
                        break;

                    case 106:
                        // Right arrow trigger
                        //P1.setX(P1.getX() + 1);
			vDriver->moveRight();
			//cout << "right" << endl;
                        break;
                    case 108:
                        // Down arrow trigger
                        //P1.setY(P1.getY() + 1);
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
