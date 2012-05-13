/*
 * libfreenect_cv.h
 *
 *  Created on: 2012. 5. 12.
 *      Author: maneulyori
 */

#ifndef LIBFREENECT_CV_H_
#define LIBFREENECT_CV_H_

#ifdef __cplusplus
//extern "C" {
#endif

#include <opencv/cv.h>

	IplImage *freenect_sync_get_depth_cv(int index);
	IplImage *freenect_sync_get_rgb_cv(int index);
	IplImage *freenect_sync_get_ir_cv(int index);

#ifdef __cplusplus
//}
#endif

#endif /* LIBFREENECT_CV_H_ */
