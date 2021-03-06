#pragma once
#include"Title.h"
#include"PlayScene.h"
#include"Clear.h"
#include "Object.h"
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

	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;

	enum Scene {
		titleScene,
		stage1,
		clearScene
	};
	//最初のシーン
	Scene scene = titleScene;

	Title* title = nullptr;
	PlayScene* play = nullptr;
	Clear* clear = nullptr;
	DebugText debugText;
	SpriteData BGGraph;
	SpriteData LoadUIGraph[7];



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


	//デバック用
	int stageDebug = 1;

	//音量
	float volume;

};
