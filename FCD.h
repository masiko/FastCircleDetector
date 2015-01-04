#include "stdio.h"
#include "vector"
#include "cv.h"
#include "highgui.h"
#include "lsd.h"

#define MAX_SIZE (307200)

class FCD{
private:
	double* tangent;
	std::vector<double> normal;
	char votemap[MAX_SIZE];
	int radiusmap[MAX_SIZE];
	std::vector<int> dst;

	int cols;
	int rows;
	int wsize;
	int scale;

	//threshold
	int MAX_LINE_LENGTH;
	int MAX_RADIUS_DIFF;
	int MIN_VOTE;

public:
	FCD();
	FCD(int width, int scale);
	~FCD(){};
	int Ipl2Double(IplImage* in, int x, int y, int width, int height, double* out);
	int Ipl2Double(IplImage* in, double* out);
	int getLS(double* in, int* num, int width, int height);
	double* getTangent(int* num);
	std::vector<double> cvtNormal(int num);
	std::vector<int> getDst();	
	int process();
	int detectCircle();
	int detectCircle(float* x, float* y);
	std::vector<int> fcd(IplImage* in);
	std::vector<int> fcd(IplImage* in, int x, int y, int width, int height);
	int fcd (IplImage* in, int x, int y, int width, int height, float* dstx, float* dsty);
};
