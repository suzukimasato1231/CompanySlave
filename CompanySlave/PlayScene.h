#pragma once
#include "_DirectX.h"
#include "Safe_delete.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Singleton.h"
#include"Sprite.h"
#include "Object.h"
#include"MapStage.h"
#include"Player.h"
#include"PushCollision.h"
#include"Enemy.h"

extern const int window_width;
extern const int window_height;
class PlayScene :public Singleton<PlayScene>
{
private:
	friend Singleton<PlayScene>;
public:
	PlayScene();
	~PlayScene();

	void Initialize();

	void Init();

	void StageInit();

	void Update();

	void Draw();

	bool GetSceneFlag();
private:
	Camera *camera = nullptr;
	LightGroup *lightGroup = nullptr;
	DebugText debugText;
	Audio *audio = nullptr;
private://定義

	//音データ
	SoundData sound1;
	SoundData sound2;

	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Parent;
	SpriteData controlGraph;
	SpriteData GameOverGraph;
	//オブジェクトデータ

	//オブジェクト画像
	int graph1;
	int graph2;
	int graph3;


	Vec2 pos = { 0.0f,0.0f };
	Vec2 deadGraphPos = { 0.0f,-800.0f };
	float nowTime = 0;//ラープ
	float endTime = 5;//ラープ
	float timeRate = 0;//ラープ

	//マップチップ
	MapStage *mapStage = nullptr;

	//プレイヤー
	Player *player = nullptr;

	//エネミー
	Enemy *enemy = nullptr;

	//パーティクル
	//敵死んだ時のパーティクル
	ParticleManager* particleMan = nullptr;
	//歩行パーティクル	
	ParticleManager *particleMan2 = nullptr;
	ParticleManager *particleMan3 = nullptr;
	ParticleManager *particleMan4 = nullptr;
	ParticleManager* particleMan5 = nullptr;
	//FBX
	Model *model1 = nullptr;
	FBXObject3d *fbxObject1 = nullptr;

	int stageNum = 1;

	bool stageFlag = true;

	bool sceneFlag = false;
};