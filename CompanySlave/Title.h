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

extern const int window_width;
extern const int window_height;
class Title :public Singleton<Title>
{
private:
	friend Singleton<Title>;
	_DirectX *directX = nullptr;
public:
	Title();
	~Title();

	void Initialize(_DirectX *directX);

	void Init();

	void Update();

	void Draw();

	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }
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

	//オブジェクトデータ
	Vec2 pos = { 0.0f,0.0f };


	int scene = 0;
	float fade = 1;
};


