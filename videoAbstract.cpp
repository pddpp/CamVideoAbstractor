#include "videoAbstract.hpp"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 

ModelBuilder::~ModelBuilder()
{
}

ModelBuilder::ModelBuilder(char *_videoname)
{
	this->videoname = _videoname;
}

int ModelBuilder::ReadVideo()
{
	const char *vname = videoname;
	pCapture = cvCaptureFromFile(vname);

	cap.open(videoname);

	if( !cap.isOpened() )
	{
		printf("can not open camera or video file\n");
		return -1;
	}
	return 1;
}

cv::BackgroundSubtractorMOG2 ModelBuilder::UpdateModel()
{
	cap >> img;
	bg_model(img, currentfgmask, -1);
	bg_model.getBackgroundImage(currentbgimg);
	return bg_model;
}

cv::Mat ModelBuilder::FinalBG()
{
	finalbgimg = currentbgimg;
	return finalbgimg;
}

ForegroundSaver::ForegroundSaver(cv::BackgroundSubtractorMOG2 _model, char *_videoname, char *_savename)
{
	this->model = _model;
	this->videoname = _videoname;
	this->savename = _savename;
}

int ForegroundSaver::ReadVideo()
{
	const char *vname = videoname;
	pCapture = cvCaptureFromFile(vname);

	cap.open(videoname);

	if( !cap.isOpened() )
	{
		printf("can not open camera or video file\n");
		return -1;
	}

	return 1;
}

void ForegroundSaver::SubstractFG()
{
	cap >> img;
	if( img.empty() )
		return;

	if( fgimg.empty() )
		fgimg.create(img.size(), img.type());

	if( txtimg.empty() )
		txtimg.create(img.size(), img.type());
	fgimg = cv::Scalar::all(0);
	txtimg = cv::Scalar::all(255);

	model(img, fgmask, -0.0000001);
	pfgmask =  &IplImage (fgmask);

	//cvErode(pfgmask, pfgmask, 0, 1);
	cvDilate(pfgmask, pfgmask, 0, 1);

	//cvErode(pfgmask, pfgmask, 0, 1);
	//cvDilate(pfgmask, pfgmask, 0, 1);

	img.copyTo(fgimg, fgmask);
	//imshow("fgmask", fgmask);
}

int ForegroundSaver::AddTimeInfo(int num)
{
	int counter = 0;
	std::vector<CvRect> vBox;
	pfgmask =  &IplImage (fgmask);
	find_connected_components(pfgmask, vBox);
	
	for (int i = 0; i != vBox.size(); ++i)
		{
			CvPoint pt1;
			int b = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS );

			pt1.x = vBox[i].x;
			pt1.y = vBox[i].y;
			/*pt2.x = pt1.x + vBox[i].width;
			pt2.y = pt1.y + vBox[i].height;*/
			int a = num/b;
			char string[25];

			itoa( a, string, 10);

			CvFont font;

			double hscale = 0.5;
			double vscale = 0.5;
			int linewidth = 1;
			cvInitFont(&font,CV_FONT_HERSHEY_PLAIN | CV_FONT_ITALIC,hscale,vscale,0,linewidth);

			CvScalar textColor =cvScalar(255,255,255);
			cvPutText(pfgmask, string, pt1, &font, textColor);
		}
	counter +=vBox.size();
	cv::Mat gmask(pfgmask,0);
	fgimg.copyTo(txtimg, gmask);
	return counter;
}

int ForegroundSaver::WriteToFile()
{
	double i = cvGetCaptureProperty(pCapture, CV_CAP_PROP_FOURCC);
	Writer = cvCreateVideoWriter (savename, cvGetCaptureProperty(pCapture, CV_CAP_PROP_FOURCC),
		cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS ),cvSize (cvGetCaptureProperty( pCapture,CV_CAP_PROP_FRAME_WIDTH ),
		cvGetCaptureProperty( pCapture, CV_CAP_PROP_FRAME_HEIGHT)),1);

	if (Writer != NULL)
	{
		return 1;
	} 
	else
	{
		return 0;
	}
}
