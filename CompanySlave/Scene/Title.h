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
	
	bool GetButtonFlag() { return ButtonFlag; }
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
	SpriteData Button[2];
	SpriteData Number[10];
	SpriteData VolumeUI[2];

	//オブジェクトデータ
	Object::ObjectData swordObject;
	Vec3 position = { 5.0f,0.0f,0.0f };
	Vec3 rotation = { 0,-90,-90 };
	Vec3 size = { 1,1,1 };
	std::array<Vec2, rainMax > pos;//ポジション
	std::array<float, rainMax > s;//サイズ



	float v = 0;//速度
	float g = 9.8f;//重力

	int titleTime = 0;
	int rainTime = 0;

	bool rainFlag = false;
	bool titleTextFlag = false;


	int scene = 0;


	bool ButtonFlag = false;
	bool audioFlag = true;
	Vec2 pos2 = { 0.0f,0.0f };

	int direction = 0;


	float volume = 0.5f;
	float volume2 = 0.5f;//他のシーン用ボリューム
	int volumeFlag = 0;

	bool volumeArrowFlag = false;


	int volumeB = 500;

	float fade = 0;
	float fade2 = 0;

	bool sceneChangeFlag = false;

	bool volumeFadeFlag = false;
	float volumeFade = 0;
	bool ControllerLFlag = false;
	bool ControllerRFlag = false;
};


