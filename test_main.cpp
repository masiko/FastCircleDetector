#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "FCD.h"

int main() {
	char fname[] = "Picture 11.jpg";
	int num;
	double in[307200];
	std::vector<int> v;
	FCD fcd(50);
	IplImage* img = cvLoadImage(fname, 0);
	cv::Mat result;
	result = cv::imread(fname);

	if (img->imageData == NULL)	return 1;

	cvNamedWindow("dst");
	cvShowImage("dst", img);
	cv::waitKey();

	fcd.Ipl2Double(img, in);
	fcd.getLS(in, &num, img->width, img->height);
	fcd.cvtNormal(num);
	fcd.process();
	fcd.detectCircle();
	v = fcd.getDst();
	
	printf("size: %d\n", v.size());	
	for (int i=0; i<v.size()/2; i++) {
		printf("%d, %d\n", v[2*i], v[2*i+1]);
		cv::circle(result, cv::Point(v[2*i], v[2*i+1]), 20, cv::Scalar(0, 0, 255));
	}

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::imwrite("dst.png", result);

	cv::waitKey();
	return 0;
}

