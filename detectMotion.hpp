#include <stdio.h>
#include <iostream> 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

class detectMotion
{
public:
	detectMotion(char *_videoname, int Threshold);
	~detectMotion();

protected:
private:
	char *videoname;
	int T;
	
};