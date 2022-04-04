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

private://定義
	enum Scene {
		titleSceneInit,
		titleScene,
		selectScene,
		stage1,
		scoreScene
	};
	//最初のシーン
	Scene scene = titleScene;

	Title* title = nullptr;
	PlayScene* play = nullptr;
	SelectScene* select = nullptr;
	DebugText debugText;
	Sprite::SpriteData BGGraph;
	Sprite::SpriteData LoadUIGraph[7];

	//フェードのフラグ(tがタイトルのやつでsがシーンチェンジのやつ)
	bool tFadeFlag = false;
	bool sFadeFlag = false;
	//フェード(tがタイトルのやつでsがシーンチェンジのやつ)
	float tFade = 1;
	float sFade = 1;

	//疑似ロード画面関係変数
	bool LoadFlag = false;
	int LoadTime = 70;
	int LoadCount = 0;
	int LoadCount2 = 0;
	//シーンごとの初期化
	bool initFlag = true;
};
