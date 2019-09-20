#include "ofxAzureKinect.h"

ofxAzureKinect::ofxAzureKinect() : TIMEOUT_IN_MS(1000) {
	this->colorTexture = new ofxRenderTexture(0);
	this->colorTexture->showInfo();
	this->depthTexture = new ofxRenderTexture(1);
	this->depthTexture->showInfo();
	//config setting
		//color
	config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.color_format = K4A_IMAGE_FORMAT_COLOR_BGRA32;
	config.color_resolution = K4A_COLOR_RESOLUTION_720P;
	colorTexture->setQuadSize(ofVec2f(1280, 720));
		//depth
	cout << "Select DepthCameraMode" << endl;
	cout << "Containts" << endl;
	cout << "NFOV_2X2BINNE : 0, NFOV_UNBINNE : 1, WFOV_2X2BINNED : 2, WFOV_UNBINNED (only runs at 15FPS or less) : 3 PASSIVE_IR (only runs at 15FPS or less) : 4" << endl;
	unsigned int depthModeInt;
	cin >> depthModeInt;
	cout << endl;
	depthMode = (DepthCameraMode)depthModeInt;

	switch (depthMode)
	{
		case DEPTH_MODE_NFOV_2X2BINNED:
			config.depth_mode = K4A_DEPTH_MODE_NFOV_2X2BINNED;
			depthTexture->setQuadSize(ofVec2f(320, 288));
			break;
		case DEPTH_MODE_NFOV_UNBINNED:
			config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
			depthTexture->setQuadSize(ofVec2f(640, 576));
			break;
		case DEPTH_MODE_WFOV_2X2BINNED:
			config.depth_mode = K4A_DEPTH_MODE_WFOV_2X2BINNED;
			depthTexture->setQuadSize(ofVec2f(512, 512));
			break;
		case DEPTH_MODE_WFOV_UNBINNED:
			cout << "DepthMode : " << "K4A_DEPTH_MODE_WFOV_UNBINNED" << endl;
			config.depth_mode = K4A_DEPTH_MODE_WFOV_UNBINNED;
			depthTexture->setQuadSize(ofVec2f(1024, 1024));
			break;
		case DEPTH_MODE_PASSIVE_IR:
			cout << "DepthMode : " << "K4A_DEPTH_MODE_PASSIVE_IR" << endl;
			//only runs at 15FPS or less
			config.depth_mode = K4A_DEPTH_MODE_PASSIVE_IR;
			depthTexture->setQuadSize(ofVec2f(1024, 1024));
			break;
		default:
			break;
	}

		//FPS
	cout << "Select FPS" << endl;
	cout << "Containts" << endl;
	cout << "FPS_5 : 0, FPS_15 : 1, FPS_30 : 2" << endl;
	unsigned int fpsModeInt;
	cin >> fpsModeInt;
	cout << endl;
	fpsMode = (FramesPerSecond)fpsModeInt;

	switch (fpsMode)
	{
		case FPS_5:
			config.camera_fps = K4A_FRAMES_PER_SECOND_5;
			break;
		case FPS_15:
			config.camera_fps = K4A_FRAMES_PER_SECOND_15;
			break;
		case FPS_30:
			config.camera_fps = K4A_FRAMES_PER_SECOND_30;
			break;
		default:
			break;
	}
}
//--------------------------------------------------------------
ofxAzureKinect::~ofxAzureKinect() {
	this->closeDevice();
}
//--------------------------------------------------------------

int ofxAzureKinect::deviceGetInstalledCount() {
	return  (int)k4a_device_get_installed_count();
}
//--------------------------------------------------------------
bool ofxAzureKinect::openDevice() {
	bool isOpened = true;
	if (K4A_RESULT_SUCCEEDED != k4a_device_open(K4A_DEVICE_DEFAULT, getDevice()))
	{
		ofLogWarning() << "Failed to open device";
		isOpened = false;
	}
	return isOpened;
}
//--------------------------------------------------------------
bool ofxAzureKinect::start() {
	bool isStarted = true;
	if (K4A_RESULT_SUCCEEDED != k4a_device_start_cameras(*getDevice(), getConfig()))
	{
		ofLogWarning() << "Failed to Start device";
		isStarted = false;
	}

	std:cout << "Open Devicce" << std::endl;
	return isStarted;
}
//--------------------------------------------------------------
void ofxAzureKinect::captureImage() {
	if (k4a_device_get_capture(device, &capture, TIMEOUT_IN_MS) == K4A_WAIT_RESULT_SUCCEEDED) {
		color_image = k4a_capture_get_color_image(capture);

		if (color_image) {
			ofVec2f res = ofVec2f(k4a_image_get_width_pixels(color_image), k4a_image_get_height_pixels(color_image));
			uint8_t *pixles = k4a_image_get_buffer(color_image);

			if (pixles != nullptr) {
				if (!isTextureUploaded) {
					colorTexture->Update(pixles, res.x, res.y, TextureType::RGBA);
					isTextureUploaded = true;
				}
				else {
					colorTexture->Upload(pixles, res.x, res.y, TextureType::RGBA);
				}
			}
		}
	}
}
//--------------------------------------------------------------
void ofxAzureKinect::captureDepth() {
	if (k4a_device_get_capture(device, &capture, TIMEOUT_IN_MS) == K4A_WAIT_RESULT_SUCCEEDED) {
		depth_image = k4a_capture_get_depth_image(capture);
		if (depth_image) {
			ofVec2f res = ofVec2f(k4a_image_get_width_pixels(depth_image), k4a_image_get_height_pixels(depth_image));
			uint8_t *pixles = k4a_image_get_buffer(depth_image);
			if (pixles != nullptr) {
				if (!isTextureUploaded) {
					depthTexture->Update(pixles, res.x, res.y, TextureType::Depth);
					isTextureUploaded = true;
				}
				else {
					depthTexture->Upload(pixles, res.x, res.y, TextureType::Depth);
				}
			}
		}
	}
}
//--------------------------------------------------------------------------------------------------------------
bool ofxAzureKinect::calibrate() {
	bool isCalibrated = false;

	if (K4A_RESULT_SUCCEEDED ==
		k4a_device_get_calibration(device, config.depth_mode, config.color_resolution, &calibration))
	{
		cout << "Calibration Successed" << endl;
		isCalibrated = true;
	}
	else {
		ofLogWarning() << "Calibration Failed";
		isCalibrated = false;
	}

	return isCalibrated;
}
//--------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::drawColorImage() {
	colorTexture->preview(0);
}
//--------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::drawDepthImage() {
	ofPushMatrix();
	//ofTranslate(depthTexture->getSize().x / 2.0, -depthTexture->getSize().y / 2.0);
	ofTranslate(ofGetWidth()/2.0 - depthTexture->getSize().x / 2.0,  - ofGetHeight()/2.0 + depthTexture->getSize().y / 2.0);
	depthTexture->preview(1);
	ofPopMatrix();
}

//--------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::imageCreate(k4a_image_format_t format, int width_pixels, int height_pixels,
		int stride_bytes, k4a_image_t* image_handle) {

		k4a_image_create(format, width_pixels, height_pixels, stride_bytes, image_handle);

}
//--------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::createXYTable(const k4a_calibration_t *calibration, k4a_image_t xy_table) {
		k4a_float2_t *table_data = (k4a_float2_t *)(void *)k4a_image_get_buffer(xy_table);
		int width = calibration->depth_camera_calibration.resolution_width;
		int height = calibration->depth_camera_calibration.resolution_height;

		k4a_float2_t p;
		k4a_float3_t ray;
		int valid;

		for (int y = 0, idx = 0; y < height; y++)
		{
			p.xy.y = (float)y;
			for (int x = 0; x < width; x++, idx++)
			{
				p.xy.x = (float)x;

				k4a_calibration_2d_to_3d(
					calibration, &p, 1.f, K4A_CALIBRATION_TYPE_DEPTH, K4A_CALIBRATION_TYPE_DEPTH, &ray, &valid);

				if (valid)
				{
					table_data[idx].xy.x = ray.xyz.x;
					table_data[idx].xy.y = ray.xyz.y;
				}
				else
				{
					table_data[idx].xy.x = nanf("");
					table_data[idx].xy.y = nanf("");
				}
			}
		}
	}
//--------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::generate_point_cloud(k4a_image_t* depth_image, const k4a_image_t xy_table,
		k4a_image_t point_cloud, int *point_count) {


		int width = k4a_image_get_width_pixels(*depth_image);
		int height = k4a_image_get_height_pixels(*depth_image);

		uint16_t *depth_data = (uint16_t *)(void *)k4a_image_get_buffer(*depth_image);


		k4a_float2_t *xy_table_data = (k4a_float2_t *)(void *)k4a_image_get_buffer(xy_table);
		k4a_float3_t *point_cloud_data = (k4a_float3_t *)(void *)k4a_image_get_buffer(point_cloud);

		*point_count = 0;
		for (int i = 0; i < width * height; i++)
		{
			if (depth_data[i] != 0 && !isnan(xy_table_data[i].xy.x) && !isnan(xy_table_data[i].xy.y))
			{

				point_cloud_data[i].xyz.x = xy_table_data[i].xy.x * (float)depth_data[i];
				point_cloud_data[i].xyz.y = xy_table_data[i].xy.y * (float)depth_data[i];
				point_cloud_data[i].xyz.z = (float)depth_data[i];
				(*point_count)++;
			}
			else
			{
				point_cloud_data[i].xyz.x = nanf("");
				point_cloud_data[i].xyz.y = nanf("");
				point_cloud_data[i].xyz.z = nanf("");
			}
		}

}

//-------------------------------------------------------------------------------------------------------------
void ofxAzureKinect::setPointCloud(const k4a_image_t point_cloud, int point_count, ofVboMesh* mesh, bool isSet) {
	int width = k4a_image_get_width_pixels(point_cloud);
	int height = k4a_image_get_height_pixels(point_cloud);


	k4a_float3_t *point_cloud_data = (k4a_float3_t *)(void *)k4a_image_get_buffer(point_cloud);


	for (int i = 0; i < width * height; i++)
	{
		ofVec3f pos = ofVec3f((float)point_cloud_data[i].xyz.x, (float)point_cloud_data[i].xyz.y, (float)point_cloud_data[i].xyz.z);
		if (isnan(point_cloud_data[i].xyz.x) || isnan(point_cloud_data[i].xyz.y) || isnan(point_cloud_data[i].xyz.z))
		{
			//continue;
			pos = ofVec3f(0.0f, 0.0f, 0.0f);
		}

		if (!isSet) {
			mesh->addColor(ofColor::white);
			mesh->addIndex(i);
			mesh->addVertex(pos);
			//cout << "index : " << i << endl;
		}
		else if (isSet) {
			mesh->setVertex(i, pos);
			//mesh->setColor(i, ofColor::white);
			//cout << "index : " << i << endl;
		}
	}
}
//--------------------------------------------------------------
void ofxAzureKinect::loadShader(string vShader1, string fShader1, string vShader2, string fShader2) {
	colorTexture->loadShader(vShader1, fShader1);
	depthTexture->loadShader(vShader2, fShader2);
}
//--------------------------------------------------------------
void ofxAzureKinect::closeDevice() {
	if (getDevice() != nullptr) {
		k4a_device_close(*getDevice());
	}
}
//--------------------------------------------------------------
k4a_device_t* ofxAzureKinect::getDevice() {
	return &device;
}
//--------------------------------------------------------------
k4a_device_configuration_t* ofxAzureKinect::getConfig() {
	return &config;
}
