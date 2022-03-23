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

	int GetStage() { return stage; }

	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://��`
	//�X�v���C�g�f�[�^
	Sprite::SpriteData spriteGraph;
	Sprite::SpriteData BGGraph;
	Sprite::SpriteData Bottan;
	Sprite::SpriteData Number[10];
	//�I�u�W�F�N�g�摜
	int graph1;
	int graph2;
	int graph3;

	Vec2 pos = { 0.0f,0.0f };

	//�p�[�e�B�N��
	ParticleManager* particleMan = nullptr;

	ParticleManager* particleMan2 = nullptr;

	int stage = 1;
	int nCount = 1;
	int nCount2 = 0;
	float fade = 1;

};
