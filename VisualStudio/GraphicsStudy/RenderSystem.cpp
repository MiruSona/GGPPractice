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

	//OpenGL 속성 설정
	//core 프로파일 사용
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//OpenGL 버전 명시
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//8-bits per RGBA 채널 칼라 버퍼 요청
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//더블 버퍼링 enable
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//하드웨어 가속 강제로 사용(그래픽 카드 사용)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	//화면 중앙 위치 계산
	SDL_DisplayMode dm;
	SDL_GetCurrentDisplayMode(0, &dm);
	int center_x = (dm.w - (int)mScreenWidth) / 2;
	int center_y = (dm.h - (int)mScreenHeight) / 2;

	//윈도우 생성
	mWindow = SDL_CreateWindow(mTitle.c_str(), center_x, center_y,
		(int)(mScreenWidth), (int)(mScreenHeight), SDL_WINDOW_OPENGL);
	if (!mWindow) {
		SDL_Log("SDL create Window failed : %s", SDL_GetError());
		return false;
	}

	//OpenGL Context 생성
	mContext = SDL_GL_CreateContext(mWindow);

	//GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW");
		return false;
	}

	//일부 플랫폼에서 GLEW 에러 코드 생길 수 잇음
	//그걸 Clear 해줌
	glGetError();

	if (!SpriteInit()) return false;
	if (!MeshInit()) return false;	

	return true;
}

bool RenderSystem::SpriteInit() {
	//VertexArray 초기화
	//점
	float vertices[] = {
		// 위치               // 텍스처 좌표
		0.5f,  0.5f, 0.0f,    1.0f, 1.0f,   // 우측 상단
		0.5f, -0.5f, 0.0f,    1.0f, 0.0f,   // 우측 하단
	   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // 좌측 하단
	   -0.5f,  0.5f, 0.0f,    0.0f, 1.0f    // 좌측 상단
	};
	//인덱스
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	mSpriteVA = new VertexArray(vertices, 4, indices, 6, true);

	//쉐이더 초기화
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("./Shader/Sprite.vert", "./Shader/Sprite.frag"))
		return false;
	//각 텍스쳐 셈플러의 숫자 설정
	mSpriteShader->Active();
	mSpriteShader->SetInt("our_texture1", 0);
	mSpriteShader->SetInt("our_texture2", 1);

	//텍스쳐 초기화
	mSpriteTextures.push_back(new Texture());
	if (!mSpriteTextures[0]->Load("./Assets/2DImages/container.jpg"))
		return false;
	mSpriteTextures.push_back(new Texture());
	if (!mSpriteTextures[1]->Load("./Assets/2DImages/awesomeface.png", true))
		return false;

	return true;
}

bool RenderSystem::MeshInit() {
	//박스 좌표
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

	//쉐이더 초기화
	mMeshShader = new Shader();
	if (!mMeshShader->Load("./Shader/BasicMesh.vert", "./Shader/BasicMesh.frag"))
		return false;
	//각 텍스쳐 셈플러의 숫자 설정
	mMeshShader->Active();
	mMeshShader->SetInt("our_texture1", 0);
	mMeshShader->SetInt("our_texture2", 1);

	//텍스쳐 초기화
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

	//화면 갱신
	SDL_GL_SwapWindow(mWindow);
}

void RenderSystem::Draw2D() {
	//배경 칠하기
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//Depth 버퍼 비활성화
	glDisable(GL_DEPTH_TEST);

	//쉐이더, VertexArray 활성화
	mSpriteShader->Active();
	mSpriteVA->Active();

	//텍스쳐 순서대로 여러개 활성화
	for (int i = 0; i < mSpriteTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mSpriteTextures[i]->Active();
	}

	//시간값
	float time_value = (float)mGameMaster.GetGameTime() * 0.001f;

	//model 행렬(모델 좌표계)
	glm::mat4 model(1.0f);
	//(0.1,0,0) 이동
	model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
	//Z축 기준으로 계속 회전
	model = glm::rotate(model, time_value, glm::vec3(0.0f, 0.0f, 1.0f));
	//X축 기분으로 -55라디안 회전
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//0.5배 크기조절
	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

	//view 행렬(카메라 위치)
	glm::mat4 view(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	//Projection 행렬(카메라 보이는 방향)
	//FOV, 너비/높이, 가까운 거리, 먼 거리 순
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), mScreenWidth / mScreenHeight, 0.1f, 100.0f);

	//유니폼 설정
	mSpriteShader->SetMat4("model", model);
	mSpriteShader->SetMat4("view", view);
	mSpriteShader->SetMat4("projection", projection);

	//그리기!
	glDrawElements(
		GL_TRIANGLES,		//그려야 할 폴리곤/프리미티브의 타입
		6,					//인덱스 버퍼에 있는 인덱스 수
		GL_UNSIGNED_INT,	//각 인덱스의 타입
		nullptr);			//걍 nullptr
}

void RenderSystem::Draw3D() {
	//배경 칠하기
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Depth 버퍼 활성화
	glEnable(GL_DEPTH_TEST);

	//쉐이더
	mMeshShader->Active();

	//텍스쳐 순서대로 여러개 활성화
	for (int i = 0; i < mMeshTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mMeshTextures[i]->Active();
	}

	//위치값
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

	//VA 10개 활성화
	for (int i = 0; i < 10; ++i) {
		mMeshVAs[i]->Active();

		//시간값
		float time_value = (float)mGameMaster.GetGameTime() * 0.001f;

		//model 행렬(모델 좌표계)
		glm::mat4 model(1.0f);
		//이동
		model = glm::translate(model, positions[i]);
		//계속 회전
		model = glm::rotate(model, time_value * glm::radians(-50.0f), positions[i]);
		
		//카메라
		const Camera& cam = mGameMaster.GetCamera();
		//view 행렬(카메라 보는 방향)
		glm::mat4 view = cam.GetViewMatrix();

		//Projection 행렬(카메라 보이는 방향)
		//FOV, 너비/높이, 가까운 거리, 먼 거리 순
		glm::mat4 projection = cam.GetPerspectiveMatrix(mScreenWidth, mScreenHeight);

		//유니폼 설정
		mMeshShader->SetMat4("model", model);
		mMeshShader->SetMat4("view", view);
		mMeshShader->SetMat4("projection", projection);

		//그리기!(인덱스 없이)
		glDrawArrays(
			GL_TRIANGLES,		//그려야 할 폴리곤/프리미티브의 타입
			0,					//무조건 0
			36);				//점 갯수
	}
}