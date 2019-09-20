#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {

	ofGLFWWindowSettings settings;
	settings.setGLVersion(3, 3);
	settings.setSize(1200, 720);
	settings.resizable = false;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());


	/*
	//GL 2.x
	ofSetupOpenGL(1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
	*/

	}
