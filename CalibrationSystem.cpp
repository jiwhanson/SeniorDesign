#ifndef CALIBRATIONSYSTEM_S
#define CALIBRATIONSYSTEM_S

#include "ImageStream.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

static void cdisplayImage(Mat* image) {
	string windowName = "String " + to_string(getCurrentString());
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, *image);
}

static void calibrateMoments(Mat* image, double hu[]) {
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
	Canny(*image, *image, 40, 40 * 3, 3);

	cdisplayImage(image);

	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(*image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Defect size
	vector<RotatedRect> minRect(contours.size());
	vector<float> cropTracker;
	for (size_t i = 0; i < contours.size(); i++) {
		//drawContours(*image, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		//draws all contours on the displayed window
		//drawContours(originalImage, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		//RotatedRect
		minRect[i] = minAreaRect(contours[i]);
		Rect roi = boundingRect(contours[i]);

		//Edge conditions:
		//Width: 80% of total image width
		//Height: less than 15px 
		//Angle: less than 2.5 degrees
		double angle;
		if (minRect[i].size.width < minRect[i].size.height) {
			angle = minRect[i].angle + 90;
		}
		else {
			angle = minRect[i].angle;
		}
		bool isEdge = (roi.width >= Mat(*image).cols * .50) && (roi.height <= 15) && (angle <= 2.5);
		if (isEdge) {
			//Push the y cooridnate of the edge to the vector
			cropTracker.push_back(minRect[i].center.y);
			cout << "pushed " << minRect[i].center.y << endl;
			}
	}
	//Get y coordinates of top edge and bottom edge of steep strip side
	//Margin of 10 px to be safe
	float cropTop = -1;
	float cropBottom = -1;
	if (cropTracker.size() != 0) {
		cropTop = *min_element(cropTracker.begin(), cropTracker.end());
		cropBottom = *max_element(cropTracker.begin(), cropTracker.end());
	}
	if (cropTop != -1 && cropBottom != -1) {
		cropTop += 10;
		cropBottom += -10;
	}
	else if (cropTop != -1) {
		cropTop += 10;
		cropBottom = cropTop + 150;
	}
	else if (cropBottom != -1) {
		cropBottom += -10;
		cropTop = cropBottom - 150;
	}
	else {
		cropTop = 300;
		cropBottom = 450;
	}

	//cout << "top" << cropTop << endl;
	//cout << "bottom" << cropBottom << endl;

	Point topLeft = Point(0, cropTop);
	Point bottomRight = Point(Mat(*image).cols, cropBottom);
	Rect roi = Rect(topLeft, bottomRight);
	cout << roi.width << endl;

	Mat defectArea = Mat(originalImage, roi);
	Moments areaMoments = moments(defectArea, false);
	double huMoments[7];
	HuMoments(areaMoments, huMoments);
	for (int i = 0; i < 7; i++) {
		hu[i] = fabs(huMoments[i]);
	}

	rectangle(originalImage, roi, Scalar(255, 255, 255), 1);
	cdisplayImage(&originalImage);
}

static double findAverage(vector<double> momentVec) {
	double sum = 0;
	for (int i = 0; i < momentVec.size(); i++) {
		sum += fabs(momentVec.at(i));
	}
	double average = double(sum / momentVec.size());
	return average;
}

static double findStdDev(vector<double> momentVec) {
	double average = findAverage(momentVec);
	double diffSum = 0;
	for (int i = 0; i < momentVec.size(); i++) {
		diffSum += pow((average - momentVec.at(i)), 2);
	}
	double deviation = sqrt(diffSum / (momentVec.size() - 1));
	return deviation;
}

int mainCalibrate() {
	vector<double> hu1, hu2, hu3, hu4, hu5, hu6, hu7;
	double hu[7];
	initializeStream();
	while (hasNext()) {
		Mat* nextImage = getNext();
		calibrateMoments(nextImage, hu);
		for (int i = 0; i < 7; i++) {
			//cout << "Hu Moment " << i << ": " << hu[i] << endl; 
		}
		hu1.push_back(hu[0]); hu2.push_back(hu[1]); hu3.push_back(hu[2]); hu4.push_back(hu[3]);
		hu5.push_back(hu[4]); hu6.push_back(hu[5]); hu7.push_back(hu[6]);

		cout << "Processed String Number " << getCurrentString() << "\n";
	}
	cout << "Hu Moment 1 Avg: " << findAverage(hu1) << endl;
	cout << "Hu Moment 1 Std Dev: " << findStdDev(hu1) << endl;
	cout << "Hu Moment 2 Avg: " << findAverage(hu2) << endl;
	cout << "Hu Moment 2 Std Dev: " << findStdDev(hu2) << endl;
	cout << "Hu Moment 3 Avg: " << findAverage(hu3) << endl;
	cout << "Hu Moment 3 Std Dev: " << findStdDev(hu3) << endl;
	cout << "Hu Moment 4 Avg: " << findAverage(hu4) << endl;
	cout << "Hu Moment 4 Std Dev: " << findStdDev(hu4) << endl;
	cout << "Hu Moment 5 Avg: " << findAverage(hu5) << endl;
	cout << "Hu Moment 5 Std Dev: " << findStdDev(hu5) << endl;
	cout << "Hu Moment 6 Avg: " << findAverage(hu6) << endl;
	cout << "Hu Moment 6 Std Dev: " << findStdDev(hu6) << endl;
	cout << "Hu Moment 7 Avg: " << findAverage(hu7) << endl;
	cout << "Hu Moment 7 Std Dev: " << findStdDev(hu7) << endl;

	waitKey(0);
	return(0);
}

#endif