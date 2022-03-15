#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

class RenderSystem {
private:
	//�ʱ�ȭ
	bool SpriteInit();
	bool MeshInit();

	//�׸���
	void Draw2D();
	void Draw3D();

	//������
	class GameMaster& mGameMaster;

	//����
	std::string mTitle;

	//���̴�
	class Shader* mSpriteShader;
	class Shader* mMeshShader;

	//VertexArray
	class VertexArray* mSpriteVA;
	std::vector<class VertexArray*> mMeshVAs;

	//�ؽ���
	std::vector<class Texture*> mSpriteTextures;
	std::vector<class Texture*> mMeshTextures;

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