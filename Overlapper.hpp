#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class videoOverlapper
{
public:
	void OverlapForeground();
	videoOverlapper(char *_infilename, cv::Mat _bgimg, char *_outfilename);
	~videoOverlapper();
protected:
private:
	char *infilename;
	char *outfilename;
	char *midlfilename;
	cv::Mat backgroundImg;
};