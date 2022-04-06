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
class PlayScene :public Singleton<PlayScene>
{
private:
	friend Singleton<PlayScene>;
public:
	PlayScene();
	~PlayScene();

	void Initialize();

	void Init();

	void StageInit();

	void Update();

	void Draw();

	bool GetSceneFlag();
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
	Sprite::SpriteData controlGraph;
	Sprite::SpriteData GameOverGraph;
	//�I�u�W�F�N�g�f�[�^

	//�I�u�W�F�N�g�摜
	int graph1;
	int graph2;
	int graph3;


	Vec2 pos = { 0.0f,0.0f };
	Vec2 deadGraphPos = { 0.0f,-800.0f };
	float nowTime = 0;//���[�v
	float endTime = 5;//���[�v
	float timeRate = 0;//���[�v

	//�}�b�v�`�b�v
	MapStage *mapStage = nullptr;

	//�v���C���[
	Player *player = nullptr;

	//�G�l�~�[
	Enemy *enemy = nullptr;

	//�p�[�e�B�N��
	ParticleManager *particleMan = nullptr;

	ParticleManager *particleMan2 = nullptr;
	ParticleManager *particleMan3 = nullptr;
	ParticleManager *particleMan4 = nullptr;
	//FBX
	Model *model1 = nullptr;
	FBXObject3d *fbxObject1 = nullptr;

	int stageNum = 1;

	bool stageFlag = true;

	bool sceneFlag = false;
};