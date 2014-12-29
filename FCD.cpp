#include "FCD.h"
#include "math.h"

FCD::FCD() {
	for (int i=0; i<307200; i++)	countmap[i] = 0;
	wsize = 10;
}

FCD::FCD(int w) {
	for (int i=0; i<307200; i++)	countmap[i] = 0;
	wsize = w/2;
}


int FCD::Ipl2Double(IplImage *in, double out[]) {
	int y;
	unsigned char value;
	
	cols = in->width;
	rows = in->height;

	if (in->width*in->height > 307200)	return 1;

	for (int i=0; i<in->height; i++) {
		y = i*in->width;
		for (int j=0; j<in->width; j++) {
			value = (unsigned char)in->imageData[j+y];
			out[j+y] = (double)value;
		}
	}
	return 0;
}

std::vector<int> FCD::getDst() {
	return dst;
}

int FCD::getLS(double* in, int* num, int width, int height) {
	tangent = lsd_scale(num, in, width, height, 2.0);
	return 0;
}

std::vector<double> FCD::cvtNormal(int num) {
	double x1,y1,x2,y2;
	double x, y, a, b, dis;

	for (int i=0; i<num; i++) {
		x1 = *(tangent + 7*i);
		y1 = *(tangent + 7*i+1);
		x2 = *(tangent + 7*i+2);
		y2 = *(tangent + 7*i+3);
		x = (x1+x2)/2.0;
		y = (y1+y2)/2.0;
		a = -((x1-x2)/(y1-y2));
		b = y-(a*x); 
		dis = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

		if (dis>20)	continue;
		normal.push_back(x);
		normal.push_back(y);
		normal.push_back(a);
		normal.push_back(b);
	}
	return normal;
}

int FCD::process(){
	FILE* fp = fopen("log.txt","w");
	FILE* fp2 = fopen("log_circle.txt","w");
	int num = normal.size()/4;
	double x,y;
	double x1, y1, x2, y2;
	double a1, b1, a2, b2;
	double r1,r2;
	int count=0;

	for (int i=0; i<num; i++) {
		x1 = normal[4*i];
		y1 = normal[4*i+1];
		a1 = normal[4*i+2];
		b1 = normal[4*i+3];
		fprintf(fp, "[%.1f,%.1f,%.1f,%.1f]\n", x1,y1,a1,b1);
		
		for ( int j=i+1; j<num; j++) {
			x2 = normal[4*j];
			y2 = normal[4*j+1];
			a2 = normal[4*j+2];
			b2 = normal[4*j+3];
			

			//filter
			if (x2<x1-wsize || x1+wsize<x2 || y2<y1-wsize || y1+wsize<y2)	continue;
			if (fabs(a1-a2) <= 0.0001)	continue;

			x = (b2-b1)/(a1-a2);
			y = a1*x+b1;
			x = std::max(0.0, std::min(x, double(cols)));
			y = std::max(0.0, std::min(y, double(rows)));
			
			//filter
			r1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
			r2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));

			if ( fabs(r1-r2) > 10)	continue;
			fprintf(fp2, "[%.0f,%.0f,%.0f,%.0f], ", x1,y1,a1,b1);
			fprintf(fp2, "[%.0f,%.0f,%.0f,%.0f], ", x2,y2,a2,b2);
			fprintf(fp2, "/%.0f,%.0f,%.0f,%.0f/\n", x,y,r1,r2);

			countmap[(int)x+cols*(int)y] += 1;
			count++;
		}
	}
	fclose(fp);
	fclose(fp2);
	return count;
}

int FCD::detectCircle(){
	int num = cols*rows;
	if (num > 307200)	return 1;

	int c=0;

	for (int i=0; i<rows/4; i++) {
		for (int j=0; j<cols/4; j++) {
			c=0;
			for (int k=0; k<4; k++) {
				for (int l=0; l<4; l++) {
					if (countmap[4*i*cols+4*j+k*cols+l]) {
						c++;
					}
				}
			}
			if (c>5) {
				dst.push_back(4*j);
				dst.push_back(4*i);
			}
		}
	}
	return 0;
}

