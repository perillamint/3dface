/*
 * libfreenect_cv.cpp
 *
 *  Created on: 2012. 5. 12.
 *      Author: maneulyori
 */

#include <libfreenect.h>
#include <libfreenect_sync.h>
#include <memory.h>
#include "libfreenect_cv.h"
#include "dumpcode.h"

IplImage *freenect_sync_get_depth_cv(int index)
{
	static IplImage *image = 0;
	uint16_t *data = 0;
	uint16_t *targetData = 0;
	int i, j;

	if (!image) image = cvCreateImage(cvSize(640, 480), IPL_DEPTH_16U, 1);
	unsigned int timestamp;
	if (freenect_sync_get_depth((void**)&data, &timestamp, index, FREENECT_DEPTH_10BIT))
	    return NULL;

	//dumpcode((unsigned char*)data, 64);


	//
	//cvSetData(image, data, 640);

	//(uint16_t*)image->imageData = data;

	//memcpy(image->imageData, data, 640*480*2);

	targetData = (unsigned short int*)image->imageData;

	for(i=0; i<640; i++)
		for(j=0; j<480; j++)
			targetData[i*480 + j] = ((unsigned short int*)data)[i*480+j]*64;

	//dumpdepth((unsigned short int*)data, 64);
	return image;
}

IplImage *freenect_sync_get_rgb_cv(int index)
{
	static IplImage *image = 0;
	static char *data = 0;
	if (!image) image = cvCreateImage(cvSize(640,480), 8, 3);
	unsigned int timestamp;
	if (freenect_sync_get_video((void**)&data, &timestamp, index, FREENECT_VIDEO_RGB))
	    return NULL;
	cvSetData(image, data, 640*3);
	cvCvtColor(image, image, CV_RGB2BGR);
	return image;
}

IplImage *freenect_sync_get_ir_cv(int index)
{
	static IplImage *image = 0;
	static char *data = 0;
	if (!image) image = cvCreateImage(cvSize(640,480), 8, 1);
	unsigned int timestamp;
	if (freenect_sync_get_video((void**)&data, &timestamp, index, FREENECT_VIDEO_IR_8BIT))
	    return NULL;
	cvSetData(image, data, 640);
	return image;
}
