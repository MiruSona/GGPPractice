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
	//앞방향 벡터 구하기
	//빗변을 1(normalize해서)로 하면
	//cosYaw = X방향, cosPitch = X 또는 Z방향
	//sinYaw = Z방향, sinPitch = Y방향
	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	//나머진 외적으로 구함
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));
}

//입력 처리
void Camera::ProcessInput(const Uint8* _key) {
	
	//키보드 움직임
	float velocity = mMoveSpeed * mGameMaster.GetDeltaTime();
	if (_key[SDL_SCANCODE_W])
		mPosition += mFront * velocity;
	if (_key[SDL_SCANCODE_S])
		mPosition -= mFront * velocity;
	if (_key[SDL_SCANCODE_A])
		mPosition -= mRight * velocity;
	if (_key[SDL_SCANCODE_D])
		mPosition += mRight * velocity;

	//마우스 화면 밖으로 못나가게 고정
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//마우스가 얼마나 이동했는지에 대한 좌표 구하기
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	
	//값 계산
	mYaw += (float)x * mMouseSensitive;
	mPitch -= (float)y * mMouseSensitive;

	//Pitch값 제한
	if (mPitch > 89.0f) mPitch = 89.0f;
	if (mPitch < -89.0f) mPitch = -89.0f;

	//방향 다시 계산(Yaw, Pitch가 갱신됬음으로)
	CalcDirectionVectors();
}

//마우스 휠 처리
void Camera::MouseWheel(Sint32 _y) {
	//휠 올릴때
	if (_y > 0 && mZoom > 1.0f) mZoom -= 1.0f;
	//휠 내릴때
	if (_y < 0 && mZoom < 45.0f) mZoom += 1.0f;
}

//View 행렬 반환
glm::mat4 Camera::GetViewMatrix() const {
	return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

//Perspective 행렬 반환
glm::mat4 Camera::GetPerspectiveMatrix(float _width, float _height) const {
	return glm::perspective(glm::radians(mZoom), _width / _height, mNear, mFar);
}