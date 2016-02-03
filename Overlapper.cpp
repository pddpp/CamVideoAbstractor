#include "Overlapper.hpp"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 
void videoOverlapper::OverlapForeground()
{
	const int times = 3;
	CvCapture *capture[times], *bgcapture;  
	int sumframe, m;

	for(int i = 0; i<times; i++)
	{
		if( !(capture[i] = cvCaptureFromFile(infilename)))
		{
			fprintf(stderr, "Can not open foreground video file %s\n", infilename);
			return ;
		}
		sumframe = (int) cvGetCaptureProperty(capture[0], CV_CAP_PROP_FRAME_COUNT);
		m = sumframe/times+0.5;
		cvSetCaptureProperty(capture[i], CV_CAP_PROP_POS_FRAMES, i*m+1);
	}

	CvVideoWriter* Writer = NULL;
	//double outCompressCod = cvGetCaptureProperty(capture[0], CV_CAP_PROP_FOURCC);
	Writer = cvCreateVideoWriter (outfilename, cvGetCaptureProperty(capture[0], CV_CAP_PROP_FOURCC),
		cvGetCaptureProperty(capture[0], CV_CAP_PROP_FPS ),cvSize (cvGetCaptureProperty( capture[0],CV_CAP_PROP_FRAME_WIDTH ),
		cvGetCaptureProperty( capture[0], CV_CAP_PROP_FRAME_HEIGHT)),1);
	
	int counter = 0;
	while( cvGrabFrame(capture[times-1]) )
	{
		IplImage *pFrame[(const int)times], *pgrayFrame;
		IplImage* pMask[(const int)times];
		//bgFrame = cvQueryFrame(bgcapture);
		pgrayFrame = cvCreateImage(backgroundImg.size(),  IPL_DEPTH_8U,1); 
		cv::Mat frameToWrite;
		backgroundImg.copyTo(frameToWrite);
		for (int i = 0; i<times; i++)
		{
			pFrame[i] = cvQueryFrame(capture[i]);// 获取当前帧
			cvCvtColor(pFrame[i], pgrayFrame, CV_BGR2GRAY);
			pMask[i] = cvCreateImage(backgroundImg.size(),  IPL_DEPTH_8U,1);
			cvThreshold( pgrayFrame, pMask[i], 250.0, 255.0, CV_THRESH_BINARY);
			cvNot(pMask[i],pMask[i]);
		}
		cvShowImage("video", pFrame[0]);

		for (int i = 0; i<times; i++)
		{
			cv::Mat mFrame(pFrame[i],0);
			cv::Mat mMask(pMask[i],0);
			//mFrame.copyTo(backgroundImg, mMask);
			mFrame.copyTo(frameToWrite, mMask);
		}
		cvShowImage("video", pFrame[0]);
		cvShowImage("mask1", pMask[0]);
		cvShowImage("mask2", pMask[1]);
		cvShowImage("mask3", pMask[2]);
		//imshow("result", backgroundImg);
		imshow("result", frameToWrite);
		/*IplImage* pbackgroundImg =  &IplImage (this->backgroundImg);
		cvWriteFrame (Writer, pbackgroundImg);
*/
		IplImage* pframeToWrite =  &IplImage (frameToWrite);
		cvWriteFrame (Writer, pframeToWrite);
		counter += 1;
		char k = (char) cv::waitKey(30);
		if( k == 27 ) 
			{
				std::cout<<"Frame number after overlapper" <<counter<<std::endl;
				break;
		}
	}
	//sprintf(test,"%s\\%d%s",savePath,count,".jpg");
	//cvSaveImage(test,pFrame);
	std::cout<<"Frame number after overlapper" <<counter<<std::endl;
	return;
}
videoOverlapper::videoOverlapper(char *_infilename, cv::Mat _bgimg, char *_outfilename)
{
	this->infilename = _infilename;
	this->backgroundImg = _bgimg;
	this->outfilename = _outfilename;
}
videoOverlapper::~videoOverlapper()
{
}
