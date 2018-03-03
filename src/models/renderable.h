#ifndef _RENDERABLE_
#define _RENDERABLE_

#include <applib>

class Renderable {
	private:
		std::vector<Point> points;
		std::unordered_map<int, std::unordered_map<int, bool> > pixels;
		int red;
		int green;
		int blue;

	public:
		std::vector<Point>& getPoints();
        void setPoints(std::vector<Point>& _points){
            points = _points;
        }
        std::unordered_map<int, std::unordered_map<int, bool> >& getPixels();
        static std::vector<Renderable *> parseFile(std::string filename,double, int, int, int);
		/*
		 * Generate borders in pixels
		 */
		void createBorder();
		/*
		 * Rastering color in pixels
		 */
		void rasterColor();

		void setColor(int r,int g, int b);

		int getRed();
		int getBlue();
		int getGreen();
};

#endif
