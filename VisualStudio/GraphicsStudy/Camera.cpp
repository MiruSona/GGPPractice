#include "Camera.h"
#include "GameMaster.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

Camera::Camera(GameMaster& _gm, glm::vec3 _position, glm::vec3 _world_up):
	mGameMaster(_gm),
	mPosition(_position),
	mWorldUp(_world_up)
{
	CalcDirectionVectors();
	
}
Camera::~Camera() {

}

void Camera::CalcDirectionVectors() {
	//�չ��� ���� ���ϱ�
	//������ 1(normalize�ؼ�)�� �ϸ�
	//cosYaw = X����, cosPitch = X �Ǵ� Z����
	//sinYaw = Z����, sinPitch = Y����
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	//������ �������� ����
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

//�Է� ó��
void Camera::ProcessInput(const Uint8* _key) {
	
	//Ű���� ������
	float velocity = mMoveSpeed * mGameMaster.GetDeltaTime();
	if (_key[SDL_SCANCODE_W])
		mPosition += mFront * velocity;
	if (_key[SDL_SCANCODE_S])
		mPosition -= mFront * velocity;
	if (_key[SDL_SCANCODE_A])
		mPosition -= mRight * velocity;
	if (_key[SDL_SCANCODE_D])
		mPosition += mRight * velocity;

	//���콺 ȭ�� ������ �������� ����
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//���콺�� �󸶳� �̵��ߴ����� ���� ��ǥ ���ϱ�
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	
	//�� ���
	mYaw += (float)x * mMouseSensitive;
	mPitch -= (float)y * mMouseSensitive;

	//Pitch�� ����
	if (mPitch > 89.0f) mPitch = 89.0f;
	if (mPitch < -89.0f) mPitch = -89.0f;

	//���� �ٽ� ���(Yaw, Pitch�� ���ŉ�������)
	CalcDirectionVectors();
}

//���콺 �� ó��
void Camera::MouseWheel(Sint32 _y) {
	//�� �ø���
	if (_y > 0 && mZoom > 1.0f) mZoom -= 1.0f;
	//�� ������
	if (_y < 0 && mZoom < 45.0f) mZoom += 1.0f;
}

//View ��� ��ȯ
glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

//Perspective ��� ��ȯ
glm::mat4 Camera::GetPerspectiveMatrix(float _width, float _height) const {
	return glm::perspective(glm::radians(mZoom), _width / _height, mNear, mFar);
}