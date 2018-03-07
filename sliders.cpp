#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

//const char* imagePointer = "LineDivot_Side.JPG";
//const char* imagePointer = "BumpDivot_Side.JPG";
const char* imagePointer = "Bump4.jpg";
Mat image = imread(imagePointer, IMREAD_COLOR);
Mat imageMedian;
Mat imageGray;
Mat imageEdges;
RNG rng(12345);
vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

int lowThreshold = 50;
int const maxThreshold = 100;

int lowRatio = 2;
int maxRatio = 4;

int lowSize = 0;
int maxSize = 300;

int kernelSize = 3;


static void CannyThreshold(int, void*) {
	blur(imageGray, imageEdges, Size(3, 3));
	Canny(imageEdges, imageEdges, lowThreshold, lowThreshold * lowRatio, kernelSize);
	findContours(imageEdges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(contours[i]);
	}
	Mat drawing = Mat::zeros(imageEdges.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		if (minRect[i].size.height > lowSize || minRect[i].size.width > lowSize )
			drawContours(drawing, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			if (minRect[i].size.height > lowSize || minRect[i].size.width > lowSize)
				line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}

	imshow("Defects", drawing);
}

int mainSlide() {
	medianBlur(image, imageMedian, 5);
	cvtColor(imageMedian, imageGray, COLOR_BGR2GRAY);

	namedWindow("Defects", WINDOW_FULLSCREEN);
	createTrackbar("Min Threshold: ", "Defects", &lowThreshold, maxThreshold, CannyThreshold);
	createTrackbar("Ratio: ", "Defects", &lowRatio, maxRatio, CannyThreshold);
	createTrackbar("Min Size: ", "Defects", &lowSize, maxSize, CannyThreshold);
	CannyThreshold(0, 0);

	waitKey(0);
	return 0;
}