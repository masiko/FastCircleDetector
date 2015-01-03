#include "stdio.h"
#include "cv.h"
#include "highgui.h"

class DownScale{
private:
	IplImage* img;
	int scale;
public:
	DownScale();
	~DownScale();
	int setImage(IplImage* in, int scale);
	int downscale(IplImage* out, unsigned char color);
	int downscale(IplImage* out, IplImage* map, unsigned short color);
};
