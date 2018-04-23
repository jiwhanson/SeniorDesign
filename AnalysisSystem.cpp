#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>
#include <math.h>
#include <iostream>
#include "chrono"

using namespace Pylon;
using namespace GenApi;
using namespace cv;
using namespace std;
using namespace std::chrono;

static vector<double> hu1, hu2, hu3, hu4, hu5, hu6, hu7;
static double hu[7];
static double momentAvg[7], momentStd[7];

int lowThreshold = 30;
int maxThreshold = 100;
int lowRatio = 20;
int maxRatio = 40;

static void displayImage(Mat* image, char* name) {
	string windowName = name;
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, *image);
}

static void calibrateMoments(Mat* image, double hu[]) {
	//Median blur
	medianBlur(*image, *image, 11);
	Mat a = *image;		//IDK why but I need this for clone() to work
	Mat originalImage = a.clone();
	Mat edges = a.clone();
	//Gaussian blur
	blur(*image, *image, Size(3, 3));
	//Canny threshold
	Canny(*image, edges/**image*/, lowThreshold, lowThreshold * 3/*double(lowRatio/10.0)*/, 3);
	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edges/**image*/, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//Defect size
	vector<float> cropTracker;
	for (size_t i = 0; i < contours.size(); i++) {
		RotatedRect minRect = minAreaRect(contours[i]);
		Rect roi = boundingRect(contours[i]);
		//Edge conditions:
		//Width: 80% of total image width
		//Height: less than 15px 
		//Angle: less than 2.5 degrees
		double angle;
		if (minRect.size.width < minRect.size.height) {
			angle = minRect.angle + 90;
		}
		else {
			angle = minRect.angle;
		}
		bool isEdge = (roi.width >= Mat(*image).cols * .50) && (roi.height <= 15) && (angle <= 2.5);
		if (isEdge) {
			//Push the y cooridnate of the edge to the vector
			cropTracker.push_back(minRect.center.y);
			cout << "pushed " << minRect.center.y << endl;
		}
	}
	//Get y coordinates of top edge and bottom edge of steep strip side
	//Margin of 10 px to be safe
	float cropTop;
	float cropBottom;
	if (cropTracker.size() > 1) {
		//10px Margin
		cropTop = *min_element(cropTracker.begin(), cropTracker.end());
		cropTop += 10;
		cropBottom = *max_element(cropTracker.begin(), cropTracker.end());
		cropBottom -= 10;
		//Determine area of interest
		Point topLeft = Point(0, cropTop);
		Point bottomRight = Point(Mat(*image).cols, cropBottom);
		Rect roi = Rect(topLeft, bottomRight);
		cout << roi.width << endl;
		//Calculate moments from area of interest
		Mat defectArea = Mat(originalImage, roi);
		Moments areaMoments = moments(defectArea, false);
		double huMoments[7];
		HuMoments(areaMoments, huMoments);
		for (int i = 0; i < 7; i++) {
			hu[i] = huMoments[i];
		}
		hu1.push_back(hu[0]); hu2.push_back(hu[1]); hu3.push_back(hu[2]); hu4.push_back(hu[3]);
		hu5.push_back(hu[4]); hu6.push_back(hu[5]); hu7.push_back(hu[6]);
		//Draw area of interest onto edge map
		//rectangle(*image, roi, Scalar(255, 255, 255), 1);
	}
	//Display edges and calibration area to Canny Calibration window
	imshow("Canny Calibration", edges);
	waitKey(1);
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

static bool compareMoments(double defectMoments[7]) {
	double momentStdDiff[7];
	for (int i = 0; i < 7; i++) {
		momentStdDiff[i] = fabs((momentAvg[i] - defectMoments[i]) / momentStd[i]);
		//cout << "Hu Moment Diff " << i << ": " << momentStdDiff[i] << endl;
	}
	//Basic J48 Classification tree based on training set
	if (momentStdDiff[0] <= 2.78291) {
		return true;
	}
	return false;
}

//TODO:
//Add code for sounding alert: saving image, etc
static void soundAlert() {
	
}

static void detectDefect(Mat* image) {
	//Median blur
	medianBlur(*image, *image, 11);
	Mat a = *image;		//IDK why but I need this for clone() to work
	Mat originalImage = a.clone();
	Mat edges;
	//Gaussian blur
	blur(*image, *image, Size(3, 3));
	//Canny threshold
	Canny(*image, edges/**image*/, lowThreshold, lowThreshold * double(lowRatio / 10.0), 3);
	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edges/**image*/, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//Defect size
	for (size_t i = 0; i < contours.size(); i++) {
		//RotatedRect
		RotatedRect minRect = minAreaRect(contours[i]);
		Rect roi = boundingRect(contours[i]);
		//Edge conditions:
		//Width: 80% of total image width
		//Height: less than 15px
		//Angle: less than 2.5 degrees
		double angle;
		if (minRect.size.width < minRect.size.height) {
			angle = minRect.angle + 90;
		}
		else {
			angle = minRect.angle;
		}
		bool isEdge = (roi.width >= Mat(*image).cols * .50) && (roi.height <= 15) && (angle <= 2.5);
		if (!isEdge) {
			Mat defectArea = Mat(originalImage, roi);
			Moments areaMoments = moments(defectArea, false);
			double huMoments[7];
			HuMoments(areaMoments, huMoments);
			//compareMoments returns true if moments vary enough to be labeled a defect
			if (compareMoments(huMoments)) {
				//rectangle(originalImage, roi.tl(), roi.br(), Scalar(0, 255, 255), 1, 8, 0);
				soundAlert();
			}
		}
	}
}

int main()
{
	// Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
	// is initialized during the lifetime of this object.
	Pylon::PylonAutoInitTerm autoInitTerm;

	try
	{
		// Create an instant camera object with the camera device found first.
		cout << "Creating Camera..." << endl;
		CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());
		cout << "Camera Created." << endl;
		cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

		// The parameter MaxNumBuffer can be used to control the count of buffers
		// allocated for grabbing. The default value of this parameter is 10.
		camera.MaxNumBuffer = 10;

		// create pylon image format converter and pylon image
		CImageFormatConverter formatConverter;
		//Specify the output pixel format
		formatConverter.OutputPixelFormat = PixelType_BGR8packed;
		//Create a PylonImage that will be used to create OpenCV images later.
		CPylonImage pylonImage;

		// Create an OpenCV image
		Mat openCvImage;

		//Create live window for calibration
		namedWindow("Canny Calibration", WINDOW_AUTOSIZE);
		createTrackbar("Min Threshold: ", "Canny Calibration", &lowThreshold, maxThreshold);
		createTrackbar("Ratio: ", "Canny Calibration", &lowRatio, maxRatio);
		waitKey(0);
		cout << "Starting aquisition" << endl;

		// Start the grabbing of c_countOfImagesToGrab images.
		// The camera device is parameterized with a default configuration which
		// sets up free-running continuous acquisition.
		//camera.StartGrabbing(c_countOfImagesToGrab);
		camera.StartGrabbing();

		// This smart pointer will receive the grab result data.
		CGrabResultPtr ptrGrabResult;

		// Camera.StopGrabbing() is called automatically by the RetrieveResult() method
		// when c_countOfImagesToGrab images have been retrieved.
		//Calibration for ~30 seconds
		auto start = high_resolution_clock::now();
		while (camera.IsGrabbing()) {
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
			if (ptrGrabResult->GrabSucceeded()) {
				// Convert the grabbed buffer to pylon image
				formatConverter.Convert(pylonImage, ptrGrabResult);
				// Create an OpenCV image out of pylon image
				openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());
				//Rotate image from camera to be horizontal
				rotate(openCvImage, openCvImage, ROTATE_90_COUNTERCLOCKWISE);
				calibrateMoments(&openCvImage, hu);
			}
			else
			{
				cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
				waitKey(0);
			}

			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			//Break calibration loop after 30 seconds/ 10 seconds for testing rn
			if (duration.count() > 10000000) {
				break;
			}
		}
		momentAvg[0] = findAverage(hu1); momentAvg[1] = findAverage(hu2); momentAvg[2] = findAverage(hu3); momentAvg[3] = findAverage(hu4);
		momentAvg[4] = findAverage(hu5); momentAvg[5] = findAverage(hu6); momentAvg[6] = findAverage(hu7);
		momentStd[0] = findStdDev(hu1); momentStd[1] = findStdDev(hu2); momentStd[2] = findStdDev(hu3); momentStd[3] = findStdDev(hu4);
		momentStd[4] = findStdDev(hu5); momentStd[5] = findStdDev(hu6); momentStd[6] = findStdDev(hu7);

		cout << "Calibration Finished" << endl;
		destroyWindow("Canny Calibration");
		cout << "Beginning Detection..." << endl;

		while (camera.IsGrabbing())
		{
			// Wait for an image and then retrieve it. A timeout of 5000 ms is used.
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				// Convert the grabbed buffer to pylon imag
				formatConverter.Convert(pylonImage, ptrGrabResult);
				// Create an OpenCV image out of pylon image
				openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());
				//Rotate to be horizontal
				rotate(openCvImage, openCvImage, ROTATE_90_COUNTERCLOCKWISE);
				detectDefect(&openCvImage);
				// Define a timeout for customer's input in ms.
				// '0' means indefinite, i.e. the next image will be displayed after closing the window 
				// '1' means live stream
				//waitKey(0);
			}
			else
			{
				cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
			}
		}
	}
	catch (GenICam::GenericException &e)
	{
		// Error handling.
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
		return 0;
	}

	// Comment the following two lines to disable waiting on exit.
	cerr << endl << "Press Enter to exit." << endl;
	while (cin.get() != '\n');

	return 0;
}
