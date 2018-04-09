#include "ImageStream.cpp"
#include "DetectionSystem.cpp"
#include "AlertSystem.cpp"
//#include "CalibrationSystem.cpp"



int mainMain() {
	//For testing, static stream of images
	
	

	/*initializeStream();
	while (hasNext()) {
		Mat* nextImage = getNext();
		detectDefect(nextImage);
		cout << "Processed String Number " << getCurrentString() << "\n";
	}
	waitKey(0);
	return(0);*/

	
	/*
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	}
	cout << "Initialized camera stream" << endl;

	//initializeCameraStream();
	//cout << "initialized camera stream" << endl;

	while (1) {
		Mat nextImage;
		cap >> nextImage;
		detectDefect(&nextImage);
		cout << "Processed String Number " << getCurrentString() << "\n";
		waitKey(0);
	}
	*/

	waitKey(0);
	return(0);
}
