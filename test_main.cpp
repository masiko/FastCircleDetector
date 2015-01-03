#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "FCD.h"
#include "DownScale.h"

int main() {
	const int scale = 1;
	char fname[] = "Picture 9.jpg";// Picture 5.jpg
	int num;
	double in[MAX_SIZE];
	std::vector<int> v;
	FCD fcd(50, 4);
	DownScale ds;
	IplImage* img = cvLoadImage(fname);
	IplImage* gray = cvCreateImage(cvSize(img->width/scale, img->height/scale), img->depth, 1);
	cv::Mat result;
	result = cv::imread(fname);

	if (img->imageData == NULL)	return 1;

	ds.setImage(img, scale);
	ds.downscale(gray, 0);

	cvNamedWindow("dst");
	cvShowImage("dst", gray);
	cv::waitKey();

/*
	fcd.Ipl2Double(img, in);
	fcd.getLS(in, &num, img->width, img->height);
	fcd.cvtNormal(num);
	fcd.process();
	fcd.detectCircle();
	v = fcd.getDst();
*/	
//	v = fcd.fcd(gray);
	v = fcd.fcd(gray, 200, 130, 40, 40);
	printf("size: %d\n", v.size());	
	for (int i=0; i<v.size()/3; i++) {
		printf("%d, %d, %d\n", v[3*i], v[3*i+1], v[3*i+2]);
		cv::circle(result, cv::Point(v[3*i]*scale+150, v[3*i+1]*scale+65), v[3*i+2]*scale, cv::Scalar(0, 0, 255));
	}

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::imwrite("dst.png", result);

	cv::waitKey();
	return 0;
}

