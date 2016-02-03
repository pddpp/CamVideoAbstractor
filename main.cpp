#include "videoAbstract.hpp"
#include "Overlapper.hpp"
#include "detectMotion.hpp"
#include "FindConnectComponet.h"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h> 

int main(int argc, char ** argv)
{
	//create windows
	cvNamedWindow("video", 1);
	cvNamedWindow("foreground image",1);
	cvNamedWindow("background image",1);
	//windows alignment
	cvMoveWindow("video", 30, 0);
	cvMoveWindow("foreground image", 360, 0);
	cvMoveWindow("background image", 690, 0);
	
	//build gmm module basing on input video
	ModelBuilder builder(argv[1]);
	builder.ReadVideo();
	cv::BackgroundSubtractorMOG2 Model;
	for (;;)
	{
		int x=GetAsyncKeyState(VK_SPACE);
		if( x<0 )
		{
			break;
		}
		Model = builder.UpdateModel();
		imshow("video", builder.img);
		if(!builder.currentbgimg.empty())
			imshow("background image", builder.currentbgimg );

		char k = (char) cv::waitKey(30);
		if( k == 27 ) break;
	}

	//save the trained background module
	ForegroundSaver saver(Model, argv[1], "fgvideo.avi");	
	saver.ReadVideo();
	saver.WriteToFile();
	
	//display foreground images and tag objects with time stamp
	int counter= 0;
	int num = 0;	
	for (int Num = 0;;++Num)
	{
		
		char k = (char) cv::waitKey(30);
		if( k == 27 ) break;
		
		int x=GetAsyncKeyState(VK_TAB);
		if( x<0 )
		{
			num = Num;
			break;
		}
		saver.SubstractFG();
		
		imshow("video", saver.img);
		imshow("foreground image", saver.fgimg);

		counter += saver.AddTimeInfo(Num);

		imshow("foreground with tag", saver.txtimg);
		IplImage* ptxtimg =  &IplImage (saver.txtimg);
		cvWriteFrame (saver.Writer, ptxtimg);


	}
	std::cout<<"Total frame numbers" <<num<<std::endl;
	std::cout<<"foreground frame numbers" <<counter<<std::endl;

	cv::Mat background = builder.FinalBG();
	
	//video concentration with balanced number of objects per frame
	detectMotion cutter("fgvideo.avi", 60);

	videoOverlapper lap("ccfgvideo.avi", background, "result.avi");
	lap.OverlapForeground();

	return 1;
}
