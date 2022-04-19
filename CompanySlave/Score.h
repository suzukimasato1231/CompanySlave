#pragma once
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
class Score :public Singleton<Score>
{
private:
	friend Singleton<Score>;
	_DirectX* directX = nullptr;
public:
	Score();
	~Score();

	void Initialize(_DirectX* directX);

	void Init();

	void Update();

	void Draw();

	//フェードの数値をシーンマネージャーで変更するためのやつ
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }
	
	int GetHiScore() { return HiScore; }
	int SetHiScore(int HiScore) { return this->HiScore = HiScore; }
private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://定義

	//音データ
	SoundData sound1;
	SoundData sound2;

	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Parent;

	//オブジェクトデータ
	Object::ObjectData Polygon;
	Object::ObjectData rayPolygon;
	Object::ObjectData BossPolygon;

	//オブジェクト画像
	int graph1;
	int graph2;
	int graph3;

	Vec3 pPos1 = { 1.0f,1.0f,0.0f };

	Vec3 pPos2 = { 2.0f,4.0f,5.0f };

	Vec3 angle = { 0.0f,0.0f,0.0f };

	Vec3 angle2 = { 0.0f,0.0f,0.0f };

	Vec2 pos = { 0.0f,0.0f };

	//マップチップ
	MapStage* mapStage = nullptr;

	//プレイヤー
	Player* player = nullptr;

	//エネミー
	Enemy* enemy = nullptr;

	//FBX
	Model* model1 = nullptr;
	FBXObject3d* fbxObject1 = nullptr;

	//フェード
	float fade = 1;

	//ハイスコア
	int HiScore = 0;
};


