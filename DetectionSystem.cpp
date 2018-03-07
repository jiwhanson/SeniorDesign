#ifndef DETECTIONSYSTEM_S
#define DETECTIONSYSTEM_S

#include "DetectionSystem.h"
#include "ImageStream.cpp"
#include "AlertSystem.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "math.h"

#include <iostream>

using namespace cv;
using namespace std;

//Database of non-defect moments
//Will calibrate for first 100 feet assuming no defects
static double momentAvg[] = { 0.0013625, 1.00638e-06, 3.66115e-13, 1.27802e-13, 4.69322e-26, 1.13918e-16, 1.82105e-26 };
static double momentStd[] = { 0.000171913, 4.65321e-07, 2.47537e-13, 1.75623e-13, 7.79158e-26, 1.53297e-16, 2.71854e-26 };

static void displayImage(Mat* image) {
	string windowName = "String " + to_string(getCurrentString());
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, *image);
}

/*	This function will compare the image moments a given defect area and
	compare it to the image moments of a nondefect area 

	Inputs:
	defectMoments - contain the 7 Hu Moments of the given defect area
	Outputs:
	true	- if categorized as a defect
	false	- if not categorized as a defect
	*/
static bool compareMoments(double defectMoments[7]) {
	double momentStdDiff[7];
	double sum = 0;
	for (int i = 0; i < 7; i++) {
		momentStdDiff[i] = fabs((momentAvg[i] - defectMoments[i]) / momentStd[i]);
		sum += momentStdDiff[i];
		cout << "Hu Moment Diff " << i << ": " << momentStdDiff[i] << endl;
	}
	cout << "Sum: " << sum << endl;
	cout << endl;

	//LINE DIVOT SETTINGS - CHANGE FOR OTHER DEFECTS
	//if (sum >= 7500)
	//TEMPORTARY OTHER DEFECT SETTINGS - CHANGE FOR OTHER DEFECTS
	//BUMP SETTINGS - CHANGE FOR OTHER DEFECTS
	if(sum >= 100000000)
		return true;
	return false;
}

static void detectDefect(Mat* image) {
	//Median blur
	medianBlur(*image, *image, 5);
	//Grayscale - LINE SCAN CAMERA IS GRAYSCALE AND THIS OPERATION WILL BE REMOVED
	cvtColor(*image, *image, COLOR_BGR2GRAY);

	Mat a = *image;		//IDK why but I need this for clone() to work
	Mat originalImage = a.clone();

	//Gaussian blur
	blur(*image, *image, Size(3, 3));

	//Canny threshold
	//LINE DIVOT SETTINGS - CHANGE FOR OTHER DEFECT IMAGES
	//Canny(*image, *image, 70, 70 * 3, 3);

	//BUMP DIVOT SETTINGS - CHANGE FOR OTHER DEFECT IMAGES
	Canny(*image, *image, 55, 55 * 3, 3);

	//BUMP SETTINGS - CHANGE FOR OTHER DEFECT IMAGES
	//Canny(*image, *image, 57, 57 * 3, 3);

	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(*image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Defect size
	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		drawContours(*image, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		//drawContours(originalImage, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		//RotatedRect
		minRect[i] = minAreaRect(contours[i]);
		Rect roi = boundingRect(contours[i]);

		//Assume metal strip edges will be one long continuous contour
		//TODO: set conditional to check for angle of defect (assumes strip will be straight horizontally)
		//LINE DIVOT SETTINGS - CHANGE FOR OTHER DEFECTS
		//if (minRect[i].size.height < 380 && minRect[i].size.width < 380) {
		//BUMP DIVOT SETTINGS - CHANGE FOR OTHER DEFECTS
		//if (minRect[i].size.height < 430 && minRect[i].size.width < 430) {
		//BUMP SETTINGS - CHANGE FOR OTHER DEFECTS
		int tempWidth = max(minRect[i].size.width, minRect[i].size.height);
		int tempHeight = min(minRect[i].size.width, minRect[i].size.height);
		if (tempWidth < 280 || tempHeight > 10) {
			Mat defectArea = Mat(originalImage, roi);
			Moments areaMoments = moments(defectArea, false);
			double huMoments[7];
			HuMoments(areaMoments, huMoments);
			for (int i = 0; i < 7; i++) {
				//cout << "Hu Moment " << i << ": " << huMoments[i] << endl;
			}
			cout << roi.height << endl;
			cout << endl;
			//compareMoments returns true if moments vary enough to be labeled a defect
			if (compareMoments(huMoments)) {
				rectangle(originalImage, roi.tl(), roi.br(), Scalar(0, 255, 255), 1, 8, 0);
				//Point2f rect_points[4]; minRect[i].points(rect_points);
				//for (int j = 0; j < 4; j++) {
				//	line(originalImage, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 255, 255), 1, 8);
				//}
				//soundAlert();
			}
		}
	}
	displayImage(&originalImage);
}
#endif