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
	void Draw();
	
	bool GetBottanFlag() { return bottanFlag; }
	float GetVolume() { return volume; }
	float SetVolume(float volume) { return this->volume = volume; }
	bool SetAudioFlag(bool audioFlag) { return this->audioFlag = audioFlag; }

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
	//オブジェクトデータ
	Object::ObjectData swordObject;
	Vec3 position{ 5.0f,0.0f,0.0f };
	
	std::array<Vec2, rainMax > pos;//ポジション
	std::array<float, rainMax > s;//サイズ

	float volume = 1.0f;

	float v = 0;//速度
	float g = 9.8f;//重力

	int titleTime = 0;
	int rainTime = 0;

	bool rainFlag = false;
	bool titleTextFlag = false;


	int scene = 0;
	float fade = 0;
	float fade2 = 0;

	bool bottanFlag = false;
	bool audioFlag = true;
};


