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
#include"Direction.h"

extern const int window_width;
extern const int window_height;
class SelectScene :public Singleton<SelectScene>
{
private:
	friend Singleton<SelectScene>;

public:
	SelectScene();
	~SelectScene();

	void Initialize();

	void Init();

	void Update();

	void Draw();

	void Direction();

	//フェードの数値をシーンマネージャーで変更するためのやつ	
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }

	float GetVolume() { return volume2; }
	float SetVolume(float volume) { return this->volume = volume; }

	bool SetAudioFlag(bool audioFlag) { return this->audioFlag = audioFlag; }


private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://定義
	SoundData sound1;
	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Bottan;
	SpriteData Number[10];
	SpriteData LoadUIGraph;
	SpriteData VolumeUI[3];

	//オブジェクト画像
	int graph2;
	int graph3;

	Vec2 pos = { 0.0f,0.0f };

	int direction = 0;


	//フェード
	float fade = 1;

	float volume = 1.0f;
	float volume2 = 1.0f;//他のシーン用ボリューム
	bool volumeFlag = false;

	int volumeB = 500;

	bool audioFlag = true;
};
