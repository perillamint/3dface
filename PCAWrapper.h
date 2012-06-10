/* 
 * PCAWrapper.h
 *
 *  Created on: 2012. 6. 10.
 *      Author: maneulyori
 */

#ifndef PCAWRAPPER_H_
#define PCAWRAPPER_H_

#include <vector>
#include <string>
#include <opencv/cv.h>

using namespace std;

typedef struct NameDistance
{
	string cName;
	double Distance;
} NDistance;

typedef struct SortData
{
	bool operator() (NameDistance * a, NameDistance * b) const
	{
		return (a->Distance <= b->Distance) ? true : false;
	}
}
SortData;

class PCAWrapper
{
  public:
	PCAWrapper();
	virtual ~ PCAWrapper();
	void insertImage(IplImage * image, char *name);
	void training();
	char *search(IplImage * image);
	NameDistance *GetNameDistance(int index);
	double measure(double *vector_1, double *vector_2, int starting_position,
				   int ending_position, int measurement, double *eigenvalue);
  private:
	  IplImage ** faceImage;
	IplImage **eigens;
	IplImage **projectionImage;
	IplImage *avg;
	CvTermCriteria criteria;
	  vector < IplImage * >trainImage;
	  vector < string > imageName;
	  vector < NDistance * >distance;

	int imageCnt;
	double **projectionVector;
};

#endif /* PCAWRAPPER_H_ */
