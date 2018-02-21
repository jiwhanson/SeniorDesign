#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;

//const char* image = "stripImage.jpg";
const char* image = "stream3.jpg";
//const char* image = "BumpDivot_Side.JPG";

int mainSingle() {
	//ORIGINAL IMAGE
	Mat stripImage;
	stripImage = imread(image, IMREAD_COLOR);
	namedWindow(image, WINDOW_AUTOSIZE);
	imshow(image, stripImage);

	//5x5 MEDIAN FILTER
	Mat stripImageFiltered;
	medianBlur(stripImage, stripImageFiltered, 5);
	//namedWindow("Median Filter", WINDOW_AUTOSIZE);
	//imshow("Median Filter", stripImageFiltered);

	//GRAYSCALE ALGORITHM FROM "AN ONLINE INSPECTION SYSTEM..."
	//Same as default RGB -> Gray Algo
	Mat stripImageGray;
	cvtColor(stripImageFiltered, stripImageGray, COLOR_BGR2GRAY);
	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", stripImageGray);

	Mat contrasted;
	equalizeHist(stripImageGray, contrasted);
	namedWindow("contrast", WINDOW_AUTOSIZE);
	imshow("contrast", contrasted);


	//CANNY THRESHOLD
	int lowThreshold = 70; //Change Later
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
	
	Mat detectedEdges;
	Mat stripImageEdges;

	//Apply Gaussian Blur
	//blur(stripImageGray, detectedEdges, Size(3,3));
	blur(contrasted, detectedEdges, Size(3, 3));
	//Apply Canny
	Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold * ratio, kernel_size);

	stripImageEdges = Scalar::all(0);
	
	//Copy original image to stripImageEdges, using detectedEdges as a mask
	stripImageFiltered.copyTo(stripImageEdges, detectedEdges);

	namedWindow("Canny Threshold", WINDOW_AUTOSIZE);
	//imshow("Canny Threshold", stripImageEdges);
	
	//FIND CONTOURS & BOUND THEM
	RNG rng(12345);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(detectedEdges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	vector<RotatedRect> minRect(contours.size());
	vector<RotatedRect> minEllipse(contours.size());


	for (size_t i = 0; i < contours.size(); i++) {
		minRect[i] = minAreaRect(contours[i]);
		if (contours[i].size() > 5) {
			//does some weird assuming point on a ellipse algorithm
			//minEllipse[i] = fitEllipse(contours[i]);
			//ellipse within the min area rect
			minEllipse[i] = minAreaRect(contours[i]);
		}
	}

	Mat contourBounds = Mat::zeros(detectedEdges.size(), CV_8UC3);
	Mat contourBoundsFiltered = Mat::zeros(detectedEdges.size(), CV_8UC3);

	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		
		//Draw contours
		drawContours(contourBounds, contours, (int)i, Scalar(255,255,255), 1, 8, hierarchy, 0, Point());
		if(minRect[i].size.height > 20 || minRect[i].size.width > 20)
			drawContours(contourBoundsFiltered, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());

		Point2f rect_points[4]; minRect[i].points(rect_points);
		/*for (int j = 0; j < 4; j++) {
			line(contourBounds, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
			//Dont draw box around stuff that is too small
			if (minRect[i].size.height > 20 || minRect[i].size.width > 20)
				line(contourBoundsFiltered, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
		}*/
	}
	

	namedWindow("Filtered", WINDOW_AUTOSIZE);
	imshow("Filtered", contourBoundsFiltered);

	for (int i = 0; i < contours.size(); i++) {
		Rect roi = boundingRect(contours[i]);
		Scalar meanValueRect = mean(stripImageGray(roi)/*, detectedEdges(roi)*/);
		Scalar meanValueContour = mean(stripImageGray(roi), detectedEdges(roi));
		/*
		if (meanValueRect[0] >= 170 && meanValueRect[0] <= 200) {
			rectangle(contourBounds, roi.tl(), roi.br(), Scalar(255, 255, 255), 2, 8, 0);
		}

		if (meanValueContour[0] >= 170 && meanValueRect[0] <= 210) {
			rectangle(contourBounds, roi.tl(), roi.br(), Scalar(0, 255, 0), 1, 8, 0);
		}*/
		

		cout << "Countour: " << i << endl;
		cout << "Height: " << minRect[i].size.height << endl;
		cout << "Width: " << minRect[i].size.width << endl;
		cout << "Mean Rect: " << meanValueRect[0] << endl;
		cout << "Mean Cont: " << meanValueContour[0] << "\n\n";
	}

	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", contourBounds);

	waitKey(0);
	return(0);
}
/*
blur(stripImageGray, detectedEdges, Size(3, 3));
Canny(detectedEdges, detectedEdges, 40, 40 * 3, kernel_size);
stripImageEdges = Scalar::all(0);
stripImageFiltered.copyTo(stripImageEdges, detectedEdges);
namedWindow("Canny Threshold 40", WINDOW_AUTOSIZE);
imshow("Canny Threshold 40", stripImageEdges);
*/