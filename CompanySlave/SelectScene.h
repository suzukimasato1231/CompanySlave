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

	//�X�e�[�W�̐����V�[���}�l�[�W���[�Ŏg�����߂̂��
	int GetStage() { return stage; }
	//�t�F�[�h�̐��l���V�[���}�l�[�W���[�ŕύX���邽�߂̂��	
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://��`
	//�X�v���C�g�f�[�^
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Bottan;
	SpriteData Number[10];
	SpriteData LoadUIGraph;

	//�I�u�W�F�N�g�摜
	int graph2;
	int graph3;

	Vec2 pos = { 0.0f,0.0f };


	int stage = 1;
	//��̈�
	int nCount = 1;
	//�\�̈�
	int nCount2 = 0;
	//�t�F�[�h
	float fade = 1;



};
