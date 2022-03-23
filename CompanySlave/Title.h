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
private://��`

	//���f�[�^
	Audio::SoundData sound1;
	Audio::SoundData sound2;

	//�X�v���C�g�f�[�^
	Sprite::SpriteData spriteGraph;
	Sprite::SpriteData BGGraph;
	Sprite::SpriteData Parent;

	//�I�u�W�F�N�g�f�[�^
	Vec2 pos = { 0.0f,0.0f };

	//�p�[�e�B�N��
	ParticleManager *particleMan = nullptr;

	ParticleManager *particleMan2 = nullptr;

	int scene = 0;
	float fade = 1;
};


