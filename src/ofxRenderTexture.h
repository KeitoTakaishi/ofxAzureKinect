#pragma once
#include "ofMain.h"

enum TextureType {
	RGBA,
	Depth,
};

class ofxRenderTexture
{
public:
	ofxRenderTexture();
	ofxRenderTexture(int index);
	~ofxRenderTexture();


	void Upload(uint8_t* data, int w, int h, TextureType textureType);
	void Update(uint8_t* data, int w, int h, TextureType textureType);
	void showInfo();

	void setQuadSize(ofVec2f size);
	void preview(int index);
	void loadShader(string vShader, string fShader);
	inline ofVec2f getSize() const {
		return quadSize;
	}
	GLuint* getTexID();

private:
	unsigned char *pixles;
	GLuint texID;

	//quad for preview
	ofVec2f quadSize;
	ofShader shader;
	ofVboMesh quad;
};
