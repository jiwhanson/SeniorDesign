#include <sstream>
//#include "DetectionSystem.cpp"
// Include files to use OpenCV API.
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>
#include <pylon/PylonGUI.h>

// Use sstream to create image names including integer


using namespace Pylon;
using namespace GenApi;
using namespace cv;
using namespace std;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 1000;

static double momentAvg[] = { 0.0013625, 1.00638e-06, 3.66115e-13, 1.27802e-13, 4.69322e-26, 1.13918e-16, 1.82105e-26 };
static double momentStd[] = { 0.000171913, 4.65321e-07, 2.47537e-13, 1.75623e-13, 7.79158e-26, 1.53297e-16, 2.71854e-26 };

static double edgeMomentAvg[] = { 0.019802875, 0.000417456, 1.12741e-08, 1.13301e-08, 4.48183e-16, 2.83882e-10, 2.02036e-19 };
static double edgeMomentStd[] = { 0.005950644, 0.000218738, 2.05465e-08, 2.06981e-08, 8.93786e-16, 5.28261e-10, 3.99384e-19 };

static void displayImage(Mat* image) {
	string windowName = "String ";// + to_string(getCurrentString());
	namedWindow(windowName, WINDOW_AUTOSIZE);
	imshow(windowName, *image);
}

static bool compareMoments(double defectMoments[7]) {
	double momentStdDiff[7];
	double edgeDiff[7];
	double sum = 0;
	double edgeSum = 0;
	for (int i = 0; i < 7; i++) {
		momentStdDiff[i] = fabs((momentAvg[i] - defectMoments[i]) / momentStd[i]);
		sum += momentStdDiff[i];
		edgeDiff[i] = fabs((edgeMomentAvg[i] - defectMoments[i]) / edgeMomentStd[i]);
		edgeSum += edgeDiff[i];
		//cout << "Hu Moment Diff " << i << ": " << momentStdDiff[i] << endl;
		//cout << "Hu Edge Moment Diff " << i << ": " << edgeDiff[i] << endl;
	}
	cout << "Sum: " << sum << endl;
	cout << "Edge Sum: " << edgeSum << endl;
	cout << endl;

	//LINE DIVOT SETTINGS - CHANGE FOR OTHER DEFECTS
	if (sum >= 7500)
		//TEMPORTARY OTHER DEFECT SETTINGS - CHANGE FOR OTHER DEFECTS
		//BUMP SETTINGS - CHANGE FOR OTHER DEFECTS
		//if(sum >= 100000000)
		//BUMP DIVOT SETTINGS - CHANGE OR OTHER DEFECTS
		//if(sum >= 600)
		return true;
	return false;
}

static void detectDefect(Mat* image) {
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

	//BUMP DIVOT SETTINGS - CHANGE FOR OTHER DEFECT IMAGES
	//Canny(*image, *image, 55, 55 * 3, 3);

	//BUMP SETTINGS - CHANGE FOR OTHER DEFECT IMAGES
	//Canny(*image, *image, 57, 57 * 3, 3);

	//Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(*image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	//Defect size
	vector<RotatedRect> minRect(contours.size());
	for (size_t i = 0; i < contours.size(); i++) {
		//drawContours(*image, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
		//draws all contours on the displayed window
		drawContours(originalImage, contours, (int)i, Scalar(255, 255, 255), 1, 8, hierarchy, 0, Point());
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
		if (!isEdge) {
			Mat defectArea = Mat(originalImage, roi);
			Moments areaMoments = moments(defectArea, false);
			double huMoments[7];
			HuMoments(areaMoments, huMoments);
			for (int i = 0; i < 7; i++) {
				//cout << "Hu Moment " << i << ": " << huMoments[i] << endl;
			}
			cout << "Height Min: " << minRect[i].size.height << endl;
			cout << "Width Min: " << minRect[i].size.width << endl;
			cout << endl;
			//compareMoments returns true if moments vary enough to be labeled a defect
			if (compareMoments(huMoments)) {
				rectangle(originalImage, roi.tl(), roi.br(), Scalar(0, 255, 255), 1, 8, 0);
				//Point2f rect_points[4]; minRect[i].points(rect_points);
				//for (int j = 0; j < 4; j++) {
				//	line(originalImage, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 255, 255), 1, 8);
				//}
				//soundAlert();
			}
		}
	}
	displayImage(&originalImage);
	//namedWindow("Original", WINDOW_AUTOSIZE);
	//namedWindow("Processed", WINDOW_AUTOSIZE);
	//imshow("Original", originalImage);
	//imshow("Processed", *image);
}

int mainCamera()
{
	// Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
	// is initialized during the lifetime of this object.
	Pylon::PylonAutoInitTerm autoInitTerm;

	try
	{
		// Create an instant camera object with the camera device found first.
		cout << "Creating Camera..." << endl;
		CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

		// or use a device info object to use a specific camera
		//CDeviceInfo info;
		//info.SetSerialNumber("21694497");
		//CInstantCamera camera( CTlFactory::GetInstance().CreateFirstDevice(info));
		cout << "Camera Created." << endl;
		// Print the model name of the camera.
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

		// Start the grabbing of c_countOfImagesToGrab images.
		// The camera device is parameterized with a default configuration which
		// sets up free-running continuous acquisition.
		camera.StartGrabbing(c_countOfImagesToGrab);

		// This smart pointer will receive the grab result data.
		CGrabResultPtr ptrGrabResult;

		// Camera.StopGrabbing() is called automatically by the RetrieveResult() method
		// when c_countOfImagesToGrab images have been retrieved.
		while (camera.IsGrabbing())
		{
			// Wait for an image and then retrieve it. A timeout of 5000 ms is used.
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);

			// Image grabbed successfully?
			if (ptrGrabResult->GrabSucceeded())
			{
				// Access the image data.
				cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
				cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
				const uint8_t *pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
				cout << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0] << endl << endl;

				// Convert the grabbed buffer to pylon imag
				formatConverter.Convert(pylonImage, ptrGrabResult);
				// Create an OpenCV image out of pylon image
				openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *)pylonImage.GetBuffer());

				// Create a display window
				//namedWindow("OpenCV Display Window", CV_WINDOW_NORMAL);//AUTOSIZE //FREERATIO
																	   // Display the current image with opencv
				//imshow("OpenCV Display Window", openCvImage);

				detectDefect(&openCvImage);
				// Define a timeout for customer's input in ms.
				// '0' means indefinite, i.e. the next image will be displayed after closing the window 
				// '1' means live stream
				waitKey(0);

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
