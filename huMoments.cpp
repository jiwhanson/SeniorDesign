#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

char* lineDivot = "LineDivot_SideMoment.jpg";
char* bumpDivot = "BumpDivot_SideMoment.jpg";
char* bump = "Bumps_SideMoment.jpg";
char* scratch = "ScratchMoment.jpg";
char* noDefect = "NoDefectMoment.jpg";
char* noDefect2 = "NoDefectMoment2.jpg";
char* noDefect3 = "NoDefectMoment3.jpg";
char* noDefect4 = "NoDefectMoment4.jpg";
char* noDefect5 = "NoDefectMoment5.jpg";
char* noDefect6 = "NoDefectMoment6.jpg";
char* noDefect7 = "NoDefectMoment7.jpg";

static void findHuMoments(char* imagePointer) {
	Mat image = imread(imagePointer, IMREAD_COLOR);
	medianBlur(image, image, 5);
	cvtColor(image, image, COLOR_BGR2GRAY);
	Moments Moments = moments(image, false);
	double huMoments[7];
	HuMoments(Moments, huMoments);
	for (int i = 0; i < 7; i++) {
		cout /*<< "Hu Moment " << i << ": "*/ << huMoments[i] << endl;
	}
	cout << endl;
}

int main() {

	
	/*Mat noDefectImg = imread(noDefect, IMREAD_COLOR);
	medianBlur(noDefectImg, noDefectImg, 5);
	cvtColor(noDefectImg, noDefectImg, COLOR_BGR2GRAY);
	Moments noDefectMoments = moments(noDefectImg, false);
	double huNoDefect[7];
	HuMoments(noDefectMoments, huNoDefect);
	cout << "No Defect Moments" << endl;
	for (int i = 0; i < 7; i++) {*/
	//	cout /*<< "Hu Moment " << i << ": "*/ << huNoDefect[i] << endl;
	//}
	//cout << endl;
	
	/*
	findHuMoments(noDefect2);
	findHuMoments(noDefect3);
	findHuMoments(noDefect4);
	findHuMoments(noDefect5);
	findHuMoments(noDefect6);
	findHuMoments(noDefect7);
	*/

	findHuMoments(scratch);
}

/*
Mat lineDivotImg = imread(lineDivot, IMREAD_COLOR);
medianBlur(lineDivotImg, lineDivotImg, 5);
cvtColor(lineDivotImg, lineDivotImg, COLOR_BGR2GRAY);
Moments lineDivotMoments = moments(lineDivotImg, false);
double huLineDivot[7];
HuMoments(lineDivotMoments, huLineDivot);
cout << "Line Divot Moments" << endl;
for (int i = 0; i < 7; i++) {
cout << "Hu Moment " << i << ": " << huLineDivot[i] << endl;
}
cout << endl;


Mat bumpDivotImg = imread(bumpDivot, IMREAD_COLOR);
medianBlur(bumpDivotImg, bumpDivotImg, 5);
cvtColor(bumpDivotImg, bumpDivotImg, COLOR_BGR2GRAY);
Moments bumpDivotMoments = moments(bumpDivotImg, false);
double huBumpDivot[7];
HuMoments(bumpDivotMoments, huBumpDivot);
cout << "Bump Divot Moments" << endl;
for (int i = 0; i < 7; i++) {
cout << "Hu Moment " << i << ": " << huBumpDivot[i] << endl;
}
cout << endl;

Mat bumpImg = imread(bump, IMREAD_COLOR);
medianBlur(bumpImg, bumpImg, 5);
cvtColor(bumpImg, bumpImg, COLOR_BGR2GRAY);
Moments bumpMoments = moments(bumpImg, false);
double huBump[7];
HuMoments(bumpMoments, huBump);
cout << "Bump Moments" << endl;
for (int i = 0; i < 7; i++) {
cout << "Hu Moment " << i << ": " << huBump[i] << endl;
}
cout << endl;

Mat scratchImg = imread(scratch, IMREAD_COLOR);
medianBlur(scratchImg, scratchImg, 5);
cvtColor(scratchImg, scratchImg, COLOR_BGR2GRAY);
Moments scratchMoments = moments(scratchImg, false);
double huScratch[7];
HuMoments(scratchMoments, huScratch);
cout << "Scratch Moments" << endl;
for (int i = 0; i < 7; i++) {
cout << "Hu Moment " << i << ": " << huScratch[i] << endl;
}
cout << endl;
*/