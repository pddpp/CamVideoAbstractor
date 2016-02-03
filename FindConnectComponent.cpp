// For connected components:
// Approx.threshold - the bigger it is, the simpler is the boundary
//

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#define CVCONTOUR_APPROX_LEVEL  2
// How many iterations of erosion and/or dilation there should be
//
#define CVCLOSE_ITR  1

void find_connected_components(
	IplImage *mask,
	std::vector<CvRect>& vBox
	/*int poly1_hull0,
	float perimScale,
	int *n,
	CvPoint *centers*/
	) {
		vBox.clear();
		static CvMemStorage*   mem_storage = NULL;
		static CvSeq*          contours    = NULL;
		//CLEAN UP RAW MASK
		//
		cvMorphologyEx( mask, mask, 0, 0, CV_MOP_OPEN,  CVCLOSE_ITR );
		cvMorphologyEx( mask, mask, 0, 0, CV_MOP_CLOSE, CVCLOSE_ITR );
		//FIND CONTOURS AROUND ONLY BIGGER REGIONS
		//
		if( mem_storage==NULL ) {
			mem_storage = cvCreateMemStorage(0);
		} else {
			cvClearMemStorage(mem_storage);
		}
		CvContourScanner scanner = cvStartFindContours(
			mask,
			mem_storage,
			sizeof(CvContour),
			CV_RETR_EXTERNAL,
			CV_CHAIN_APPROX_SIMPLE
			);
		CvSeq *c;
		int numCont = 0;
		while( (c = cvFindNextContour( scanner )) != NULL ) {
			double len = cvContourPerimeter( c );
			// calculate perimeter len threshold:
			//
			double q = (mask->height + mask->width)/4;
			//Get rid of blob if its perimeter is too small:
			//
			if( len < q ) {
				cvSubstituteContour( scanner, NULL );
			} else {
				// Smooth its edges if its large enough
				//
				CvSeq* c_new;
				if( 1 ) {
					// Polygonal approximation
					//
					c_new = cvApproxPoly(
						c,
						sizeof(CvContour),
						mem_storage,
						CV_POLY_APPROX_DP,
						CVCONTOUR_APPROX_LEVEL,
						0);
				} else {
					// Convex Hull of the segmentation
					//
					c_new = cvConvexHull2(
						c,
						mem_storage,
						CV_CLOCKWISE,
						1
						);
				}
				cvSubstituteContour( scanner, c_new );
				numCont++;
			}
		}
		contours = cvEndFindContours( &scanner );
		// Just some convenience variables
		const CvScalar CVX_WHITE = CV_RGB(0xff,0xff,0xff);
		const CvScalar CVX_BLACK = CV_RGB(0x00,0x00,0x00);
		// PAINT THE FOUND REGIONS BACK INTO THE IMAGE
		//
		cvZero( mask );
		IplImage *maskTemp;

		// CALC CENTER OF MASS AND/OR BOUNDING RECTANGLES
		//
		/*if(num != NULL)*/ {
			//User wants to collect statistics
			//
			int /*N = *num,*/ numFilled = 0, i=0;
			CvMoments moments;
			double M00, M01, M10;
			maskTemp = cvCloneImage(mask);
			for(i=0, c=contours; c != NULL; c = c->h_next,i++ ) {
				{
					
					// Only process up to *num of them
					//
					cvDrawContours(
						maskTemp,
						c,
						CVX_WHITE,
						CVX_WHITE,
						-1,
						CV_FILLED,
						8
						);

					/*
					// Find the center of each contour
					//
					if(centers != NULL) {
						cvMoments(maskTemp,&moments,1);
						M00 = cvGetSpatialMoment(&moments,0,0);
						M10 = cvGetSpatialMoment(&moments,1,0);
						M01 = cvGetSpatialMoment(&moments,0,1);
						centers[i].x = (int)(M10/M00);
						centers[i].y = (int)(M01/M00);
					}
					*/
					//Bounding rectangles around blobs
					//
					vBox.push_back(cvBoundingRect(c));
					cvZero(maskTemp);
					numFilled++;
				}
				// Draw filled contours into mask
				//
				cvDrawContours(
					mask,
					c,
					CVX_WHITE,
					CVX_WHITE,
					-1,
					CV_FILLED,
					8
					);

			}                               //end looping over contours
			/*num = numFilled;*/
			cvReleaseImage( &maskTemp);
		}
}
