#include "ofxRenderTexture.h"

ofxRenderTexture::ofxRenderTexture()
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}


ofxRenderTexture::ofxRenderTexture(int index)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + index);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

ofxRenderTexture::~ofxRenderTexture()
{
	glDeleteTextures(1, &texID);
}

void ofxRenderTexture::Upload(uint8_t* data, int w, int h, TextureType textureType) {
	uint8_t *pixels;
	unsigned short *depth;
	ofVec2f size = ofVec2f(w, h);

	if (textureType == RGBA) {
		int num = w * h * 4;
		pixels = new uint8_t[num];
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = (x + y * w) * 4;
				//input data order is bgra
				pixels[index] = data[index + 2];
				pixels[index + 1] = data[index + 1];
				pixels[index + 2] = data[index];
				pixels[index + 3] = 255.0;
			}
		}
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, reinterpret_cast<unsigned char*>(pixels));
	}
	else if (textureType == Depth) {
		int num = w * h;
		depth = new unsigned short[num];
		int offSet = 0;
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int depthDataIndex = (x + y * w) * 2;
				int pixleDataIndex = (x + y * w);
				unsigned short depth16 = (unsigned short)data[depthDataIndex];
				unsigned short depth16up = (unsigned short)data[depthDataIndex + 1];
				depth16up = depth16up << 8;
				depth16 = (depth16 | depth16up);
				depth[pixleDataIndex] = depth16;
			}
		}
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, depth);
	}
}

void ofxRenderTexture::Update(uint8_t* data, int w, int h, TextureType textureType) {
	if (textureType == RGBA) {
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, data);
	}
	else if (textureType == Depth) {
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, data);
	}
}

void ofxRenderTexture::showInfo() {
	cout << "texID : " << texID << endl;
}

void ofxRenderTexture::setQuadSize(ofVec2f size) {

	float w = size.x / 2.0;
	float h = size.y / 2.0;
	quadSize = ofVec2f (w, h);
	quad.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	quad.addVertex(ofVec3f(w, h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 1.0f));
	quad.addVertex(ofVec3f(w, -h, 0.0));
	quad.addTexCoord(ofVec2f(1.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, -h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 0.0f));
	quad.addVertex(ofVec3f(-w, h, 0.0));
	quad.addTexCoord(ofVec2f(0.0f, 1.0f));
}

void ofxRenderTexture::preview(int index) {
	shader.begin();
	shader.setUniform1i("tex0", index);
	quad.draw();
	shader.end();
}


void ofxRenderTexture::loadShader(string path1, string path2) {
	shader.load(path1, path2);
}

GLuint* ofxRenderTexture::getTexID() {
	return &texID;
}
