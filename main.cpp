/*
 * main.cpp
 *
 *  Created on: 2012. 5. 12.
 *      Author: maneulyori
 */

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include "libfreenect_cv.h"

#define DEBUG printf("%s %s %d\n", __FILE__, __func__, __LINE__)

static CvMemStorage* storage = 0;
static CvHaarClassifierCascade* cascade = 0;

void detect_and_draw(IplImage* img, IplImage *depth)
{
    int scale = 1;

    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3);
    memcpy(temp->imageData, img->imageData, 640*480*3);

    IplImage *depthTemp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 16, 1);
    memcpy(depthTemp->imageData, depth->imageData, 640*480*2);

    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );

    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )
    {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        /*CvSeq* faces = cvHaarDetectObjects( temp, cascade, storage,
                                            1.1, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );
*/
    	CvSeq* faces = cvHaarDetectObjects( temp, cascade, storage,
                                            1.7, 2, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(40, 40) );

        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
           // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
            pt1.x = r->x*scale+(r->width*.2);
            pt2.x = (r->x+(r->width*(1-.2)))*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
            cvRectangle( temp, pt1, pt2, CV_RGB(0,255,0), 3, 8, 0 );
            cvRectangle(depthTemp, pt1, pt2, CV_RGB(0,255,0), 3, 8, 0);
        }
    }

    // Show the image in the window named "result"
    cvShowImage( "result", temp );
    cvShowImage("resultDepth", depthTemp);

    // Release the temp image created.
    cvReleaseImage( &temp );
}

int main(int argc, char **argv)
{
	cascade = (CvHaarClassifierCascade*)cvLoad("cascade.xml", 0, 0, 0 );

	if( !cascade )
	{
	    fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
	    return -1;
	}

	storage = cvCreateMemStorage(0);

	while (cvWaitKey(10) < 0) {
		IplImage *image = freenect_sync_get_rgb_cv(0);
		if (!image) {
		    printf("Error: Kinect not connected?\n");
		    return -1;
		}

		DEBUG;
/*
		IplImage *irimage = freenect_sync_get_ir_cv(0);
		if (!irimage) {
		    printf("Error: Kinect not connected?\n");
		    return -1;
		}*/

		//DEBUG;

		//cvCvtColor(image, image, CV_RGB2BGR);
		IplImage *depth = freenect_sync_get_depth_cv(0);
		if (!depth) {
		    printf("Error: Kinect not connected?\n");
		    return -1;
		}

		DEBUG;

		detect_and_draw(image, depth);
		cvShowImage("RGB", image);

		//DEBUG;
		//cvShowImage("IR", irimage);
		cvShowImage("Depth", depth);
		//cvShowImage("Depth", GlViewColor(depth));
	}
	return 0;
}
