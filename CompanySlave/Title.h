#pragma once
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
#include<array>
#include<time.h>
#include "Direction.h"

extern const int window_width;
extern const int window_height;
class Title :public Singleton<Title>
{
private:
	friend Singleton<Title>;

public:
	Title();
	~Title();

	void Initialize();

	void Init();

	void Update();

	void AudioUpdate();

	void Direction();

	void Draw();
	
	bool GetBottanFlag() { return bottanFlag; }
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }

	float GetVolume() { return volume2; }
	float SetVolume(float volume) { return this->volume = volume; }

	bool SetAudioFlag(bool audioFlag) { return this->audioFlag = audioFlag; }
	int GetVolumeFlag() { return volumeFlag; }

	bool GetSceneChangeFlag() { return sceneChangeFlag; }


private:
	Camera *camera = nullptr;
	LightGroup *lightGroup = nullptr;
	DebugText debugText;
	Audio *audio = nullptr;
private://定義

	//音データ
	SoundData sound1;
	SoundData sound2;

	static const int rainMax = 30;

	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Parent;
	SpriteData titleGraph;
	SpriteData startGraph; 
	SpriteData rain[rainMax];
	SpriteData Bottan[2];
	SpriteData Number[10];
	SpriteData VolumeUI[2];

	//オブジェクトデータ
	Object::ObjectData swordObject;
	Vec3 position{ 5.0f,0.0f,0.0f };
	
	std::array<Vec2, rainMax > pos;//ポジション
	std::array<float, rainMax > s;//サイズ



	float v = 0;//速度
	float g = 9.8f;//重力

	int titleTime = 0;
	int rainTime = 0;

	bool rainFlag = false;
	bool titleTextFlag = false;


	int scene = 0;


	bool bottanFlag = false;
	bool audioFlag = true;
	Vec2 pos2 = { 0.0f,0.0f };

	int direction = 0;


	float volume = 1.0f;
	float volume2 = 1.0f;//他のシーン用ボリューム
	int volumeFlag = 0;

	int volumeB = 500;

	float fade = 0;
	float fade2 = 0;

	bool sceneChangeFlag = false;

};


