//
// Created by Kenneth Halim on 2/24/18.
//
#include <applib>
using namespace std;

vector<Point>& Renderable::getPoints() {
    return points;
}


vector<Renderable *> Renderable::parseFile(string filename, double scale, int red, int green, int blue) {
    vector<Renderable *> result;
    FILE * pFile;
    pFile = fopen(filename.c_str(),"r");
    //~ fprintf(stderr,"Reading new File: %s\n", filename.c_str());
    int q;
    while(fscanf(pFile, "%d", &q) && q!= -999) {

        int x, y;

        //~ fprintf(stderr,"Reading shape with %d point\n", q);
        Renderable *buffer = new Renderable();
        while(q--) {

            fscanf(pFile, "%d %d\n", &x, &y);
            //~ fprintf(stderr,"Reading point: %d %d\n",x ,y);
            Point pBuff(x * scale,y * scale);
            //~ fprintf(stderr, "Add point : %d %d", pBuff.getX(), pBuff.getY());
            buffer->getPoints().push_back(pBuff);
        }
        result.push_back(buffer);
        buffer->setColor(red, green, blue);
        buffer->createBorder();
        buffer->rasterColor();
    }
    return result;
}
	
void Renderable::setColor(int r, int g, int b){
	red = r;
	green = g;
	blue = b;
}

int Renderable::getRed() {
	return red;
}
int Renderable::getBlue() {
	return blue;
}
int Renderable::getGreen() {
	return green;
}


void Renderable::createBorder(){
	vector<int>::iterator itr;
	int steep;
	int x1,x2,y1,y2;
	int dx,dy;
	int derr,err;
	int x,y;
    //~ fprintf(stderr,"Points size: %d\n", points.size());
	for (int i = 0; i < points.size(); i++)
	{
		Point t1 = points[i];
		Point t2 = points[(i+1) % points.size()];
		x1 = t1.getX();
		x2 = t2.getX();
		y1 = t1.getY();
		y2 = t2.getY();
        //~ fprintf(stderr, "Creating line from %d %d to %d %d\n",x1,y1,x2,y2);
		//cout << *itr << endl;
		steep = 0;
		if(abs(x1-x2) < abs(y1-y2)){
	        swap(x1, y1);
	        swap(x2, y2);
	        steep = 1;
	    }
	    if(x1 > x2){
	        swap(x1,x2);
	        swap(y1,y2);
	    }
	    dx = x2-x1;
	    dy = y2-y1;
	    derr = 2 * abs(dy);
	    err = 0;
	    y = y1;
	    for(x = x1; x <= x2; x++){
	        if(steep){
	            pixels[y][x] = true;
	        }else{
	            pixels[x][y] = true;

	        }
	        err+=derr;
	        if(err > dx){
	            y += (y2>y1)?1:-1;
	            err -= 2 * dx;
	        }
	    }
	}
}

void Renderable::rasterColor(){
    int xmin = 9999999, ymin = 9999999;
    int xmax = -9999999, ymax = -9999999;
    
    for(int i=0; i<points.size(); i++){
        xmax = (points[i].getX() > xmax)? points[i].getX():xmax;
        ymax = (points[i].getY() > ymax)? points[i].getY():ymax;
        xmin = (points[i].getX() < xmin)? points[i].getX():xmin;
        ymin = (points[i].getY() < ymin)? points[i].getY():ymin;
    }    
    // cout << xmin << " " << ymin << endl;
    // cout << xmax << " " << ymax << endl;
    unsigned char onFlag = 0;
    unsigned char started = 0;
    for(int i = 0; i < ymax-ymin+1; i++) {
        //cout << "tes" << endl;
        int arr[xmax-xmin+1];
        int nPoint = 0;
        int y = ymin + i;

        for(int j = 0; j <= xmax-xmin+1; j++) {
            int x = xmin + j;
            if(pixels[x][y]) {
                arr[nPoint] = j;
                while(pixels[x][y]) {
                    j++;
                    x = xmin + j;
                }
                nPoint++;
            }
        }
        //printf("i: %d, nPoint: %d\n", i, nPoint);
        int median = -1;
        
        if(nPoint % 2 != 0) {
            median = nPoint / 2;
        }
        // cout << nPoint << endl;
        //printf("median: %d\n", median);
        //printf("Start Printing::\n");
        if(nPoint > 1 && i!=0 && i!=(xmax-xmin+1)) {
            for(int it = 0; it < nPoint-1; it+=2) {
                if(it == median) {
                    it++;
                }
                int startPoint = it;
                int endPoint = it+1;
                if(endPoint == median)
                    endPoint++;
                //printf("SP: %d %d EP %d %d\n", startPoint, arr[startPoint], endPoint, arr[endPoint]);
                if(endPoint < nPoint){
                    if(arr[endPoint] > arr[startPoint]){
                        for(int jt = arr[startPoint]; jt < arr[endPoint];jt++){
                            int x = xmin + jt;
                            pixels[x][y] = true;
                            //~ fprintf(stderr, "Filling point %d %d\n", x, y);
                        }
                    }
                }
            }
        }
    }
}

std::unordered_map<int, std::unordered_map<int, bool> >& Renderable::getPixels() {
    return pixels;
};


