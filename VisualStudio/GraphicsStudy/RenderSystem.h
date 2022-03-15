#pragma once
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <GLM/glm.hpp>
#include <GLM/ext.hpp>

class RenderSystem {
private:
	//초기화
	bool SpriteInit();
	bool MeshInit();

	//그리기
	void Draw2D();
	void Draw3D();

	//관리자
	class GameMaster& mGameMaster;

	//제목
	std::string mTitle;

	//쉐이더
	class Shader* mSpriteShader;
	class Shader* mMeshShader;

	//VertexArray
	class VertexArray* mSpriteVA;
	std::vector<class VertexArray*> mMeshVAs;

	//텍스쳐
	std::vector<class Texture*> mSpriteTextures;
	std::vector<class Texture*> mMeshTextures;

	//화면 크기
	float mScreenWidth, mScreenHeight;

	//창, GLContext
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