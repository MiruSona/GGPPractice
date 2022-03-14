#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

class RenderSystem {
private:
	//������
	class GameMaster& mGameMaster;

	//����
	std::string mTitle;

	//���̴�
	class Shader* mSpriteShader;

	//VertexArray
	class VertexArray* mSpriteVA;

	//�ؽ���
	std::vector<class Texture*> mSpriteTextures;

	//ȭ�� ũ��
	float mScreenWidth, mScreenHeight;

	//â, GLContext
	SDL_Window* mWindow;
	SDL_GLContext mContext;

public:
	RenderSystem(class GameMaster& _game_master);

	bool Initialize(float _screen_width, float _screen_height);
	void Destroy();

	void Draw();

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }
};