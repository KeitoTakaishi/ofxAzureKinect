#pragma once
#include <k4a/k4a.h>
#include <k4a/k4a.hpp>
#include "ofMain.h"
#include "ofxRenderTexture.h"

enum DepthCameraMode {
	DEPTH_MODE_NFOV_2X2BINNED = 0,
	DEPTH_MODE_NFOV_UNBINNED = 1,
	DEPTH_MODE_WFOV_2X2BINNED = 2,
	DEPTH_MODE_WFOV_UNBINNED = 3,
	DEPTH_MODE_PASSIVE_IR = 4,
};

enum FramesPerSecond {
	FPS_5 = 0,
	FPS_15 = 1,
	FPS_30 = 2,
};

class ofxAzureKinect {

public:
	ofxAzureKinect();
	~ofxAzureKinect();


	bool openDevice();
	bool start();
	void captureImage();
	void captureDepth();
	bool calibrate();



	inline void releaseImage(k4a_image_t* image) {
		k4a_image_release(*image);
	}
	inline void releaseCapture(k4a_capture_t* capture) {
		k4a_capture_release(*capture);
	}

	void closeDevice();
	void loadShader(string vShader1, string fShader1, string vShader2, string fShader2);

	void drawColorImage();

	void drawDepthImage();


	inline k4a_capture_t* getCapture() {
		return &(this->capture);
	}

	inline k4a_image_t* getColorImage() {
		return &(this->color_image);
	}

	inline k4a_image_t* getDepthImage() {
		return &(this->depth_image);
	}

	inline k4a_calibration_t* getCalibration() {
		return &(this->calibration);
	}

	inline k4a_image_t* getTable() {
		return &(this->xy_table);
	}

	inline k4a_image_t* getPointCloud() {
		return &(this->point_cloud);
	}
	inline int* getPointCount() {
		return &(this->pointCount);
	}


	//static method
	static int deviceGetInstalledCount();
	static void imageCreate(k4a_image_format_t format,
		int width_pixels,
		int height_pixels,
		int stride_bytes,
		k4a_image_t *image_handle);

	static void createXYTable(const k4a_calibration_t *calibration, k4a_image_t xy_table);

	static void generate_point_cloud(k4a_image_t* depth_image, const k4a_image_t xy_table,
		k4a_image_t point_cloud, int *point_count);

	static void setPointCloud(const k4a_image_t point_cloud, int point_count, ofVboMesh* mesh, bool isMeshSet);




	//accessor
	k4a_device_t* getDevice();
	k4a_device_configuration_t* getConfig();
private:

	const int32_t TIMEOUT_IN_MS;

	k4a_device_configuration_t config;
	k4a_calibration_t calibration;
	k4a_device_t device;
	k4a_capture_t capture;

	DepthCameraMode depthMode;
	FramesPerSecond fpsMode;

	k4a_image_t color_image;
	k4a_image_t depth_image;
	k4a_image_t xy_table;
	k4a_image_t point_cloud;


	ofxRenderTexture* colorTexture;
	ofxRenderTexture* depthTexture;

	int pointCount;
	bool isTextureUploaded;

};
