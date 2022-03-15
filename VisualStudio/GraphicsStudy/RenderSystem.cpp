#include "RenderSystem.h"
#include "GameMaster.h"
#include "Camera.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GLM/gtx/string_cast.hpp>
#include <GLM/glm.hpp>

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

	if (!SpriteInit()) return false;
	if (!MeshInit()) return false;	

	return true;
}

bool RenderSystem::SpriteInit() {
	//VertexArray �ʱ�ȭ
	//��
	float vertices[] = {
		// ��ġ               // �ؽ�ó ��ǥ
		0.5f,  0.5f, 0.0f,    1.0f, 1.0f,   // ���� ���
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f,   // ���� �ϴ�
	   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // ���� �ϴ�
	   -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // ���� ���
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
	if (!mSpriteTextures[0]->Load("./Assets/2DImages/container.jpg"))
		return false;
	mSpriteTextures.push_back(new Texture());
	if (!mSpriteTextures[1]->Load("./Assets/2DImages/awesomeface.png", true))
		return false;

	return true;
}

bool RenderSystem::MeshInit() {
	//�ڽ� ��ǥ
	float BoxVerts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	for(int i = 0; i < 10; ++i)
		mMeshVAs.push_back(new VertexArray(BoxVerts, 36));

	//���̴� �ʱ�ȭ
	mMeshShader = new Shader();
	if (!mMeshShader->Load("./Shader/BasicMesh.vert", "./Shader/BasicMesh.frag"))
		return false;
	//�� �ؽ��� ���÷��� ���� ����
	mMeshShader->Active();
	mMeshShader->SetInt("our_texture1", 0);
	mMeshShader->SetInt("our_texture2", 1);

	//�ؽ��� �ʱ�ȭ
	mMeshTextures.push_back(new Texture());
	if (!mMeshTextures[0]->Load("./Assets/2DImages/container.jpg"))
		return false;
	mMeshTextures.push_back(new Texture());
	if (!mMeshTextures[1]->Load("./Assets/2DImages/awesomeface.png", true))
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

	//Draw2D();
	Draw3D();

	//ȭ�� ����
	SDL_GL_SwapWindow(mWindow);
}

void RenderSystem::Draw2D() {
	//��� ĥ�ϱ�
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Depth ���� ��Ȱ��ȭ
	glDisable(GL_DEPTH_TEST);

	//���̴�, VertexArray Ȱ��ȭ
	mSpriteShader->Active();
	mSpriteVA->Active();

	//�ؽ��� ������� ������ Ȱ��ȭ
	for (int i = 0; i < mSpriteTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mSpriteTextures[i]->Active();
	}

	//�ð���
	float time_value = (float)mGameMaster.GetGameTime() * 0.001f;

	//model ���(�� ��ǥ��)
	glm::mat4 model(1.0f);
	//(0.1,0,0) �̵�
	model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
	//Z�� �������� ��� ȸ��
	model = glm::rotate(model, time_value, glm::vec3(0.0f, 0.0f, 1.0f));
	//X�� ������� -55���� ȸ��
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//0.5�� ũ������
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

	//view ���(ī�޶� ��ġ)
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	//Projection ���(ī�޶� ���̴� ����)
	//FOV, �ʺ�/����, ����� �Ÿ�, �� �Ÿ� ��
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), mScreenWidth / mScreenHeight, 0.1f, 100.0f);

	//������ ����
	mSpriteShader->SetMat4("model", model);
	mSpriteShader->SetMat4("view", view);
	mSpriteShader->SetMat4("projection", projection);

	//�׸���!
	glDrawElements(
		GL_TRIANGLES,		//�׷��� �� ������/������Ƽ���� Ÿ��
		6,					//�ε��� ���ۿ� �ִ� �ε��� ��
		GL_UNSIGNED_INT,	//�� �ε����� Ÿ��
		nullptr);			//�� nullptr
}

void RenderSystem::Draw3D() {
	//��� ĥ�ϱ�
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Depth ���� Ȱ��ȭ
	glEnable(GL_DEPTH_TEST);

	//���̴�
	mMeshShader->Active();

	//�ؽ��� ������� ������ Ȱ��ȭ
	for (int i = 0; i < mMeshTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mMeshTextures[i]->Active();
	}

	//��ġ��
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//VA 10�� Ȱ��ȭ
	for (int i = 0; i < 10; ++i) {
		mMeshVAs[i]->Active();

		//�ð���
		float time_value = (float)mGameMaster.GetGameTime() * 0.001f;

		//model ���(�� ��ǥ��)
		glm::mat4 model(1.0f);
		//�̵�
		model = glm::translate(model, positions[i]);
		//��� ȸ��
		model = glm::rotate(model, time_value * glm::radians(-50.0f), positions[i]);
		
		//ī�޶�
		const Camera& cam = mGameMaster.GetCamera();
		//view ���(ī�޶� ���� ����)
		glm::mat4 view = cam.GetViewMatrix();

		//Projection ���(ī�޶� ���̴� ����)
		//FOV, �ʺ�/����, ����� �Ÿ�, �� �Ÿ� ��
		glm::mat4 projection = cam.GetPerspectiveMatrix(mScreenWidth, mScreenHeight);

		//������ ����
		mMeshShader->SetMat4("model", model);
		mMeshShader->SetMat4("view", view);
		mMeshShader->SetMat4("projection", projection);

		//�׸���!(�ε��� ����)
		glDrawArrays(
			GL_TRIANGLES,		//�׷��� �� ������/������Ƽ���� Ÿ��
			0,					//������ 0
			36);				//�� ����
	}
}