#ifndef DETECTIONSYSTEM_S
#define DETECTIONSYSTEM_S

#include "DetectionSystem.h"
#include "ImageStream.cpp"
#include "AlertSystem.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;


static void detectDefect(Mat* image) {
	//Median blur
	medianBlur(*image, *image, 5);
	//Grayscale
	cvtColor(*image, *image, COLOR_BGR2GRAY);
	//Gaussian blur
	blur(*image, *image, Size(3, 3));
	//Canny threshold
	Canny(*image, *image, 70, 70 * 3, 3);
	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(*image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Defect size
	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(*image, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		minRect[i] = minAreaRect(contours[i]);
		//Set a limit based on 400 pixel width
		//Assume metal strip edges will be one long continuous contour
		//TODO: set conditional to check for angle of defect (assumes strip will be straight horizontally)
		if (minRect[i].size.height < 380 && minRect[i].size.width < 380) {
			Point2f rect_points[4]; minRect[i].points(rect_points);
			for (int j = 0; j < 4; j++)
				line(*image, rect_points[j], rect_points[(j + 1) % 4], Scalar(255, 255, 255), 1, 8);
			soundAlert();
		}
	}

}

static void displayImage(Mat* image) {
	string windowName = "String " + to_string(getCurrentString());
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, *image);
}

#endif