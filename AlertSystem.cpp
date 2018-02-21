#ifndef ALERTSYSTEM_S
#define ALERTSYSTEM_S

#include "AlertSystem.h"
#include "ImageStream.cpp"

using namespace std;

static void soundAlert() {
	cout << "Defect detected in String #" << getCurrentString() << "\n";
}

#endif