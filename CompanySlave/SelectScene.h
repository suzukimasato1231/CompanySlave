#pragma once
#include "_DirectX.h"
#include "Safe_delete.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include"Collision.h"
#include"Audio.h"
#include"Singleton.h"
#include"Sprite.h"
#include "Object.h"

extern const int window_width;
extern const int window_height;
class SelectScene :public Singleton<SelectScene>
{
private:
	friend Singleton<SelectScene>;
	_DirectX* directX = nullptr;
public:
	SelectScene();
	~SelectScene();

	void Initialize(_DirectX* directX);

	void Init();

	void Update();

	void Draw();

	//ステージの数をシーンマネージャーで使うためのやつ
	int GetStage() { return stage; }
	//フェードの数値をシーンマネージャーで変更するためのやつ	
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://定義
	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Bottan;
	SpriteData Number[10];
	SpriteData LoadUIGraph;

	//オブジェクト画像
	int graph2;
	int graph3;

	Vec2 pos = { 0.0f,0.0f };


	int stage = 1;
	//一の位
	int nCount = 1;
	//十の位
	int nCount2 = 0;
	//フェード
	float fade = 1;



};
