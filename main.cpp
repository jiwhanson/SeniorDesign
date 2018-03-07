#include "ImageStream.cpp"
#include "DetectionSystem.cpp"
#include "AlertSystem.cpp"

int main() {
	initializeStream();
	while (hasNext()) {
		Mat* nextImage = getNext();
		detectDefect(nextImage);
		cout << "Processed String Number " << getCurrentString() << "\n";
	}
	waitKey(0);
	return(0);
}
