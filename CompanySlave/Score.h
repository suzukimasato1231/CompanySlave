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

	//�t�F�[�h�̐��l���V�[���}�l�[�W���[�ŕύX���邽�߂̂��
	float GetFade() { return fade; };
	float SetFade(float fade) { return this->fade = fade; }
	
	int GetHiScore() { return HiScore; }
	int SetHiScore(int HiScore) { return this->HiScore = HiScore; }
private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://��`

	//���f�[�^
	SoundData sound1;
	SoundData sound2;

	//�X�v���C�g�f�[�^
	SpriteData spriteGraph;
	SpriteData BGGraph;
	SpriteData Parent;

	//�I�u�W�F�N�g�f�[�^
	Object::ObjectData Polygon;
	Object::ObjectData rayPolygon;
	Object::ObjectData BossPolygon;

	//�I�u�W�F�N�g�摜
	int graph1;
	int graph2;
	int graph3;

	Vec3 pPos1 = { 1.0f,1.0f,0.0f };

	Vec3 pPos2 = { 2.0f,4.0f,5.0f };

	Vec3 angle = { 0.0f,0.0f,0.0f };

	Vec3 angle2 = { 0.0f,0.0f,0.0f };

	Vec2 pos = { 0.0f,0.0f };

	//�}�b�v�`�b�v
	MapStage* mapStage = nullptr;

	//�v���C���[
	Player* player = nullptr;

	//�G�l�~�[
	Enemy* enemy = nullptr;

	//FBX
	Model* model1 = nullptr;
	FBXObject3d* fbxObject1 = nullptr;

	//�t�F�[�h
	float fade = 1;

	//�n�C�X�R�A
	int HiScore = 0;
};


