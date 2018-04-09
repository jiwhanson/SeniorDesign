#ifndef DEFECTSELECTIONSYSTEM_S
#define DEFECTSELECTIONSYSTEM_S

#include "ImageStream.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

static double momentAvg[] = { 0.0013625, 1.00638e-06, 3.66115e-13, 1.27802e-13, 4.69322e-26, 1.13918e-16, 1.82105e-26 };
static double momentStd[] = { 0.000171913, 4.65321e-07, 2.47537e-13, 1.75623e-13, 7.79158e-26, 1.53297e-16, 2.71854e-26 };

static void calculateDev(double defectMoments[7]) {
	double momentStdDiff[7];
	for (int i = 0; i < 7; i++) {
		momentStdDiff[i] = fabs((momentAvg[i] - defectMoments[i]) / momentStd[i]);
	}
	cout << momentStdDiff[0] << "\t" << momentStdDiff[1] << "\t" << momentStdDiff[2] << "\t" << momentStdDiff[3] << "\t" 
		<< momentStdDiff[4] << "\t" << momentStdDiff[5] << "\t" << momentStdDiff[6] << "\t";
}

static void selectDefects(Mat* image) {
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
	Canny(*image, *image, 65, 65 * 3, 3);

	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(*image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<RotatedRect> minRect(contours.size());
	namedWindow("Defect Selection", WINDOW_AUTOSIZE);
	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(contours[i]);
		Rect roi = boundingRect(contours[i]);
		double angle;
		if (minRect[i].size.width < minRect[i].size.height) {
			angle = minRect[i].angle + 90;
		}
		else {
			angle = minRect[i].angle;
		}
		bool isEdge = (roi.width >= Mat(*image).cols * .50) && (roi.height <= 15) && (angle <= 2.5);
		if (!isEdge) {
			drawContours(originalImage, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
			rectangle(originalImage, roi.tl(), roi.br(), Scalar(0, 255, 255), 1, 8, 0);
			imshow("Defect Selection", originalImage);
			Mat defectArea = Mat(originalImage, roi);
			Moments areaMoments = moments(defectArea, false);
			double huMoments[7];
			HuMoments(areaMoments, huMoments);
			calculateDev(huMoments);
			char selection = (char)waitKey(0);
			if (selection == 'y') {
				cout << "Y\n";
			}
			else {
				cout << "N\n";
			}
		}
	}
}

int mainSelect() {
	initializeStream();
	while (hasNext()) {
		Mat* nextImage = getNext();
		selectDefects(nextImage);
	}

	waitKey(0);
	return(0);
}
#endif