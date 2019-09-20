#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(0);
ofSetFrameRate(30);
ofSetVerticalSync(true);

kinect = new ofxAzureKinect();
kinect->openDevice();
kinect->start();

if (ofIsGLProgrammableRenderer()) {
	cout << "openGL 3.x" << endl;
	kinect->loadShader("shadersGL3/shader.vert", "shadersGL3/colorShader.frag",
		"shadersGL3/shader.vert", "shadersGL3/depthShader.frag");
}
else {
	cout << "openGL 2.x" << endl;
	kinect->loadShader("shadersGL2/shader.vert", "shadersGL2/colorShader.frag",
		"shadersGL2/shader.vert", "shadersGL2/depthShader.frag");
}


cout << ofxAzureKinect::deviceGetInstalledCount() << endl;
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(to_string(ofGetFrameRate()));
kinect->captureImage();
kinect->captureDepth();

}

//--------------------------------------------------------------
void ofApp::draw() {

	cam.begin();
	kinect->drawColorImage();
	kinect->drawDepthImage();
	cam.end();


	kinect->releaseCapture(kinect->getCapture());
	kinect->releaseImage(kinect->getColorImage());
	kinect->releaseImage(kinect->getDepthImage());
}

//--------------------------------------------------------------
void ofApp::exit() {
	delete(kinect);
}
