/*#include <opencv2/opencv.hpp>
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "chrono"

using namespace std;
using namespace std::chrono;
using namespace cv;

//Initialize resources for optimization
struct initializedResources {
	Mat image, imageOriginal, defectArea;
	gpu::GpuMat imageGpu;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RotatedRect minRect;
	Rect roi;
	Moments areaMoments;
	double huMoments[7], momentStdDiff[7];
	double sum;
};

//Possibly optimize this by using matricies to do the operations
static bool dummyHuCompare(double defectMoments[7], initializedResources& R) {
	R.sum = 0;
	for (int i = 0; i < 7; i++) {
		R.momentStdDiff[i] = fabs((0 - defectMoments[i]) / 1);
		R.sum += R.momentStdDiff[i];
	}
	return true;
}

static void thresholdProcess(char* imagePointer, initializedResources& R) {
	auto start = high_resolution_clock::now();

	//RESOURCES HERE
	//Mat image is used to manipulate
	//Mat imageOriginal is used to calculate moments
	R.image = imread(imagePointer, IMREAD_COLOR);
	R.imageOriginal = imread(imagePointer, IMREAD_COLOR);

	//RNG rng(12345);
	

	//Mat image converted to GpuMat
	R.imageGpu.upload(R.image);

	//IDK where the median filter is
	
	gpu::cvtColor(R.imageGpu, R.imageGpu, COLOR_BGR2GRAY);
	gpu::GaussianBlur(R.imageGpu, R.imageGpu, Size(3,3));
	gpu::Canny(R.imageGpu, R.imageGpu, 70, 70 * 3, 3);

	R.imageGpu.download(R.image);

	findContours(R.image, R.contours, R.hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	for (size_t i = 0; i < R.contours.size(); i++) {
		//RESOURCES
		R.minRect = minAreaRect(R.contours[i]);
		R.roi = boundingRect(R.contours[i]);
		R.defectArea = Mat(R.imageOriginal, R.roi);
		R.areaMoments = moments(R.defectArea, false);
		HuMoments(R.areaMoments, R.huMoments);
		if (dummyHuCompare(R.huMoments, R)) {
			int a = 0;
			//cout << "compared moment" << endl;
		}
	}

	auto stop = high_resolution_clock::now();
	int rows = R.image.rows;
	int columns = R.image.cols;
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken on " << rows << "x" << columns << " px image: "
		<< duration.count() << " microseconds" << endl;
	cout << "Countours detected: " << R.contours.size() << "\n\n";

	/*
	Mat drawing = Mat::zeros(image.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		Point2f rect_points[4]; minRect[i].points(rect_points);
		for (int j = 0; j < 4; j++)
			line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
	}
	

	//namedWindow(imagePointer, WINDOW_AUTOSIZE);
	//imshow(imagePointer, drawing);
}

int mainGpu() {
	initializedResources Resources;

	thresholdProcess("imageTest100.jpg", Resources);
	thresholdProcess("imageTest200.jpg", Resources);
	thresholdProcess("imageTest400.jpg", Resources);
	thresholdProcess("imageTest800.jpg", Resources);
	thresholdProcess("imageTest1600.jpg", Resources);
	thresholdProcess("imageTest3200.jpg", Resources);
	//thresholdProcess("LineDivot_Side.jpg");
	//thresholdProcess("BumpDivot_Side.jpg");
	//thresholdProcess("Bumps_Side2.jpg");

	waitKey(0);
	return(0);
}
*/