/*#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;

const char* noDefect = "NoDefect.jpg";
const char* scratch = "Scratch.jpg";
const char* pit = "Pit.jpg";
const char* smearing = "Smearing.jpg";


int main() {
	Mat noDefectImg;
	Mat scratchImg;
	Mat pitImg;
	Mat smearingImg;

	noDefectImg = imread(noDefect, IMREAD_COLOR);
	scratchImg = imread(scratch, IMREAD_COLOR);
	pitImg = imread(pit, IMREAD_COLOR);
	smearingImg = imread(smearing, IMREAD_COLOR);

	namedWindow(noDefect, WINDOW_AUTOSIZE);
	namedWindow(scratch, WINDOW_AUTOSIZE);
	namedWindow(pit, WINDOW_AUTOSIZE);
	namedWindow(smearing, WINDOW_AUTOSIZE);

	imshow(noDefect, noDefectImg);
	imshow(scratch, scratchImg);
	imshow(pit, pitImg);
	imshow(smearing, smearingImg);

	waitKey(0);
	return 0;
}*/