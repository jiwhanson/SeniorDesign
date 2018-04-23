#ifndef IMAGESTREAM_S
#define IMAGESTREAM_S

//TODO: DEALLOCATE MAT POINTER

#include "ImageStream.h"
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <deque>

using namespace cv;
using namespace std;

static deque<string> stream;
static int stringNumber;
static VideoCapture cap;

static void initializeCameraStream() {
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cout << "Error opening video stream" << endl;
	}
}

static Mat* getNextCamera() {
	cout << "sending next img" << endl;
	++stringNumber;
	Mat nextFrame;
	cap.read(nextFrame);
	
	Mat* address = &(nextFrame.clone());
	return address;
}

static bool hasNextCamera() {
	cout << "has next is" << cap.isOpened() << endl;
	return cap.isOpened();
}

static void initializeStream() {
	//Initialize queue of strip phones
	
	//STREAM OF LINE DIVOT DEFECT
	//stream.push_back("LineDivot1.jpg");
	//stream.push_back("LineDivot2.jpg");
	//stream.push_back("LineDivot3.jpg");
	//stream.push_back("LineDivot4.jpg");
	
	//STREAM OF BUMP DIVOT DEFECT
	//stream.push_back("bumpDivot1.jpg");
	//stream.push_back("bumpDivot2.jpg");
	//stream.push_back("bumpDivot3.jpg");

	//STREAM OF BUMP DEFECT
	/*stream.push_back("Bump1.jpg");
	stream.push_back("Bump2.jpg");
	stream.push_back("Bump3.jpg");
	stream.push_back("Bump4.jpg");*/

	stream.push_back("BigLineDiag1.jpg");
	stream.push_back("BigLineDiag2.jpg");
	stream.push_back("BigLineDiag3.jpg");
	stream.push_back("BigLineVert1.jpg");
	stream.push_back("BigLineVert2.jpg");
	stream.push_back("BigLineVert3.jpg");
	stream.push_back("BumpBump1.jpg");
	stream.push_back("BumpBump2.jpg");
	stream.push_back("BumpBump3.jpg");
	stream.push_back("BumpScratchHoriz1.jpg");
	stream.push_back("BumpScratchHoriz2.jpg");
	stream.push_back("BumpScratchHoriz3.jpg");
	stream.push_back("LineDiag1.jpg");
	stream.push_back("LineDiag2.jpg");
	stream.push_back("LineDiag3.jpg");
	stream.push_back("LineHorz1.jpg");
	stream.push_back("LineHorz2.jpg");
	stream.push_back("LineHorz3.jpg");
	stream.push_back("LineVert1.jpg");
	stream.push_back("LineVert2.jpg");
	stream.push_back("LineVert3.jpg");
	
}
static bool hasNext() {
	if(stream.size() > 0)
		return true;
	return false;
}
static int getCurrentString() {
	//Return the string number of last sent photo
	return stringNumber;
}
static Mat* getNext() {
	//Increment string number
	++stringNumber;

	//DEBUGGING
	//cout << "sent string number " << getCurrentString();
	
	//Return the next photo in queue
	//DEALLOCATE THE MAT LATER
	Mat* image = new Mat(imread(stream.front(), IMREAD_COLOR));
	stream.pop_front();
	return image;
}


#endif