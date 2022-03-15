#pragma once
#include <GLM/glm.hpp>
#include <SDL2/SDL.h>

class Camera {
private:
	//������
	class GameMaster& mGameMaster;

	//��ġ
	glm::vec3 mPosition;

	//���� ����
	glm::vec3 mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 mUp;
	glm::vec3 mRight;
	//���� ������
	glm::vec3 mWorldUp;

	//Euler ����
	float mYaw = -90.0f, mPitch = 0.0f;

	//�Ӽ�
	float mMoveSpeed = 2.5f;
	float mMouseSensitive = 0.1f;
	float mZoom = 45.0f;

	//�ܸ� �Ÿ�
	float mNear = 0.1f;
	float mFar = 100.0f;

	//���� ���� ���
	void CalcDirectionVectors();

public:
	Camera(class GameMaster& _gm, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _world_up = glm::vec3(0.0f, 1.0f, 0.0f));
	~Camera();

	//�Է� ó��
	void ProcessInput(const Uint8* state);
	void MouseWheel(Sint32 _y);

	//View ��� ���
	glm::mat4 GetViewMatrix() const;
	//Perspective ��� ���
	glm::mat4 GetPerspectiveMatrix(float _width, float _height) const;
};