#ifndef STRIPESMATCH_H
#define STRIPESMATCH_H

#include<iostream>
#include<vector>
#include<string>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/legacy/legacy.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"
//#include "opencv2/contrib/retina.hpp"

using namespace cv;
using namespace std;

#define SAVE_IMGS 1
#define PRINT_MSGS 1



typedef struct SphereAngles{
	float phi;
	float theta;
}sphereAngles;


typedef struct Match{
	sphereAngles sa1;
	sphereAngles sa2;
}match;



class StripesMatcher
{


public:

	StripesMatcher(int winSizex, int winSizey, int match_norm_method, int y_search_area, int fast_thresh);
	~StripesMatcher();

	void Init(double W, double h, double theta_min, double theta_max);

	void process(const Mat &in_image1, const Mat &in_image2, vector<match> &result);


private:
	int win_sizex;		///<Размер малого окна вокруг ключ. точки
	int win_sizey;		///<Размер малого окна вокруг ключ. точки
	int match_method;	///<Метод расчета MatchTemplate
	int yarea;			///<Размер увеличенной области поиска по y
	int fastThresh;		///<Порог для метода FAST

	double thetaMin;
	double thetaMax;

	int count;


	vector<Point> pt1;
	vector<Point> pt2;

};



#endif // STRIPESMATCH_H