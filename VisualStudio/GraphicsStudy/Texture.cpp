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
	//�ؽ��� ����
	if (mLoad) glDeleteTextures(1, &mTextureID);
}

//�ؽ��� �ε�
bool Texture::Load(const std::string& fileName) {

	int channels = 0;
	//�ؽ��� �̹��� ������ �ε�
	unsigned char* image = SOIL_load_image(
		fileName.c_str(),
		&mWidth, &mHeight,
		&channels, SOIL_LOAD_AUTO
	);

	//�ε� ���н�
	if (image == nullptr) {
		SDL_Log("SOIL �̹��� �ε� ���� %s : %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	//ä�� �� == RGBA
	GLenum format = GL_RGB;
	if (channels == 4)
		format = GL_RGBA;

	//�ؽ��� ������Ʈ ����
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//�ؽ��� ������Ʈ�� �ؽ��� �̹��� ������ ����
	glTexImage2D(
		GL_TEXTURE_2D,		//�ؽ��� Ÿ��(1D, 3D �� �ִ�)
		0,					//LOD(�Ӹ� ������ �������� ������ ��)
		format,				//OpenGL�� ����ؾ� �Ǵ� ���� ����(ä��)
		mWidth,				//�ʺ�
		mHeight,			//����
		0,					//�׻� 0�̾�ߵ�
		format,				//����(�Է�) ������ ���� ����(ä��)
		GL_UNSIGNED_BYTE,	//����(�Է�) ������ ��Ʈ ����
		image);				//����(�Է�) �̹��� �������� ������

	//Mipmaps ����
	glGenerateMipmap(GL_TEXTURE_2D);
	//���(Minifying) ���� : �Ӹ� ��������(3�� ��������) ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//Ȯ��(Magnifying) ���� : ���� ���� ����(2�� ��������) ��� -> Ȯ��� �Ӹ� �ʿ�X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//�̹��� ������ ����
	SOIL_free_image_data(image);

	mLoad = true;
	return true;
}

//���ε�
void Texture::Active() {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
