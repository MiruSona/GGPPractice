#include "GameMaster.h"
#include "RenderSystem.h"
#include <string>
#include <SDL2/SDL.h>

GameMaster::GameMaster() :
	mState(State::Paused)
{
	mRenderSystem = new RenderSystem(*this);
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
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				mState = State::Quit;
				break;
			}
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_ESCAPE])
			mState = State::Quit;

		mRenderSystem->Draw();

		mGameTime = SDL_GetTicks();
	}

	Quit();
}

void GameMaster::Quit() {
	if (!mInit) return;
	delete mRenderSystem;
}