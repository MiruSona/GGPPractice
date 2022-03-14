#include "GameMaster.h"
#include "RenderSystem.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GLM/gtx/string_cast.hpp>

RenderSystem::RenderSystem(GameMaster& _game_master) :
	mGameMaster(_game_master),
	mTitle("GraphicsStudy"),
	mSpriteShader(nullptr), mSpriteVA(nullptr),
	mScreenWidth(0.0f), mScreenHeight(0.0f),
	mWindow(nullptr), mContext()
{

}

bool RenderSystem::Initialize(float _screen_width, float _screen_height) {
	mScreenWidth = _screen_width;
	mScreenHeight = _screen_height;

	//OpenGL �Ӽ� ����
	//core �������� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//OpenGL ���� ���
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//8-bits per RGBA ä�� Į�� ���� ��û
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//���� ���۸� enable
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//�ϵ���� ���� ������ ���(�׷��� ī�� ���)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	//ȭ�� �߾� ��ġ ���
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	int center_x = (dm.w - (int)mScreenWidth) / 2;
	int center_y = (dm.h - (int)mScreenHeight) / 2;

	//������ ����
	mWindow = SDL_CreateWindow(mTitle.c_str(), center_x, center_y,
		(int)(mScreenWidth), (int)(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("SDL create Window failed : %s", SDL_GetError());
		return false;
	}

	//OpenGL Context ����
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEW �ʱ�ȭ
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW");
		return false;
	}

	//�Ϻ� �÷������� GLEW ���� �ڵ� ���� �� ����
	//�װ� Clear ����
	glGetError();


	//VertexArray �ʱ�ȭ
	//��
	float vertices[] = {
		// ��ġ              // �÷�             // �ؽ�ó ��ǥ
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ���� ���
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ���� �ϴ�
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ���� �ϴ�
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ���� ���
	};
	//�ε���
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mSpriteVA = new VertexArray(vertices, 4, indices, 6, true);

	//���̴� �ʱ�ȭ
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("./Shader/Sprite.vert", "./Shader/Sprite.frag"))
		return false;
	//�� �ؽ��� ���÷��� ���� ����
	mSpriteShader->Active();
	mSpriteShader->SetInt("our_texture1", 0);
	mSpriteShader->SetInt("our_texture2", 1);

	//�ؽ��� �ʱ�ȭ
	mSpriteTextures.push_back(new Texture());
	if(!mSpriteTextures[0]->Load("./Assets/2DImages/container.jpg"))
		return false;
	mSpriteTextures.push_back(new Texture());
	if (!mSpriteTextures[1]->Load("./Assets/2DImages/awesomeface.png"))
		return false;

	return true;
}

void RenderSystem::Destroy() {
	delete mSpriteVA;
	delete mSpriteShader;
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void RenderSystem::Draw() {

	//��� ĥ�ϱ�
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//���̴�, VertexArray Ȱ��ȭ
	mSpriteShader->Active();
	mSpriteVA->Active();
	//�ؽ��� ������� ������ Ȱ��ȭ
	for (int i = 0; i < mSpriteTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mSpriteTextures[i]->Active();
	}

	//������
	mSpriteShader->SetFloat("mix_value", 0.5f);

	//�׸���!
	glDrawElements(
		GL_TRIANGLES,		//�׷��� �� ������/������Ƽ���� Ÿ��
		6,					//�ε��� ���ۿ� �ִ� �ε��� ��
		GL_UNSIGNED_INT,	//�� �ε����� Ÿ��
		nullptr);			//�� nullptr

	//ȭ�� ����
	SDL_GL_SwapWindow(mWindow);
}