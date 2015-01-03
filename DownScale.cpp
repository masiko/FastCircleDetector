#include "DownScale.h"

DownScale::DownScale(){};

DownScale::~DownScale(){};

int DownScale::setImage(IplImage* in, int s) {
	int col, row;
	scale = s;
	col = in->width;
	row = in->height;

	if (col==0 && row==0)	return 1;
	img = cvCreateImage(cvSize(col, row), 8, 1);
	
	cvCvtColor(in, img, CV_BGR2GRAY);
	
	return 0;
}

int DownScale::downscale(IplImage* out, unsigned char color) {
	int col, row;
	int x,y;
	int flag=0;
	int pos=0;
	col = out->width;;
	row = out->height;

	if (col != img->width/scale || row != img->height/scale)	return 1;

	for (int i=0; i<row; i++) {
		y = i*scale*img->width;
		for (int j=0; j<col; j++) {
			x = j*scale;
			flag=0;
			for(int k=0; k<scale; k++) {
				for (int l=0; l<scale; l++) {
					if ( (unsigned char)img->imageData[x+y+l+k*img->width] > color) {
						flag=1;
						pos =x+y+l+k*img->width;
						break;
					}
				}
				if (flag)	break;
			}
			out->imageData[j+i*col] = flag ? (unsigned char)img->imageData[pos] : 0;
		}
	}
	return 0;
}

int DownScale::downscale(IplImage* out, IplImage* map, unsigned short color) {
	unsigned short* p = (unsigned short*)map->imageData;
	int col, row;
	int x,y;
	int flag=0;
	int pos=0;
	col = out->width;;
	row = out->height;

	if (col != img->width/scale || row != img->height/scale)	return 1;

	for (int i=0; i<row; i++) {
		y = i*scale*map->width;
		for (int j=0; j<col; j++) {
			x = j*scale;
			flag=0;
			for(int k=0; k<scale; k++) {
				for (int l=0; l<scale; l++) {
					if ( *(p + x+y+l+k*map->width) == color) {
						flag=1;
						pos =x+y+l+k*img->width;
						break;
					}
				}
				if (flag)	break;
			}
			out->imageData[j+i*col] = flag ? (unsigned char)img->imageData[pos] : 0;
		}
	}
	return 0;
}

