#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 
#include "FindConnectComponet.h"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"

class ModelBuilder
{
public:
	int ReadVideo();
	cv::BackgroundSubtractorMOG2 UpdateModel();
	cv::Mat FinalBG();
	ModelBuilder(char *_videoname);
	~ModelBuilder();

	cv::Mat img, currentbgimg, finalbgimg;
protected:
private:
	char *videoname;
	CvCapture* pCapture;
	//IplImage *pFrame;

	cv::VideoCapture cap;

	//IplImage pfgimg, pblkimg;

	cv::BackgroundSubtractorMOG2 bg_model;
	cv::Mat currentfgimg, currentfgmask;
};

class ForegroundSaver
{
public:
	ForegroundSaver(cv::BackgroundSubtractorMOG2 _model, char *_videoname, char *_savename);
	int ReadVideo();
	void SubstractFG();
	int AddTimeInfo(int num);
	int WriteToFile();
	cv::Mat img, txtimg, fgimg, fgmask;
	CvVideoWriter* Writer; 
	IplImage *pfgmask;
protected:
private:
	cv::BackgroundSubtractorMOG2 model;
	char *videoname,*savename;
	CvCapture* pCapture;
	IplImage *pfgimg;
	
	cv::VideoCapture cap;
};

