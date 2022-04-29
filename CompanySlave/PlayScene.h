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

	bool GetSceneFlag();

	void SetStageDebug(int debugNum) { this->stageNum = debugNum; }
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



	//�p�[�e�B�N��
	//�G���񂾎��̃p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	//���s�p�[�e�B�N��	
	ParticleManager* particleMan2 = nullptr;
	ParticleManager* particleMan3 = nullptr;
	ParticleManager* particleMan4 = nullptr;
	ParticleManager* particleMan5 = nullptr;



	int stageNum = 1;
	float fade = 1.0f;
	bool stageFlag = true;

	bool sceneFlag = false;
	bool sceneChangeFlag = false;
	Vec2 ChangeGraphPosition = { -1600.0f, 0.0f };
	//int sceneChangeTime = 60;

	float volume = 1.0f;
	
	//�J�֌W
	static const int rainMax = 80;//OBJ�̌�
	std::array<Object::ObjectData, rainMax > RainOBJ;
	int rainGraph;//�J�p�̃e�N�X�`��
	std::array<Vec3, rainMax > position;//�|�W�V����
	std::array<float, rainMax > s;//�T�C�Y
	

	float v = 0;//���x
	float g = 9.8f;//�d��
	float rainSlow = 1.0f;//�J�̃X���[
};