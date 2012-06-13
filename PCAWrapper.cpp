/* 
 * PCAWrapper.cpp
 *
 *  Created on: 2012. 6. 10.
 *      Author: maneulyori
 *  Based on karuanas's code. http://karuanas.blogspot.kr/2011/10/opencv-pca-class.html
 */

#include "PCAWrapper.h"
#include <opencv/cv.h>
#include <opencv/cvaux.h>

#define DEBUG printf("%s %s %d\n", __FILE__, __func__, __LINE__)

PCAWrapper::PCAWrapper()
{
	imageCnt = 0;
	avg = 0;
	eigens = 0;
	faceImage = 0;
	projectionImage = 0;
	projectionVector = 0;

}

PCAWrapper::~PCAWrapper()
{
	int i;
	for (i = 0; i < imageCnt; i++)
	{
		cvReleaseImage(&trainImage[i]);
		if (i != imageCnt - 1)
		{
			cvReleaseImage(&eigens[i]);
		}
	}
	if (distance.size() != 0)
		for (i = 0; i < (int)distance.size(); i++)
			delete distance[i];
	cvReleaseImage(&avg);
	delete[]projectionVector[0];
	delete[]projectionVector;
	delete[]eigens;
	distance.clear();
	trainImage.clear();
	imageName.clear();
}

void PCAWrapper::insertImage(IplImage * image, char *name)
{
	IplImage *grayscale;

	grayscale = cvCreateImage(cvGetSize(image), image->depth, 1);

	cvCopy(image, grayscale);

	trainImage.push_back(grayscale);
	imageCnt++;
	imageName.push_back(name);
}

void PCAWrapper::training()
{
	double *vals;
	double **coeffs;
	int i, j;

	faceImage = new IplImage *[imageCnt];
	eigens = new IplImage *[imageCnt - 1];
	projectionImage = new IplImage *[imageCnt];

	for (i = 0; i < imageCnt; i++)
	{
		faceImage[i] = trainImage[i];
		projectionImage[i] =
			cvCreateImage(cvGetSize(faceImage[i]), IPL_DEPTH_8U, 1);
	}

	for (i = 0; i < imageCnt - 1; i++)
	{
		eigens[i] = cvCreateImage(cvGetSize(faceImage[i]), IPL_DEPTH_32F, 1);
	}

	vals = new double[imageCnt];
	memset(vals, 0, sizeof(double) * imageCnt);
	coeffs = new double *[imageCnt];
	coeffs[0] = new double[imageCnt * (imageCnt - 1)];
	DEBUG;
	for (i = 1; i < imageCnt; i++)
	{
		coeffs[i] = coeffs[i - 1] + (imageCnt - 1);
	}

	avg = cvCreateImage(cvGetSize(faceImage[0]), IPL_DEPTH_32F, 1);
	criteria = cvTermCriteria(CV_TERMCRIT_ITER, imageCnt - 1, 0);

	cvCalcEigenObjects(imageCnt, faceImage, eigens, 0, 0, 0, &criteria, avg,
					   (float *)vals);

	projectionVector = new double *[imageCnt];
	projectionVector[0] = new double[imageCnt * (imageCnt - 1)];

	for (i = 1; i < imageCnt; i++)
	{
		projectionVector[i] = projectionVector[i - 1] + (imageCnt - 1);
	}

	for (i = 0; i < imageCnt; i++)
	{
		for (j = 0; j < imageCnt-1; j++)
		{
			projectionVector[i][j] =
				cvCalcDecompCoeff(trainImage[i], eigens[j], avg);
		}
	}

	for (i = 0; i < imageCnt; i++)
	{
		cvReleaseImage(&projectionImage[i]);
	}

	delete[]coeffs[0];
	delete[]coeffs;
	delete[]faceImage;
	delete[]projectionImage;
	delete[]vals;
}

char *PCAWrapper::search(IplImage * image)
{
	IplImage *grayscale;
	int i;

	distance.clear();

	vector < double >distances;

	grayscale = cvCreateImage(cvGetSize(image), image->depth, 1);
	cvCopy(image, grayscale);

	double *searchVector = new double[imageCnt - 1];

	vector < double >dist;

	for (i = 0; i < imageCnt - 1; i++)
	{
		searchVector[i] = cvCalcDecompCoeff(grayscale, eigens[i], avg);
	}

	for (i = 0; i < imageCnt; i++)
	{
		double Tp =
			measure((double *)searchVector, (double *)projectionVector[i], 0,
					imageCnt - 2, 0, NULL);
		distances.push_back(Tp);

		NDistance *distanceSet = new NDistance;
		distanceSet->Distance = Tp;
		distanceSet->cName = imageName[i];
		distance.push_back(distanceSet);
	}

	double min = 1.0 / 0.0;
	int index = 0;

	for (i = 0; i < imageCnt; i++)
	{
		if (min > distances.at(i))
		{
			min = distances.at(i);
			index = i;
		}
	}

	cvReleaseImage(&grayscale);
	delete[]searchVector;

	return &imageName[index][0];
}

double PCAWrapper::searchDist(IplImage *image)
{
	IplImage *grayscale;
	int i;

	distance.clear();

	vector < double >distances;

	grayscale = cvCreateImage(cvGetSize(image), image->depth, 1);
	cvCopy(image, grayscale);

	double *searchVector = new double[imageCnt - 1];

	vector < double >dist;

	for (i = 0; i < imageCnt - 1; i++)
	{
		searchVector[i] = cvCalcDecompCoeff(grayscale, eigens[i], avg);
	}

	for (i = 0; i < imageCnt; i++)
	{
		double Tp =
			measure((double *)searchVector, (double *)projectionVector[i], 0,
					imageCnt - 2, 0, NULL);
		distances.push_back(Tp);

		NDistance *distanceSet = new NDistance;
		distanceSet->Distance = Tp;
		distanceSet->cName = imageName[i];
		distance.push_back(distanceSet);
	}

	double min = 1.0 / 0.0;

	for (i = 0; i < imageCnt; i++)
	{
		if (min > distances.at(i))
		{
			min = distances.at(i);
		}
	}

	cvReleaseImage(&grayscale);
	delete[]searchVector;

	return min;
}

NameDistance *PCAWrapper::GetNameDistance(int index)
{
	return distance[index];
}

double PCAWrapper::measure(double *vector_1, double *vector_2,
						   int starting_position, int ending_position,
						   int measurement, double *eigenvalue)
{
	double distance = 0.0;
	double temp = 0.0;
	double temp_1 = 0.0;
	double temp_2 = 0.0;
	if (measurement == 0)		// L1
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += abs(vector_1[i] - vector_2[i]);
		}
	}
	else if (measurement == 1)	// L2
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance +=
				(vector_1[i] - vector_2[i]) * (vector_1[i] - vector_2[i]);
		}
	}
	else if (measurement == 2)	// Cosine
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			temp += vector_1[i] * vector_2[i];
			temp_1 += vector_1[i] * vector_1[i];
			temp_2 += vector_2[i] * vector_2[i];
		}
		distance = -temp / sqrt(temp_1 * temp_2);
	}
	else if (measurement == 3)	// Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += vector_1[i] * vector_2[i] * eigenvalue[i];
		}
		distance = -distance;
	}
	else if (measurement == 4)	// L1 + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += abs(vector_1[i] - vector_2[i]) * eigenvalue[i];
		}
	}
	else if (measurement == 5)	// L2 + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance +=
				(vector_1[i] - vector_2[i]) * (vector_1[i] -
											   vector_2[i]) * eigenvalue[i];
		}
	}
	else if (measurement == 6)	// Cosine + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			temp += vector_1[i] * vector_2[i] * eigenvalue[i];
			temp_1 += vector_1[i] * vector_1[i];
			temp_2 += vector_2[i] * vector_2[i];
		}
		distance = -temp / sqrt(temp_1 * temp_2);
	}
	return distance;
}
