#include "StripesMatcher.h"






StripesMatcher::StripesMatcher(int winSizex, int winSizey, int match_norm_method, int y_search_area, int fast_thresh)
{
	win_sizex = winSizex;
	win_sizey = winSizey;
	match_method = match_norm_method;
	yarea = y_search_area;
	fastThresh = fast_thresh;


	count = 1;

}


StripesMatcher::~StripesMatcher()
{


}

void StripesMatcher::Init(double W, double h, double theta_min, double theta_max){

	thetaMin = theta_min;
	thetaMax = theta_max;

}

void StripesMatcher::process(const Mat &in_image1, const Mat &in_image2, vector<match> &result)
{
	pt1.clear();
	pt2.clear();

	double t = (double)getTickCount();
	double t2;


	//medianBlur(image1, image1, 3);
	//medianBlur(image2, image2, 3);
	
	//cv::GaussianBlur(in_image1, in_image1, cv::Size(3, 3), 0);
	//cv::addWeighted(image1, 1.5, image1, -0.5, 0, image1);
	//cv::GaussianBlur(in_image2, in_image2, cv::Size(3, 3), 0);
	//cv::addWeighted(image2, 1.5, image2, -0.5, 0, image2);
	
	Mat image1;
	Mat image2;
	

	Mat img1;
	cvtColor(in_image1, img1, CV_BGR2GRAY);

	Mat img2;
	cvtColor(in_image2, img2, CV_BGR2GRAY);

	
	Ptr<CLAHE> clahe = createCLAHE();
	clahe->setClipLimit(6);
	clahe->setTilesGridSize(cv::Size(22, 22));

	clahe->apply(img1, image1);
	clahe->apply(img2, image2);

	cv::GaussianBlur(image1, image1, cv::Size(3, 3), 0);
	cv::GaussianBlur(image2, image2, cv::Size(3, 3), 0);
	
//	imwrite("img1.bmp", image1);
//	imwrite("img2.bmp", image2);

	t2 = ((double)getTickCount() - t) / getTickFrequency();
	std::cout << "filter [s]: " << t2 << std::endl;

	FastFeatureDetector detector(fastThresh, true);

	std::vector<KeyPoint> keypoints_1;

	detector.detect(img1, keypoints_1);

	double minVal; double maxVal; Point minLoc; Point maxLoc;

	
	for (vector<KeyPoint>::iterator k = keypoints_1.begin(); k != keypoints_1.end(); k++) //по всем точкам
	{

		if ((k->pt.x >= win_sizex) && (k->pt.y >= win_sizey) && (k->pt.y < image1.rows - win_sizey) && (k->pt.x < image1.cols - win_sizex)){


			Mat in1 = image1(Rect(cvRound(k->pt.x - win_sizex), cvRound(k->pt.y - win_sizey), win_sizex * 2 + 1, win_sizey * 2 + 1));

			int ysize = yarea*2 + 1;
			int y1 = cvRound(k->pt.y - yarea);
			if (y1 < 0) y1 = 0;
			if (y1 + ysize >= image2.rows) ysize = image2.rows - y1 - 1;

		
			Mat in2 = image2(Rect(cvRound(k->pt.x - win_sizex), y1, image2.cols - cvRound(k->pt.x - win_sizex), ysize));

			
			/// Create the result matrix
			Mat result;
			int result_cols = in2.cols - in1.cols + 1;
			int result_rows = in2.rows - in1.rows + 1;

			result.create(result_rows, result_cols, CV_32FC1);

			matchTemplate(in2, in1, result, match_method);
			//normalize(result, result, 0, 1, NORM_MINMAX, -1);

			/// Localizing the best match with minMaxLoc
			minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);

			if (minVal < 0.008) {
				pt1.push_back(Point(k->pt.x + image1.cols, k->pt.y));
				pt2.push_back(Point(minLoc.x + win_sizex + cvRound(k->pt.x - win_sizex), y1 + minLoc.y + win_sizey));
#if (PRINT_MSGS!= 0)
				printf("%.3f, %.3f res = %d, %d %.8f\n", k->pt.x + image1.cols, k->pt.y, minLoc.x + win_sizex + cvRound(k->pt.x), y1 + minLoc.y + win_sizey, minVal);
#endif
			}

		}//if

	}

#if (PRINT_MSGS!= 0)
	t2 = ((double)getTickCount() - t) / getTickFrequency();
	std::cout << "match time [s]: " << t2 << std::endl;
	printf("key size=%d, pt size=%d\n", (int)keypoints_1.size(), (int)pt1.size());
#endif


	t2 = ((double)getTickCount() - t) / getTickFrequency();
	std::cout << "all time [s]: " << t2 << std::endl;

	Mat res_img;
	res_img.create(image1.rows, image1.cols*2, image1.type());

	image2.copyTo(res_img(Rect(0, 0, image1.cols, image1.rows)));
	image1.copyTo(res_img(Rect(image1.cols, 0, image2.cols, image2.rows)));

	for (int i = 0; i < (int)pt1.size(); i++)
	{
		line(res_img, pt1[i], pt2[i], CV_RGB(255, 0, 0), 1, CV_AA, 0);

	}

	

#if (SAVE_IMGS!= 0)
	//imwrite("img_L1.bmp", image1);
	//imwrite("img_L2.bmp", image2);

	//imwrite("img_matches_e.bmp", res_img);
	char fbufr[64];
	sprintf_s(fbufr, 64, "img_matches%d.bmp", count++);
	imwrite(fbufr, res_img);
#endif



}
