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

bool Texture::Load(const std::string& fileName, bool _flip_y) {

	//�Ӹ� ���� �÷���
	unsigned int flags = SOIL_FLAG_MIPMAPS;
	//�ʿ��ϸ� Y �ø� �߰�
	if (_flip_y) flags |= SOIL_FLAG_INVERT_Y;

	//�ؽ��� �ε� + ID��ȯ
	mTextureID = SOIL_load_OGL_texture(
		fileName.c_str(),
		SOIL_LOAD_AUTO, 
		SOIL_CREATE_NEW_ID, 
		flags);

	//ID�� 0�̸� ���� ���Ѱ�
	if (mTextureID == 0)
		return false;

	//���ε�
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	//�ʺ�, ���� �ޱ�
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mHeight);

	//Mipmaps ����
	//glGenerateMipmap(GL_TEXTURE_2D);
	//���(Minifying) ���� : �Ӹ� ��������(3�� ��������) ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//Ȯ��(Magnifying) ���� : ���� ���� ����(2�� ��������) ��� -> Ȯ��� �Ӹ� �ʿ�X
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	mLoad = true;
	return true;
}

//���ε�
void Texture::Active() {
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
