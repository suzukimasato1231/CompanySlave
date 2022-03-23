#pragma once
#include"Title.h"
#include"PlayScene.h"
#include"SelectScene.h"

extern const int window_width;
extern const int window_height;
class GameSceneManager :public Singleton<GameSceneManager>
{
private:
	friend Singleton<GameSceneManager>;
	_DirectX* directX = nullptr;
public:
	GameSceneManager();
	~GameSceneManager();

	void Initialize(_DirectX* directX);

	void Init();

	void Update();

	void Draw();

private://’è‹`
	enum Scene {
		titleScene,
		selectScene,
		stage1,
		scoreScene
	};
	Scene scene = titleScene;
	Title* title = nullptr;
	PlayScene* play = nullptr;
	SelectScene* select = nullptr;

	bool tFadeFlag = false;
	bool sFadeFlag = false;
	float tFade = 1;
	float sFade = 1;
};
