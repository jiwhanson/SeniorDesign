#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "chrono"

using namespace std;
using namespace std::chrono;
using namespace cv;

const char* ptrImage100 = "imageTest100.jpg";
const char* ptrImage200 = "imageTest200.jpg";
const char* ptrImage400 = "imageTest400.jpg";
const char* ptrImage800 = "imageTest800.jpg";
const char* ptrImage1600 = "imageTest1600.jpg";

static void thresholdProcess(char* imagePointer) {
	auto start = high_resolution_clock::now();

	Mat image = imread(imagePointer, IMREAD_COLOR);
	Mat imageMedian;
	Mat imageGray;
	Mat imageEdges;
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	medianBlur(image, imageMedian, 5);
	cvtColor(imageMedian, imageGray, COLOR_BGR2GRAY);
	blur(imageGray, imageEdges, Size(3, 3));
	Canny(imageEdges, imageEdges, 80, 80 * 3, 3);
	findContours(imageEdges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(contours[i]);
	}

	auto stop = high_resolution_clock::now();
	int rows = image.rows;
	int columns = image.cols;
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken on "<< rows << "x" << columns << " px image: "
		<< duration.count() << " microseconds" << endl;
	cout << "Countours detected: " << contours.size() << "\n\n";

	Mat drawing = Mat::zeros(imageEdges.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, (int)i, Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}

	namedWindow(imagePointer, WINDOW_AUTOSIZE);
	imshow(imagePointer, drawing);
	/*for (int i = 0; i < contours.size(); i++) {
		//Size2f rectSize = minRect[i].size;
		cout << "\nCountour: " << i << "\n";
		cout << "Height: " << minRect[i].size.height << "\n";
		cout << "Width: " << minRect[i].size.width << "\n\n";
	}
	*/
}

int mainTime() {
	thresholdProcess("imageTest100.jpg");
	thresholdProcess("imageTest200.jpg");
	thresholdProcess("imageTest400.jpg");
	thresholdProcess("imageTest800.jpg");
	thresholdProcess("imageTest1600.jpg");
	thresholdProcess("imageTest.jpg");
	
	waitKey(0);
	return(0);
}