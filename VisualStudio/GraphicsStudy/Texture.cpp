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

//텍스쳐 로드
bool Texture::Load(const std::string& fileName) {

	int channels = 0;
	//텍스쳐 이미지 데이터 로드
	unsigned char* image = SOIL_load_image(
		fileName.c_str(),
		&mWidth, &mHeight,
		&channels, SOIL_LOAD_AUTO
	);

	//로드 실패시
	if (image == nullptr) {
		SDL_Log("SOIL 이미지 로드 실패 %s : %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	//채널 수 == RGBA
	GLenum format = GL_RGB;
	if (channels == 4)
		format = GL_RGBA;

	//텍스쳐 오브젝트 생성
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//텍스쳐 오브젝트에 텍스쳐 이미지 데이터 복사
	glTexImage2D(
		GL_TEXTURE_2D,		//텍스쳐 타깃(1D, 3D 도 있다)
		0,					//LOD(밉맵 레벨을 수동으로 지정할 때)
		format,				//OpenGL이 사용해야 되는 색상 포맷(채널)
		mWidth,				//너비
		mHeight,			//높이
		0,					//항상 0이어야됨
		format,				//원본(입력) 데이터 색상 포맷(채널)
		GL_UNSIGNED_BYTE,	//원본(입력) 데이터 비트 깊이
		image);				//원본(입력) 이미지 데이터의 포인터

	//Mipmaps 생성
	glGenerateMipmap(GL_TEXTURE_2D);
	//축소(Minifying) 필터 : 밉맵 선형보간(3중 선형보간) 사용
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//확대(Magnifying) 필터 : 기존 선형 보간(2중 선형보간) 사용 -> 확대는 밉맵 필요X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//이미지 데이터 해제
	SOIL_free_image_data(image);

	mLoad = true;
	return true;
}

//바인딩
void Texture::Active() {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
