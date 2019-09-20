#pragma once

#include "ofMain.h"
#include "ofxAzureKinect.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void exit();

	ofxAzureKinect* kinect;
	ofEasyCam cam;

	ofVboMesh mesh;
	bool isMeshSet = false;
};
