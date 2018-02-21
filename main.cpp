#include "ImageStream.cpp"
#include "DetectionSystem.cpp"
#include "AlertSystem.cpp"

int mainZ() {
	initializeStream();
	while (hasNext()) {
		Mat* nextImage = getNext();
		detectDefect(nextImage);
		displayImage(nextImage);
		cout << "Processed String Number " << getCurrentString() << "\n";
	}
	waitKey(0);
	return(0);
}
