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



	if (kinect->calibrate()) {
		cout << (*(kinect->getCalibration())).depth_camera_calibration.resolution_width << endl;


		ofxAzureKinect::imageCreate(
			K4A_IMAGE_FORMAT_CUSTOM,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_width,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_height,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_width*(int)sizeof(k4a_float2_t),
			kinect->getTable()
		);

		ofxAzureKinect::createXYTable(kinect->getCalibration(), *(kinect->getTable()));
		ofxAzureKinect::imageCreate(
			K4A_IMAGE_FORMAT_CUSTOM,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_width,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_height,
			(*(kinect->getCalibration())).depth_camera_calibration.resolution_width*(int)sizeof(k4a_float3_t),
			kinect->getPointCloud()
		);
	}
	mesh.setMode(OF_PRIMITIVE_POINTS);
	cam.setPosition(0.0, 0.0, -100.0);
	cam.lookAt(ofVec3f(0.0, 0.0, 100.0), ofVec3f(0.0, -1.0, 0.0));

}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(to_string(ofGetFrameRate()));
	kinect->captureImage();
	kinect->captureDepth();


	ofxAzureKinect::generate_point_cloud(kinect->getDepthImage(), *(kinect->getTable()),
		*(kinect->getPointCloud()), kinect->getPointCount());

	if (!isMeshSet) {
		kinect->setPointCloud(*(kinect->getPointCloud()),
			*(kinect->getPointCount()), &mesh, isMeshSet);
		isMeshSet = true;
	}else{
		kinect->setPointCloud(*(kinect->getPointCloud()),
			*(kinect->getPointCount()), &mesh, isMeshSet);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	cam.begin();
	mesh.drawVertices();
	cam.end();


	kinect->releaseCapture(kinect->getCapture());
	kinect->releaseImage(kinect->getColorImage());
	kinect->releaseImage(kinect->getDepthImage());
}
//--------------------------------------------------------------
void ofApp::exit() {
	delete(kinect);
}
