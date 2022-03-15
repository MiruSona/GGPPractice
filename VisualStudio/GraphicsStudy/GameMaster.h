#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <SDL2/SDL_types.h>

class GameMaster {
public:
	//���� ����
	enum class State
	{
		Play = 0,
		Paused = 1,
		Quit = 2
	};

private:
	//�ʱ�ȭ ����
	bool mInit;

	//������ ���
	class RenderSystem* mRenderSystem;

	//ī�޶�
	class Camera* mCamera;

	//���� �ð�
	Uint32 mGameTime;
	//�ð���
	float mDeltaTime;
	//���� ����
	State mState;

	//�Է� ó��
	void ProcessInput();

public:
	GameMaster();

	bool Initialize();
	void GameLoop();
	void Quit();

	const class Camera& GetCamera() const { return *mCamera; }

	State GetState() const { return mState; }
	void SetState(State _state) { mState = _state; }

	Uint32 GetGameTime() const { return mGameTime; }
	const float GetDeltaTime() const { return mDeltaTime; }
};