#include<applib>
FramebufferDriver::FramebufferDriver() {
	fbfd = 0;
	screensize = 0;
	location = 0;
}

void FramebufferDriver::init() {
	fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (atoi(fbp) == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");
}

void FramebufferDriver::clearScreen() {
     for (int x = VIEWPORT_START_X; x < VIEWPORT_WIDTH + VIEWPORT_START_X; x++){
        for (int y = VIEWPORT_START_Y; y < VIEWPORT_HEIGHT + VIEWPORT_START_Y; y++) {
            printPixel(x,y,255,255,255);
        }
    }
}

void FramebufferDriver::printPixel(int x, int y, int colorR, int colorG, int colorB){  //Print Pixel Color using RGB
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32) {
        *(fbp + location)     = colorB;     //Blue Color
        *(fbp + location + 1) = colorG;     //Green Color
        *(fbp + location + 2) = colorR;     //Red Color
        *(fbp + location + 3) = 0;          //Transparancy
    }
}