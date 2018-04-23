#include "DetectionSystem.cpp"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int mainVideo() {
	VideoCapture cap("videoTest.avi");
	
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}

	cout << cap.get(5) << endl;
	int frameCounter = 0;
	while (1) {
		Mat frame;

		cap >> frame;

		if (frame.empty())
			break;

		if (frameCounter % 22 == 0) {
			imshow("Frame", frame);
			detectDefect(&frame);
			waitKey(0);
		}
		frameCounter++;
		
	}

	return 0;
}