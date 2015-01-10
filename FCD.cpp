#include "FCD.h"
#include "math.h"

FCD::FCD() {
	for (int i=0; i<MAX_SIZE; i++)	votemap[i] = 0, radiusmap[i] = 0;
	wsize = 10;
	scale = 4;
	MAX_LINE_LENGTH = 20;
	MAX_RADIUS_DIFF = 10;
	MIN_VOTE = 5;
}

FCD::FCD(int w, int s) {
	for (int i=0; i<MAX_SIZE; i++)	votemap[i] = 0;
	wsize = w/2;
	scale = s;
	MAX_LINE_LENGTH = 20;
	MAX_RADIUS_DIFF = 5;
	MIN_VOTE = 5;
}


int FCD::Ipl2Double(IplImage *in, double out[]) {
	int y;
	unsigned char value;
	
	cols = in->width;
	rows = in->height;

	if (in->width*in->height > MAX_SIZE)	return 1;

	for (int i=0; i<in->height; i++) {
		y = i*in->width;
		for (int j=0; j<in->width; j++) {
			value = (unsigned char)in->imageData[j+y];
			out[j+y] = (double)value;
		}
	}
	return 0;
}

int FCD::Ipl2Double(IplImage* in, int sx, int sy, int width, int height, double* out) {
	int ex,ey;
	int y;
	int k=0, l=0;
	unsigned char value;
	
	cols = width;
	rows = height;

	if (sx<0 || in->width<=sx || sy<0 || in->height<=sy)	return 1;
	if (width*height > MAX_SIZE)				return 2;

	ex = std::max(0, std::min(sx+width, in->width-1));
	ey = std::max(0, std::min(sy+height, in->height-1));
	for (int i=sy; i<ey; i++) {
		y = i*in->width;
		k++;
		l=0;
		for (int j=sx; j<ex; j++) {
			l++;
			value = (unsigned char)in->imageData[j+y];
			
			out[l+k*width] = (double)value;
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

		if (dis>MAX_LINE_LENGTH)	continue;
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
	double x,y,r;
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
//			if (fabs(a1-a2) <= 0.0001)	continue;

			x = (b2-b1)/(a1-a2);
			y = a1*x+b1;
			if (x>(double)cols*1.2 || y>(double)rows*1.2)	continue;
			else if (x<-(double)cols*0.2 || y<-(double)rows*0.2)	continue;
			x = std::max(0.0, std::min(x, double(cols)));
			y = std::max(0.0, std::min(y, double(rows)));
			
			//filter
			r1 = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
			r2 = sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));

			if ( fabs(r1-r2) > MAX_RADIUS_DIFF)	continue;
			fprintf(fp2, "[%.0f,%.0f,%.0f,%.0f,%.0f], ", x1,y1,a1,b1,r1);
			fprintf(fp2, "[%.0f,%.0f,%.0f,%.0f,%.0f], ", x2,y2,a2,b2,r2);
			fprintf(fp2, "/%.0f,%.0f,%.0f/\n", x,y,(r1+r2)/2);

			votemap[(int)x+cols*(int)y] += 1;
			radiusmap[(int)x+cols*(int)y] = (int)(r1+r2)/2;
			count++;
		}
	}
	fclose(fp);
	fclose(fp2);
	return count;
}

int FCD::detectCircle(){
	int c=0;
	int r=0;
	int pos;

	for (int i=0; i<rows/scale; i++) {
		for (int j=0; j<cols/scale; j++) {
			c=0;
			r=0;
			for (int k=0; k<scale; k++) {
				for (int l=0; l<scale; l++) {
					pos = scale*i*cols+scale*j+k*cols+l;
					if (votemap[pos]) {
						c += (int)votemap[pos];
						r += radiusmap[pos];
					}
				}
			}
			if (c>MIN_VOTE) {
				dst.push_back(scale*j);
				dst.push_back(scale*i);
			//	dst.push_back((int)(r/c));
				dst.push_back(c);
			}
		}
	}
	return 0;
}

int FCD::detectCircle(float* x, float *y){
	int cx=0, cy=0, num=0;
	int c=0;
	int r=0;
	int pos;

	for (int i=0; i<rows/scale; i++) {
		for (int j=0; j<cols/scale; j++) {
			c=0;
			r=0;
			for (int k=0; k<scale; k++) {
				for (int l=0; l<scale; l++) {
					pos = scale*i*cols+scale*j+k*cols+l;
					if (votemap[pos]) {
						c += (int)votemap[pos];
						r += radiusmap[pos];
					}
				}
			}
			if (c>MIN_VOTE) {
				if (c>num) {
					cx = scale*j;
					cy = scale*i;
					num = c;
				}
			}
		}
	}
	*x = (float)cx;
	*y = (float)cy;
	return 0;
}

std::vector<int> FCD::fcd(IplImage* img) {
	double in[MAX_SIZE];
	int num;

	Ipl2Double(img, in);
	getLS(in, &num, img->width, img->height);
	cvtNormal(num);
	process();
	detectCircle();	
	return getDst();
}

std::vector<int> FCD::fcd(IplImage* img, int x, int y, int width, int height) {
	double in[MAX_SIZE];
	int num;

	Ipl2Double(img, x, y, width, height, in);
	getLS(in, &num, width, height);
	cvtNormal(num);
	process();
	detectCircle();	
	return getDst();
}

int FCD::fcd(IplImage* img, int x, int y, int width, int height, float* dstx, float* dsty) {
	double in[MAX_SIZE];
	int num;

	Ipl2Double(img, x, y, width, height, in);
	getLS(in, &num, width, height);
	cvtNormal(num);
	process();
	detectCircle(dstx, dsty);	
	return 0;
}
