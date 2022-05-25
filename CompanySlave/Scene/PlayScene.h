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
#include"MapStage.h"
#include"Player.h"
#include"PushCollision.h"
#include"Enemy.h"
#include<array>
#include"../ParticleAdd.h"

extern const int window_width;
extern const int window_height;
class PlayScene :public Singleton<PlayScene>
{
private:
	friend Singleton<PlayScene>;
public:
	PlayScene();
	~PlayScene();
	//�P�񂾂�
	void Initialize();

	void Init();

	void StageInit();

	void Update();

	void Draw();
	void LoadDraw();

	bool GetSceneFlag();
	bool SetSceneFlag(bool sceneFlag) { return this->sceneFlag = sceneFlag; }
	bool GetSceneChangeFlag() { return sceneChangeFlag; }
	int GetStageNum() { return stageNum; }
	void SetStageDebug(int debugNum) { this->stageNum = debugNum; }
	float SetVolume(float volume) { return this->volume = volume; }
	bool GetUpdateFlag() { return UpdateFlag; }
	bool SetUpdateFlag(bool UpdateFlag) { return this->UpdateFlag = UpdateFlag; }
	bool SetLoadFlag(bool LoadFlag) { return this->LoadFlag = LoadFlag; }

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
	SpriteData controlGraph;
	SpriteData GameOverGraph;
	SpriteData GameOverTextGraph;
	SpriteData SChangeGraph;


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
	MapStage* mapStage = nullptr;

	//�v���C���[
	Player* player = nullptr;

	//�G�l�~�[
	Enemy* enemy = nullptr;

	ParticleAdd* particleAdd = nullptr;

	int stageNum = 1;
	int deathTime = 20;
	int swordTime = 10;
	float fade = 0.0f;
	float fade2 = 0.0f;
	float color = 0.0f;
	bool stageFlag = true;

	bool sceneFlag = false;
	bool sceneChangeFlag = false;
	Vec2 ChangeGraphPosition = { -2000.0f, 0.0f };
	//int sceneChangeTime = 60;

	float volume = 1.0f;
	bool audioFlag = false;
	bool UpdateFlag = false;

	//�J�֌W
	static const int rainMax = 80;//OBJ�̌�
	std::array<Object::ObjectData, rainMax > RainOBJ;
	int rainGraph;//�J�p�̃e�N�X�`��
	std::array<Vec3, rainMax > position;//�|�W�V����
	std::array<float, rainMax > s;//�T�C�Y
	

	float v = 0;//���x
	float g = 9.8f;//�d��
	float rainSlow = 1.0f;//�J�̃X���[
	Object::ObjectData tutorial;
	Object::ObjectData tutorial2;
	bool tutorialAFlag = false;
	int AButtonTimer = 30;
	int AButtonCount = 0;
	bool tutorialXFlag = false;
	int XButtonTimer = 30;
	int XButtonCount = 0;
	bool tutorialRBFlag = false;
	int RBButtonTimer = 30;
	int RBButtonCount = 0;
	bool tutorialLBFlag = false;
	int LBButtonTimer = 30;
	int LBButtonCount = 0;

	int AButton[2];
	int LBButton[2];
	int RBButton[2];
	int XButton[2];

	bool LoadFlag = false;

	Object::ObjectData sword;
	Vec3 positionS = { -20.0f,5.0f,-270.0f };

	Object::ObjectData sword2;
};