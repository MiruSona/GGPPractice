#pragma once
#include <GLM/glm.hpp>
#include <SDL2/SDL.h>

class Camera {
private:
	//관리자
	class GameMaster& mGameMaster;

	//위치
	glm::vec3 mPosition;

	//방향 벡터
	glm::vec3 mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 mUp;
	glm::vec3 mRight;
	//세계 상향축
	glm::vec3 mWorldUp;

	//Euler 각도
	float mYaw = -90.0f, mPitch = 0.0f;

	//속성
	float mMoveSpeed = 2.5f;
	float mMouseSensitive = 0.1f;
	float mZoom = 45.0f;

	//단면 거리
	float mNear = 0.1f;
	float mFar = 100.0f;

	//방향 벡터 계산
	void CalcDirectionVectors();

public:
	Camera(class GameMaster& _gm, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _world_up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera();

	//입력 처리
	void ProcessInput(const Uint8* state);
	void MouseWheel(Sint32 _y);

	//View 행렬 계산
	glm::mat4 GetViewMatrix() const;
	//Perspective 행렬 계산
	glm::mat4 GetPerspectiveMatrix(float _width, float _height) const;
};