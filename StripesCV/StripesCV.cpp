// StripesCV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "StripesMatcher.h"



using namespace cv;
using namespace std;

//1 - CV_TM_SQDIFF_NORMED


int _tmain(int argc, _TCHAR* argv[])
{


	StripesMatcher StrMatch(5, 11, 1, 22, 7);
	StrMatch.Init(0, 0, 80.0, 93.0);
	
	vector <match> match_str;
	cv::Mat image[2];
	image[0] = cv::imread("files\\1_1.jpg", 1);   // Read the file
	image[1] = cv::imread("files\\1_2.jpg", 1);   // Read the file
	StrMatch.process(image[0], image[1], match_str);
	
	/*
	//MULTIPLE TEST
	for (int i = 0; i < 10; i++) {

		vector <match> match_str;
		cv::Mat image[2];
		
		char fbuf[64];
		sprintf_s(fbuf, 64, "files\\%d_1.jpg", i + 1);
		char fbuf2[64];
		sprintf_s(fbuf2, 64, "files\\%d_2.jpg", i + 1);

		image[0] = cv::imread(fbuf, 1);
		image[1] = cv::imread(fbuf2, 1);

		StrMatch.process(image[0], image[1], match_str);

	}
*/



	return 0;
}

