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


	//VertexArray 초기화
	//점
	float vertices[] = {
		// 위치              // 컬러             // 텍스처 좌표
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 우측 상단
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 우측 하단
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 좌측 하단
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 좌측 상단
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

	//배경 칠하기
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//쉐이더, VertexArray 활성화
	mSpriteShader->Active();
	mSpriteVA->Active();
	//텍스쳐 순서대로 여러개 활성화
	for (int i = 0; i < mSpriteTextures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		mSpriteTextures[i]->Active();
	}

	//유니폼
	mSpriteShader->SetFloat("mix_value", 0.5f);

	//그리기!
	glDrawElements(
		GL_TRIANGLES,		//그려야 할 폴리곤/프리미티브의 타입
		6,					//인덱스 버퍼에 있는 인덱스 수
		GL_UNSIGNED_INT,	//각 인덱스의 타입
		nullptr);			//걍 nullptr

	//화면 갱신
	SDL_GL_SwapWindow(mWindow);
}