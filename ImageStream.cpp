#ifndef IMAGESTREAM_S
#define IMAGESTREAM_S

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

static void initializeStream() {
	//Initialize queue of strip phones
	stream.push_back("stream1.jpg");
	stream.push_back("stream2.jpg");
	stream.push_back("stream3.jpg");
	stream.push_back("stream4.jpg");
	stream.push_back("stream5.jpg");
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
	//deallocate the mat later
	Mat* image = new Mat(imread(stream.front(), IMREAD_COLOR));
	stream.pop_front();
	return image;
}


#endif