#include "GameMaster.h"
#include "RenderSystem.h"
#include "Camera.h"
#include <string>
#include <SDL2/SDL.h>

GameMaster::GameMaster() :
	mState(State::Paused),
	mRenderSystem(nullptr),
	mInit(false),
	mCamera(nullptr),
	mGameTime(0)
{
	mRenderSystem = new RenderSystem(*this);
	mCamera = new Camera(*this, glm::vec3(0.f, 0.f, 3.f));
}
bool GameMaster::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_Log("SDL Initialize failed : %s", SDL_GetError());
		return false;
	}

	if (!mRenderSystem->Initialize(1024.f, 768.0f))
		return false;

	mGameTime = SDL_GetTicks();

	mState = State::Play;
	mInit = true;

	return true;
}
void GameMaster::GameLoop() {
	while (mState != State::Quit) {
		//Ÿ�� ������ ��ٸ���
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mGameTime + 16));

		//�ð��� ���
		mDeltaTime = (SDL_GetTicks() - mGameTime) / 1000.0f;
		if (mDeltaTime > 0.05f)
			mDeltaTime = 0.05f;
		mGameTime = SDL_GetTicks();

		ProcessInput();
		mRenderSystem->Draw();
	}
}

void GameMaster::ProcessInput() {
	//�̺�Ʈ ó��
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		SDL_Keycode key = event.key.keysym.sym;
		switch (event.type)
		{
			//x��ư ������ ����
		case SDL_QUIT:
			mState = State::Quit;
			break;
			//���콺 ��
		case SDL_MOUSEWHEEL:
			mCamera->MouseWheel(event.wheel.y);
			break;
		}
	}

	//�����̽��� ������ ����
	const Uint8* input_key = SDL_GetKeyboardState(NULL);
	if (input_key[SDL_SCANCODE_ESCAPE])
		mState = State::Quit;

	//ī�޶󿡰� �Ѱ��ֱ�
	mCamera->ProcessInput(input_key);
}

void GameMaster::Quit() {
	if (!mInit) return;
	delete mRenderSystem;
}