#include <stdio.h>
#include <iostream> 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "detectMotion.hpp"

detectMotion::detectMotion(char *_videoname, int Threshold)
{
	this->videoname= _videoname;
	T = Threshold;

	//IplImage pointers
	IplImage* pFrame = NULL; 
	IplImage* pFrImg = NULL;
	IplImage* pBkImg = NULL;
	//CvMat pointers
	CvMat* pFrameMat = NULL;
	CvMat* pFrMat = NULL;
	CvMat* pBkMat = NULL;
	//CvVideoWriter pointers
	CvVideoWriter* vWriter = NULL;
	CvCapture* pCapture = NULL;

	int nFrmNum = 0;
	int Mt = 0; //Motion
	double TM = 0; //Total motion
	double AM = 0; //Avarage motion

	if( !(pCapture = cvCaptureFromFile(videoname)))
	{
		fprintf(stderr, "Can not open video file %s\n", videoname);
		return ;
	}

	//For each frame
	while(pFrame = cvQueryFrame( pCapture ))
	{
		nFrmNum++;

		//memory alloc and variable intial for first frame
		if(nFrmNum == 1)
		{
			//8 bit unsigned int array of size width*height
			pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1); 
			pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1); 
			//
			pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
			pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1); 
			pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

			//save to gray image
			cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
			//convert gray image to pFrameMat pointer
			cvConvert(pFrImg, pFrameMat);
			cvConvert(pFrImg, pFrMat);
			cvConvert(pFrImg, pBkMat);
			//double temp = cvGetCaptureProperty(pCapture,CV_CAP_PROP_FPS); //fps

			//init the writer pointer
			vWriter = cvCreateVideoWriter ("ccfgvideo.avi", cvGetCaptureProperty(pCapture, CV_CAP_PROP_FOURCC), cvGetCaptureProperty(pCapture, CV_CAP_PROP_FPS ),cvSize (pFrame->width,pFrame->height),1); 
			cvWriteFrame (vWriter, pFrame);
		}
		else
		{
			//convert to gray image
			cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
			cvConvert(pFrImg, pFrameMat);
			//gaussian filter
			//cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

			//get background frame
			cvAbsDiff(pFrameMat, pBkMat, pFrMat);

			//T=60
			cvThreshold(pFrMat, pFrImg, T, 255.0, CV_THRESH_BINARY);

			//erode and dilate to reduce hole in objects  
			cvErode(pFrImg, pFrImg, 0, 1);
			cvDilate(pFrImg, pFrImg, 0, 1);

			//calculate Mt and reset fps base on it
			Mt = (cvSum(pFrImg).val[0]/255); //(pFrame->width*pFrame->height);
			//cvCreateVideoWriter( const char* filename, int fourcc, double fps, CvSize frame_size, int is_color=1 );
			if(Mt>0)
			{
				int n=cvWriteFrame (vWriter, pFrame); //write to frame
				//cout<<n;
				if (Mt<100)
				{
					cvRunningAvg(pFrameMat, pBkMat, 0.05, 0);
				} 
				else
				{
					cvRunningAvg(pFrameMat, pBkMat, 0.8, 0);
				}
			}
			else
			{
				cvRunningAvg(pFrameMat, pBkMat, 0.05, 0);
				nFrmNum-=1;
			}


			/*CvScalar Mn;
			CvScalar SDev;
			cvAvgSdv(pFrMat,&Mn,&SDev);
			Mn.val[0];*/

			/*if ( SDev.val[0]<1)
			{
			cvRunningAvg(pFrameMat, pBkMat, 1, 0);
			} 
			else
			{

			}*/
			//renew background
			//cvRunningAvg(pFrameMat, pBkMat, 1, 0);
			cvConvert(pBkMat, pBkImg);


			//wait for key interupt
			if( cvWaitKey(2) >= 0 )
			{
				cvReleaseVideoWriter(&vWriter);
				cvReleaseImage(&pFrImg);
				cvReleaseImage(&pBkImg);
				cvReleaseMat(&pFrameMat);
				cvReleaseMat(&pFrMat);  
				cvReleaseMat(&pBkMat);
				
				cvReleaseCapture(&pCapture);
				std::cout<<"Frame number after deleting zero motion frames" <<nFrmNum<<std::endl;
				break;
			}
			//
			//TM+=Mt;

		}

	}
	//AM=TM/nFrmNum;

	//Release pointers
	cvReleaseVideoWriter(&vWriter);
	cvReleaseImage(&pFrImg);
	cvReleaseImage(&pBkImg);
	cvReleaseMat(&pFrameMat);
	cvReleaseMat(&pFrMat);  
	cvReleaseMat(&pBkMat);
	//print Mt and Tm
	cvReleaseCapture(&pCapture);
	//cout<<"Total Motion = "<<TM<<endl;
	//cout<<"Average Motion = "<<AM<<endl;
	std::cout<<"Frame number after deleting zero motion frames" <<nFrmNum<<std::endl;
	return;


}

detectMotion::~detectMotion()
{

}