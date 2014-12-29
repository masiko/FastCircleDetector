#include "stdio.h"
#include "vector"
#include "cv.h"
#include "highgui.h"
#include "lsd.h"

class FCD{
private:
	double* tangent;
	std::vector<double> normal;
	char countmap[307200];
	std::vector<int> dst;

	int cols;
	int rows;
	int wsize;

public:
	FCD();
	FCD(int);
	~FCD(){};
	int Ipl2Double(IplImage* in, double* out);
	double* getTangent(int* num);
	std::vector<int> getDst();
	int getLS(double* in, int* num, int width, int height);
	std::vector<double> cvtNormal(int num);
	int process();
	int detectCircle();
//	int isCircleEdge(double x, double y, double ang, double dis);
};
