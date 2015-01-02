#include "stdio.h"
#include "cv.h"
#include "highgui.h"
#include "FCD.h"

int main() {
	char fname[] = "Picture 11.jpg";// Picture 5.jpg
	int num;
	double in[MAX_SIZE];
	std::vector<int> v;
	FCD fcd(50, 4);
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
	for (int i=0; i<v.size()/3; i++) {
		printf("%d, %d, %d\n", v[3*i], v[3*i+1], v[3*i+2]);
		cv::circle(result, cv::Point(v[3*i], v[3*i+1]), v[3*i+2], cv::Scalar(0, 0, 255));
	}

	cv::namedWindow("result");
	cv::imshow("result", result);
	cv::imwrite("dst.png", result);

	cv::waitKey();
	return 0;
}

