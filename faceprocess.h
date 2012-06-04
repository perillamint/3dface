/* 
 * faceprocess.h
 *
 *  Created on: 2012. 5. 15.
 *      Author: maneulyori
 */

#ifndef FACEPROCESS_H_
#define FACEPROCESS_H_

struct Imageavg
{
	unsigned int redAverage;
	unsigned int greenAverage;
	unsigned int blueAverage;
};

typedef Imageavg ImageAvg;

uint16_t avgFaceDepth(IplImage *image);
void stretchFaceDepth(IplImage *image);

#endif /* FACEPROCESS_H_ */
