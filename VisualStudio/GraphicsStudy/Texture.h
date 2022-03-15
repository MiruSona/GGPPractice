#pragma once
#include <string>

class Texture {
private:
	//텍스쳐 ID
	unsigned int mTextureID;
	//너비, 높이
	int mWidth, mHeight;
	//로드 여부
	bool mLoad = false;

public:
	//생성/소멸자
	Texture();
	~Texture();

	//로드
	bool Load(const std::string& fileName, bool _flip_y = false);

	//활성화(바인딩)
	void Active();

	//Get
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	unsigned int GetTextureID() const { return mTextureID; }

};