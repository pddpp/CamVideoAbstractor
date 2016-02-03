#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <vector>
///////////////////////////////////////////////////////////////////
// void find_connected_components(IplImage *mask, int poly1_hull0,
//                            float perimScale, int *num,
//                            CvRect *bbs, CvPoint *centers)
// This cleans up the foreground segmentation mask derived from calls
// to backgroundDiff
//
// mask          Is a grayscale (8-bit depth) ¡°raw¡± mask image that
//               will be cleaned up
//
// OPTIONAL PARAMETERS:
// poly1_hull0   If set, approximate connected component by
//                 (DEFAULT) polygon, or else convex hull (0)
// perimScale    Len = image (width+height)/perimScale. If contour
//                 len < this, delete that contour (DEFAULT: 4)
// num           Maximum number of rectangles and/or centers to
//                 return; on return, will contain number filled
//                 (DEFAULT: NULL)
// bbs           Pointer to bounding box rectangle vector of
//                 length num. (DEFAULT SETTING: NULL)
// centers      Pointer to contour centers vector of length
//                 num (DEFAULT: NULL)
//
void find_connected_components(
	IplImage* mask,
	std::vector<CvRect>& vBox
	/*int       poly1_hull0 = 1,
	float     perimScale  = 4,
	int*      n         = NULL,
	CvPoint*  centers     = NULL*/
	);