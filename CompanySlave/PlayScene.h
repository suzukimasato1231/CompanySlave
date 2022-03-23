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
	_DirectX* directX = nullptr;
public:
	PlayScene();
	~PlayScene();

	void Initialize(_DirectX* directX);

	void Init();

	void Update();

	void Draw();

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://定義

	//音データ
	Audio::SoundData sound1;
	Audio::SoundData sound2;

	//スプライトデータ
	Sprite::SpriteData spriteGraph;
	Sprite::SpriteData BGGraph;
	Sprite::SpriteData Parent;

	//オブジェクトデータ

	//オブジェクト画像
	int graph1;
	int graph2;
	int graph3;

	Vec2 pos = { 0.0f,0.0f };

	//マップチップ
	MapStage* mapStage = nullptr;

	//プレイヤー
	Player* player = nullptr;

	//エネミー
	Enemy* enemy = nullptr;

	//パーティクル
	ParticleManager* particleMan = nullptr;

	ParticleManager* particleMan2 = nullptr;

	//FBX
	Model* model1 = nullptr;
	FBXObject3d* fbxObject1 = nullptr;
};