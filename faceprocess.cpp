/* 
 * faceprocess.cpp
 *
 *  Created on: 2012. 5. 15.
 *      Author: maneulyori
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "faceprocess.h"

ImageAvg *avgFace(IplImage * image, CvPoint pt1, CvPoint pt2)
{
	int x, y;
	unsigned int redSum = 0;
	unsigned int greenSum = 0;
	unsigned int blueSum = 0;
	ImageAvg *ret;

	ret = (ImageAvg *) malloc(sizeof(ImageAvg));

	for (x = pt1.x; x < pt2.y; x++)
	{
		for (y = pt1.y; y < pt2.y; y++)
		{
			redSum +=
				image->imageData[pt1.x * image->nChannels +
								 image->widthStep * pt1.y + 2];
			greenSum +=
				image->imageData[pt1.x * image->nChannels +
								 image->widthStep * pt1.y + 1];
			blueSum +=
				image->imageData[pt1.x * image->nChannels +
								 image->widthStep * pt1.y + 0];
		}
	}

	redSum /= (pt2.x - pt1.x) * (pt2.y - pt1.y);
	greenSum /= (pt2.x - pt1.x) * (pt2.y - pt1.y);
	blueSum /= (pt2.x - pt1.x) * (pt2.y - pt1.y);

	ret->redAverage = redSum;
	ret->greenAverage = greenSum;
	ret->blueAverage = blueSum;

	return ret;
}

uint16_t avgFaceDepth(IplImage *image)
{
	int x, y;
	uint64_t sum = 0;

	for(x=0; x<image->width; x++)
	{
		for(y=0; y<image->height; y++)
		{
			sum += (((uint16_t *) (image->imageData)) + y * image->width)[x];
		}
	}

	return (uint16_t)(sum / (image->width * image->height));
}
