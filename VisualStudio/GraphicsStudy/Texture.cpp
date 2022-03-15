#include "Texture.h"
#include <SOIL2/SOIL2.h>
#include <GL/glew.h>
#include <SDL2/SDL.h>

Texture::Texture() :
	mTextureID(0),
	mWidth(0), mHeight(0)
{

}
Texture::~Texture() {
	//텍스쳐 삭제
	if (mLoad) glDeleteTextures(1, &mTextureID);
}

bool Texture::Load(const std::string& fileName, bool _flip_y) {

	//밉맵 생성 플래그
	unsigned int flags = SOIL_FLAG_MIPMAPS;
	//필요하면 Y 플립 추가
	if (_flip_y) flags |= SOIL_FLAG_INVERT_Y;

	//텍스쳐 로드 + ID반환
	mTextureID = SOIL_load_OGL_texture(
		fileName.c_str(),
		SOIL_LOAD_AUTO, 
		SOIL_CREATE_NEW_ID, 
		flags);

	//ID가 0이면 생성 못한거
	if (mTextureID == 0)
		return false;

	//바인딩
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//너비, 높이 받기
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mHeight);

	//Mipmaps 생성
	//glGenerateMipmap(GL_TEXTURE_2D);
	//축소(Minifying) 필터 : 밉맵 선형보간(3중 선형보간) 사용
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//확대(Magnifying) 필터 : 기존 선형 보간(2중 선형보간) 사용 -> 확대는 밉맵 필요X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	mLoad = true;
	return true;
}

//바인딩
void Texture::Active() {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
