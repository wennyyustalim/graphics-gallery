#include <fcntl.h>
#include <linux/fb.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#define HEIGHT 500
#define WIDTH 900
#define INIT_HEIGHT 100
#define INIT_WIDTH 100

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

typedef struct{
    int x, y;
}point;

typedef struct{
    point point1;
    point point2;
}line;

typedef struct{
    line borders[1000];
}shape;

typedef struct{
    shape tail;
    shape left_wing;
    shape right_wing;
    
    point cockpit_center;
    int cockpit_radius;
    
    point body_center;
    int body_radius;
    
    point inner_center;
    int inner_radius;
}airplane;

airplane jet;
line hasil;
char bg[1000][1000];
point stack[500000];
point stackTemp[500000];
int isVisited[900][900];

char *fbp = 0;
int fbfd = 0;
long int screensize = 0;
long int location = 0;

// For Cohen Sutherland algorithm
const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

const int x_min = 100;
const int y_min = HEIGHT;
const int x_max = WIDTH;
const int y_max = 0;

// Calculating region code of a point
int computeCode(point p);

// Implement Cohen Sutherland's algorithm
void cohenSutherlandClipping(point p1, point p2);

void swap(int* a, int* b);
double DEG_to_RAD(double theta);

point setPoint(int x, int y);
point translatePoint(point p, int dx, int dy);
point rotatePoint(point p, double theta, point pivot);
point scalePoint(point p, double scale_factor, point pivot);

void loadFile();
void init();
void printPixel(int x, int y, int colorR, int colorG, int colorB);
void drawLine(point p1, point p2, int thickness, int colorR, int colorG, int colorB);
void drawCircle(point center, int radius, int colorR, int colorG, int colorB);
void clearScreen();
void getPixelColor(int x, int y, int *rColor, int *gColor, int *bColor);
int isBlack(int x, int y, int colorR, int colorG, int colorB);
void rasterize(int roffset, int coffset, int height, int width, int colorR, int colorG, int colorB);
void drawPropeller(int d1, int d2, point pivot, double theta, double scale_factor, int colorR, int colorG, int colorB);//theta = 0 start from pivot.x-d1/2
void drawAirplane(point offset, double scale_factor, double theta);
void floodFill(point source, int colorR, int colorG, int colorB);

int main() {
    init();   
    int i,j;
    
    loadFile();
    clearScreen();
    point p1 = setPoint(350, 250);
    point p2 = setPoint(150, 250);
    point pivot = setPoint(150 + 200, 90 + 250);
    point p1a,p2a;

    double k = 2.0;
    int enlarge = 1;
    int right = 1;
    int counter = 0;
    int x = 200, y = 250;
    double degree = 0;
    int counterGeser = 0;
    while(1) {    
        for (i = 1; i < 360; i += 24) {
            clearScreen();
            if (counterGeser == 10) {
                for(j = 1; j <= 230;j += 10) {
                    clearScreen();
                    point p = translatePoint(setPoint(x, y), -j, 0);
                    drawAirplane(p, k, degree);

                    point pivot = translatePoint(setPoint(x + 150, y + 90), -j, 0);
                    drawPropeller(120,30,pivot,i,k,150,0,20);
                    int offsetRow = pivot.y-120;
                    int offsetCol = pivot.x-120;
                    if(offsetRow<0){offsetRow = 0;}
                    else if(offsetRow>=HEIGHT){offsetRow = HEIGHT-1;}
                    if(offsetCol<0){offsetCol = 0;}
                    else if(offsetCol>=WIDTH){offsetCol = WIDTH-1;}
                    rasterize(offsetRow, offsetCol, 240,240,150,0,20);
                }

                int jSekarang = j;
                // printf("jSekarang: %d\n",j);
                for(j=-jSekarang;j<=300;j+=10){
                    clearScreen();
                    // printf("j lagi: %d\n",j);
                    point p = translatePoint(setPoint(x,y),j,0);
                    drawAirplane(p,k,degree);   
                    point pivot = translatePoint(setPoint(x+150,y+90),j,0);
                    drawPropeller(120,30,pivot,i,k,150,0,20);
                    int offsetRow = pivot.y-120;
                    int offsetCol = pivot.x-120;
                    if(offsetRow<0){offsetRow = 0;}
                    else if(offsetRow>=HEIGHT){offsetRow = HEIGHT-1;}
                    if(offsetCol<0){offsetCol = 0;}
                    else if(offsetCol>=WIDTH){offsetCol = WIDTH-1;}
                    rasterize(offsetRow, offsetCol, 240,240,150,0,20);
                }
                counterGeser = 0;
                return 0;
            }
            drawAirplane(setPoint(x,y),k,degree);
            
            if((x < 230)&&(right)){
                x++;
                degree -= 0.2;
            }else{
                right = 0;
                x--;
                degree += 0.2;
                if(x == 170){
                    right = 1;
                }
            }
            
            //p1a = rotatePoint(p1, i, pivot);
            //p2a = rotatePoint(p2, i, pivot);
            //drawLine(p2a,p1a,1);
            // printf("mau gambar\n");
            drawPropeller(120,30,setPoint(x+150,y+90),i,k,150,0,20);
            // floodFill(setPoint(x+150, y+90), 150,0,20);
            // printf("tergambar\n");
            
            if(counter >= 10){
	           k -= 0.12;
	        }
            // printf("masih\n");
            if(k >= 3.8){
                enlarge = 0;
                counter++;
            }
            // printf("masih2\n");
            if(k <= 1.0){
                enlarge = 1;
                counter = 0;
            }
            // printf("masih3\n");
            if(enlarge){
                k += 0.12;
            }
            // printf("counter: %d\n",counter);
            // printf("k: %.2lf\n",k);
            // printf("masih4\n");
            //printf("%lf\n",degree);
            int offsetRow = pivot.y-120;
            int offsetCol = pivot.x-120;
            if(offsetRow<0){offsetRow = 0;}
            else if(offsetRow>=HEIGHT){offsetRow = HEIGHT-1;}
            if(offsetCol<0){offsetCol = 0;}
            else if(offsetCol>=WIDTH){offsetCol = WIDTH-1;}
            rasterize(offsetRow, offsetCol, 240,240,150,0,20);

            // printf("halo\n");
            counterGeser++;
            //printPixel(p1.x, p1.y, 0, 100, 255);
            for(j = 0; j < 10000; j++){} //for delay
        }
    }
    
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
/***************** BASIC ******************/
void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

double DEG_to_RAD(double theta){
    return (theta*M_PI/180.0);    
}

/***************** LOAD FILE ******************/
void loadFile(){
    FILE *fairplane;
    fairplane = fopen("../data/airplane.txt","r");
    if(fairplane == NULL) {
        printf("No data in font.txt\n");
        return 0;
    }
    else {
        int xa,xb,ya,yb;
        int num_of_line;
        printf("ok\n");
        for(int i = 1; i <= 6; i++){
            char dummy;
            fscanf(fairplane, "%d",&num_of_line);
            
            for(int j = 0; j<num_of_line; j++){
                switch (i) {
                    case 1:
                        fscanf(fairplane, "%d %d %d %d",&xa,&ya,&xb,&yb);
                        jet.tail.borders[j].point1 = setPoint(xa,ya);
                        jet.tail.borders[j].point2 = setPoint(xb,yb);
                        break;
                    case 2:
                        fscanf(fairplane, "%d %d %d %d",&xa,&ya,&xb,&yb);
                        jet.left_wing.borders[j].point1 = setPoint(xa,ya);
                        jet.left_wing.borders[j].point2 = setPoint(xb,yb);
                        break;
                    case 3:
                        fscanf(fairplane, "%d %d %d %d",&xa,&ya,&xb,&yb);
                        jet.right_wing.borders[j].point1 = setPoint(xa,ya);
                        jet.right_wing.borders[j].point2 = setPoint(xb,yb);
                        break;
                    case 4:
                        fscanf(fairplane, "%d %d %d",&xa,&ya,&xb);
                        jet.body_center = setPoint(xa,ya);
                        jet.body_radius = xb;
                        break;
                    case 5:
                        fscanf(fairplane, "%d %d %d",&xa,&ya,&xb);
                        jet.inner_center = setPoint(xa,ya);
                        jet.inner_radius = xb;
                        break;
                    case 6:
                        fscanf(fairplane, "%d %d %d",&xa,&ya,&xb);
                        jet.cockpit_center = setPoint(xa,ya);
                        jet.cockpit_radius = xb;
                        break;
                    default:
                }
            }
        }
    }
    fclose(fairplane);
}

void init() {
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
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

}

void clearScreen() {
    for (int h = 0; h < HEIGHT; h++){
        for (int w = 100; w < WIDTH; w++) {
	    printPixel(w,h,122,224,255);
        }
    }
}

void printPixel(int x, int y, int colorR, int colorG, int colorB) {
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

    if (vinfo.bits_per_pixel == 32) {
        *(fbp + location) = colorB;
        *(fbp + location + 1) = colorG;
        *(fbp + location + 2) = colorR;
        *(fbp + location + 3) = 0;
    }
}

void drawLine(point p1, point p2, int thickness, int colorR, int colorG, int colorB){//Bresenham
    int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
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
            printPixel(y,x,colorR,colorG,colorB);
        }else{
            printPixel(x,y,colorR,colorG,colorB);
        }
        err+=derr;
        if(err > dx){
            y += (y2>y1)?1:-1;
            err -= 2 * dx;
        }
    }
}

void drawCircle(point center, int radius, int colorR, int colorG, int colorB){//Mid Point Algo
    int x = radius, y = 0;
    printPixel(x + center.x, y + center.y, colorR, colorG, colorB);
    if(radius > 0){
        printPixel(-x + center.x, y + center.y, colorR, colorG, colorB);
        printPixel(y + center.x, x + center.y, colorR, colorG, colorB);
        printPixel(y + center.x, -x + center.y, colorR, colorG, colorB);      
    }
    
    int P = 1 - radius;
    while(x > y){
        y++;
        if(P <= 0){
            P = P + 2*y + 1;
        }else{
            x--;
            P = P + 2*y - 2*x + 1;
        }

        if(x < y){ break; }
        printPixel(x + center.x, y + center.y, colorR, colorG, colorB);
        printPixel(-x + center.x, y + center.y, colorR, colorG, colorB);
        printPixel(x + center.x, -y + center.y, colorR, colorG, colorB);
        printPixel(-x + center.x, -y + center.y, colorR, colorG, colorB); 

        if(x != y){
            printPixel(y + center.x, x + center.y, colorR, colorG, colorB);
            printPixel(-y + center.x, x + center.y, colorR, colorG, colorB);
            printPixel(y + center.x, -x + center.y, colorR, colorG, colorB);
            printPixel(-y + center.x, -x + center.y, colorR, colorG, colorB); 

        }
    }
}

// Colouring

void getPixelColor(int x, int y, int *rColor, int *gColor, int *bColor) {
    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
            (y+vinfo.yoffset) * finfo.line_length;
    *bColor = *(fbp+location);
    *gColor = *(fbp+location+1);
    *rColor = *(fbp+location+2);
}

int isBorder(int x, int y, int colorR, int colorG, int colorB) {
    int rColorfb;
    int gColorfb;
    int bColorfb;
    getPixelColor(x,y,&rColorfb, &gColorfb, &bColorfb);
    
    rColorfb = (rColorfb < 0)? rColorfb + 256:rColorfb;
    gColorfb = (gColorfb < 0)? gColorfb + 256:gColorfb;
    bColorfb = (bColorfb < 0)? bColorfb + 256:bColorfb;
    
    return(rColorfb == colorR && gColorfb == colorG && bColorfb == colorB);
}

void rasterize(int roffset, int coffset, int height, int width, int colorR, int colorG, int colorB) {
    unsigned char onFlag = 0;
    unsigned char started = 0;
    for(int i = 0; i < height; i++) {
        int arr[width];
        int nPoint = 0;
        int y = roffset + i;

        for(int j = 0; j <= width; j++) {
            int x = coffset + j;
            if(isBorder(x,y,colorR,colorG,colorB)) {
                arr[nPoint] = j;
                while(isBorder(x,y,colorR,colorG,colorB)) {
                    j++;
                    x = coffset + j;
                }
                nPoint++;
            }
        }
        int median = -1;
        
        if(nPoint % 2 != 0) {
            median = nPoint / 2;
        }
       
        if(nPoint > 1 && i != 0 && i != height) {
            for(int it = 0; it < nPoint-1; it += 2) {
                if(it == median) {
                    it++;
                }
                int startPoint = it;
                int endPoint = it + 1;
                if(endPoint == median) endPoint++;
                if(endPoint < nPoint) {
                    if(arr[endPoint] > arr[startPoint]){
                        for(int jt = arr[startPoint]; jt < arr[endPoint];jt++){
                            int x = coffset + jt;
                            printPixel(x,y,colorR,colorG,colorB);
                        }
                    }
                }
            }
        }
    }
}

// Point
point setPoint(int x, int y){
    point p;
    p.x = x;
    p.y = y;

    return p;
}

point translatePoint(point p, int dx, int dy){
    return setPoint(p.x + dx, p.y + dy);    
}

point rotatePoint(point p, double theta, point pivot){
    double rad = DEG_to_RAD(theta);
    int x = (int) ((double)(p.x-pivot.x) * cos(rad) - (double)(p.y-pivot.y) * sin(rad)) + pivot.x;
    int y = (int) ((double)(p.x-pivot.x) * sin(rad) + (double)(p.y-pivot.y) * cos(rad)) + pivot.y;
    
    return setPoint(x,y); 
}

point scalePoint(point p, double scale_factor, point pivot){
    int x = (int) ((double)(p.x-pivot.x) * scale_factor) + pivot.x;
    int y = (int) ((double)(p.y-pivot.y) * scale_factor) + pivot.y;
    
    return setPoint(x,y);
}

// Custom object
void drawPropeller(int d1, int d2, point pivot, double theta, double scale_factor, int colorR, int colorG, int colorB){
    point p1, p2, p3, p4, po1, pt1, po2, pt2, po3, pt3, po4, pt4;
    int i;
    point p[5];
    p[0] = setPoint(pivot.x-d1/2, pivot.y);
    p[1] = setPoint(pivot.x, pivot.y-d2/2);
    p[2] = setPoint(pivot.x+d1/2, pivot.y);
    p[3] = setPoint(pivot.x, pivot.y+d2/2);   

    for(i = 0; i < 4; i++) {
        p[i] = rotatePoint(scalePoint(p[i], scale_factor, pivot), theta, pivot);
    }
    for(i = 0; i < 4; i++) {
        if (i<3) {
            cohenSutherlandClipping(p[i],p[i+1]);
        } else {
            cohenSutherlandClipping(p[3],p[0]);
        }
        
        po1 = hasil.point1; pt1 = hasil.point2;
        drawLine(po1, pt1, 1, colorR, colorG, colorB);
    }
}

void drawAirplane(point offset, double scale_factor, double theta) {
    // Left wing
    point p1,p2, pcolor;
    point pivot = setPoint(150+offset.x, 90+offset.y);
    point listPoint1[3], listPoint2[3];
    for(int i=0;i<3;i++){
        p1 = setPoint(jet.left_wing.borders[i].point1.x + offset.x, 
                      jet.left_wing.borders[i].point1.y + offset.y);
        p1 = scalePoint(p1, scale_factor, pivot);
        p1 = rotatePoint(p1, theta, pivot);

        p2 = setPoint(jet.left_wing.borders[i].point2.x + offset.x, 
                      jet.left_wing.borders[i].point2.y + offset.y);
        p2 = scalePoint(p2, scale_factor, pivot);
        p2 = rotatePoint(p2, theta, pivot);

        if(i == 0){
            pcolor = p1;
        }
        
        cohenSutherlandClipping(p1,p2);
        p1 = hasil.point1; p2 = hasil.point2;
        listPoint1[i] = p1; listPoint2[i] = p2;
        drawLine(p1, p2, 1, 107,91,0);    
    }
    if(listPoint1[0].x==100 && listPoint2[2].x==100){
        p1 = listPoint1[0]; p2 = listPoint2[2];
        drawLine(p1, p2, 1, 107,91,0);
    }
    int nilai1 = pcolor.y-50 * scale_factor;
    if(nilai1<0){nilai1 = 0;}
    else if(nilai1>=HEIGHT){nilai1 = HEIGHT-1;}
    int nilai2 = pcolor.x;
    if(nilai2<0){nilai2 = 0;}
    else if(nilai2>=WIDTH){nilai2 = WIDTH-1;}

    rasterize(nilai1, nilai2, 150 * scale_factor,150 * scale_factor,107,91,0);

    // Right wing
    for(int i=0;i<3;i++){
        p1 = setPoint(jet.right_wing.borders[i].point1.x + offset.x, 
                      jet.right_wing.borders[i].point1.y + offset.y);
        p1 = scalePoint(p1, scale_factor, pivot);
        p1 = rotatePoint(p1, theta, pivot);

        p2 = setPoint(jet.right_wing.borders[i].point2.x + offset.x, 
                      jet.right_wing.borders[i].point2.y + offset.y);
        p2 = scalePoint(p2, scale_factor, pivot);
        p2 = rotatePoint(p2, theta, pivot);

        if(i == 0){
            pcolor = p1;
        }

        cohenSutherlandClipping(p1,p2);
        p1 = hasil.point1; p2 = hasil.point2;
        listPoint1[i] = p1; listPoint2[i] = p2;
        drawLine(p1, p2, 1, 107,91,0);     
    }
    if(listPoint2[0].x==0 && listPoint1[1].x==0){
        p1 = listPoint2[0]; p2 = listPoint1[1];
        drawLine(p1, p2, 1, 107,91,0);
    }
    nilai1 = pcolor.y-50 * scale_factor;
    if(nilai1<0){nilai1 = 0;}
    else if(nilai1>=HEIGHT){nilai1 = HEIGHT-1;}
    nilai2 = pcolor.x-20;
    if(nilai2<0){nilai2 = 0;}
    else if(nilai2>=WIDTH){nilai2 = WIDTH-1;}
    rasterize(nilai1, nilai2, 150 * scale_factor,150 * scale_factor,107,91,0);
    for(int i=0;i<3;i++){
        p1 = setPoint(jet.tail.borders[i].point1.x + offset.x, 
                      jet.tail.borders[i].point1.y + offset.y);
        p1 = scalePoint(p1, scale_factor, pivot);
        p1 = rotatePoint(p1, theta, pivot);

        p2 = setPoint(jet.tail.borders[i].point2.x + offset.x, 
                      jet.tail.borders[i].point2.y + offset.y);
        p2 = scalePoint(p2, scale_factor, pivot);
        p2 = rotatePoint(p2, theta, pivot);

        if(i == 0){
            pcolor = p1;
        }
        
        cohenSutherlandClipping(p1,p2);
        p1 = hasil.point1; p2 = hasil.point2;
        drawLine(p1, p2, 1, 107,91,0);   
    }
    nilai1 = pcolor.y-20 * scale_factor;
    if(nilai1<0){nilai1 = 0;}
    else if(nilai1>=HEIGHT){nilai1 = HEIGHT-1;}
    nilai2 = pcolor.x-20 * scale_factor;
    if(nilai2<0){nilai2 = 0;}
    else if(nilai2>=WIDTH){nilai2 = WIDTH-1;}

    rasterize(nilai1, nilai2, 150 * scale_factor,150 * scale_factor,107,91,0);

    // Cockpit
    p1 = setPoint(jet.cockpit_center.x + offset.x, jet.cockpit_center.y + offset.y);
    p1 = rotatePoint(scalePoint(p1,scale_factor,pivot),theta,pivot);
    drawCircle(p1, jet.cockpit_radius * scale_factor, 114,114,114);
    rasterize(p1.y - (jet.cockpit_radius+5) * scale_factor, 
              p1.x - (jet.cockpit_radius+5) * scale_factor, 
              2 * (jet.cockpit_radius+5) * scale_factor, 
              2 * (jet.cockpit_radius+5) * scale_factor, 
              114, 114, 114);
    // Body
    p1 = setPoint(jet.body_center.x + offset.x, jet.body_center.y + offset.y);
    drawCircle(p1, jet.body_radius * scale_factor,168,143,0);
    rasterize(p1.y - (jet.body_radius+5) * scale_factor, 
              p1.x - (jet.body_radius+5) * scale_factor, 
              2 * (jet.body_radius+5) * scale_factor, 
              2 * (jet.body_radius+5) * scale_factor, 
              168, 143, 0);

    // Inner 
    p1 = setPoint(jet.inner_center.x + offset.x, jet.inner_center.y + offset.y);
    drawCircle(p1, jet.inner_radius * scale_factor,107, 80,0);
    rasterize(p1.y - (jet.inner_radius+5) * scale_factor, 
              p1.x - (jet.inner_radius+5) * scale_factor, 
              2 * (jet.inner_radius+5) * scale_factor, 
              2 * (jet.inner_radius+5) * scale_factor, 
              107, 80, 0);
}

int computeCode(point p){
    int x = p.x;
    int y = p.y;
    int code = INSIDE;
    if(x<x_min){code |= LEFT;}
    else if(x>x_max){code |= RIGHT;}

    if(y>y_min){code |= BOTTOM;}
    else if(y<y_max){code|=TOP;}

    return code;
}

void cohenSutherlandClipping(point p1, point p2){
    double x1 = p1.x, y1 = p1.y;
    double x2 = p2.x, y2 = p2.y;
    int code1 = computeCode(p1);
    int code2 = computeCode(p2);

    int isDrawn = 0;
    while(1){
        if((code1==0) && (code2==0)){
            isDrawn = 1;
            break;
        }else if(code1 & code2){
            point p1; p1.x = -1; p1.y = -1;
            point p2; p2.x = -1; p2.y = -1;
            hasil.point1 = p1;
            hasil.point2 = p2;
            break;
        }else{
            int code_out;
            double x,y;
            if(code1!=0){code_out = code1;}
            else{code_out = code2;}

            // Find intersection point
            if (code_out & TOP) {
                x = x1+(x2-x1)*(y_max - y1) / (y2-y1);
                y = y_max;
            }else if(code_out & BOTTOM){
                x = x1 + (x2-x1)*(y_min-y1) / (y2-y1);
                y = y_min;
            }else if(code_out & RIGHT){
                y = y1 + (y2-y1) * (x_max-x1) / (x2-x1);
                x = x_max;
            }else if(code_out & LEFT){
                y = y1 + (y2-y1) * (x_min-x1) / (x2-x1);
                x = x_min;
            }

            if(code_out == code1){
                x1 = x;
                y1 = y;
                code1 = computeCode(p1);
            }else{
                x2 = x;
                y2 = y;
                code2 = computeCode(p2);
            }
            isDrawn = 1;
            break;
        }
    }
    if(isDrawn){
        point p1; p1.x = x1; p1.y = y1;
        point p2; p2.x = x2; p2.y = y2;
        hasil.point1 = p1;
        hasil.point2 = p2;
    }
}

void floodFill(point source, int colorR, int colorG, int colorB){
    int dx[] = {0,1,0,-1};
    int dy[] = {-1,0,1,0};

    int i,j;
    int x = source.x, y = source.y;
    printPixel(x,y,colorR,colorG,colorB);
    memset(isVisited,0,sizeof isVisited);
    int ukuranStack = 1;
    stack[0] = source;
    isVisited[x][y] = 1;
    while(ukuranStack>0){
        int idxStackTemp = 0;
        int idxStack = 0;
        for(j=idxStack;j<ukuranStack;j++){
            x = stack[j].x; y = stack[j].y;

            for(i=0;i<4;i++){
                int xbaru = x+dx[i], ybaru = y+dy[i];
                if(xbaru>=0 && xbaru<WIDTH && ybaru>=0 && ybaru<HEIGHT){
                    location = (xbaru+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (ybaru+vinfo.yoffset) * finfo.line_length;
                    if((*(fbp+location)==colorB) && (*(fbp+location+1)==colorG) && (*(fbp+location+2)==colorR)){
                        printf("masuk kondisi sini\n");
                        continue;
                    }
                    if(isVisited[xbaru][ybaru]){continue;}
                    point pBaru;
                    pBaru.x = xbaru; pBaru.y = ybaru;
                    stackTemp[idxStackTemp] = pBaru; idxStackTemp++;
                    printPixel(xbaru,ybaru,colorR,colorG,colorB);
                    isVisited[xbaru][ybaru] = 1;
                }
            }
        }
        for(i=0;i<idxStackTemp;i++){
            stack[i] = stackTemp[i];
        }
        ukuranStack = idxStackTemp;
    }   
}