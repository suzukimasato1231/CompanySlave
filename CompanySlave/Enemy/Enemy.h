#pragma once
#include<vector>
#include"LoadCSV.h"
#include"Player.h"
#include"EnemyHelper.h"
#include"OniType.h"
#include"OniBow.h"
#include"WOLF.h"
#include"BigOniBoss.h"
#include"WolfFlock.h"
#include"Boar.h"
#include "Audio.h"
#include"TwinBoar.h"
class Enemy
{
private:
	//�G�ő吔
	static const int eNumMax = 100;
	//�G�z��
	std::vector<EnemyData*>eData;

public:
	Enemy();//�R���X�g���N�^

	~Enemy();//�ł��R���X�g���N�^

	void Init();//������

	//�X�e�[�W���Ƃ̏�����
	void StageInit(int stageNum);

	void Update(class Player* player);//�X�V

	void Draw();//�`��

	void PreDraw();

	void DrawUI();//UI�`��
	//�����`��
	void FirstDraw();
	//�G�̈ʒu��ݒ�
	void SetPosition(int i, Vec3 position);

	/// <summary>
	/// �v���C���[�̒ʏ�U���_���[�W
	/// </summary>
	void DamegeNormal(int i, int pAttackDirection);

	/// <summary>
	/// ���̃_���[�W����
	/// </summary>
	/// <param name="i"></param>
	void DamegeThrowSword(int i);
	/// <summary>
	/// ���̃_���[�W�߂�
	/// </summary>
	/// <param name="i"></param>
	void DamegeSword(int j, int i);

	void DrawBlood();
private:
	//�폜
	void Delete();
	//�G�������Ă������
	int Direction(int i, class Player* player);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
	//�m�b�N�o�b�N����
	void NockBack(int i);

	void SetFirstPosition(Vec3 pos, float r, int eNum);
	//���ꂽ��|��鉉�o
	void FallDown(int eNum);
public://�擾�n
	// ���W�擾
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Vec3 GetBowPosition(int i) { return eData[i]->bowPos; }

	Vec3 GetBowOldPos(int i) { return eData[i]->bowOldPos; }

	Vec3 SetBowPos(int i, Vec3 pos) { return eData[i]->bowPos = pos; }

	void SetBowFlag(int i) { eData[i]->bowFlag = false, eData[i]->bowMove = false; }

	int GetStatus(int i) { return eData[i]->Status; }

	int GetType(int i) { return eData[i]->type; }

	bool GetBossFlag(int i) { return eData[i]->bossFlag; }

	int GetBowAngle(int i) { return eData[i]->bowAngle; }

	float GetHP(int i) { return eData[i]->HP; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//�G�l�~�[���擾
	int GetEnemySize() { return eData.size(); }

	float GetEnemyR(int i) { return eData[i]->r; }

	int GetTime(int i) { return BloodTime[i]; }

	bool GetDamegeFlag(int i) { return eData[i]->DamegeFlag; }
	bool SetDamegeFlag(int i, bool DamegeFlag);

	bool GetExplosionFlag(int i) { return eData[i]->explosionFlag; }
	bool SetExplosionFlag(int i);

	int GetExplosionCount(int i) { return eData[i]->explosionCount; }
	bool SetExplosionCount(int i);
	
	bool GetRushFlag(int i) { return eData[i]->RushFlag; }

	float SetVolume(float volume) { return this->volume = volume; };

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//�����n�̃}�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu

	float TwinBoarHP() { return twinTotalHP; }

	bool SetReturnDamageFlag(int j, int i);
private:
	//�G�l�~�[�P
	class OniType oniType;

	void UpdateOni(int i, class Player* player);
	//�G�l�~�[�|
	class OniBow oniBow;

	void UpdateBow(int i, class Player* player);

	//�T
	class Wolf wolf;

	void UpdateWolf(int i, class Player* player);
	//�{�X�S
	class BigOniBoss bigOniBoss;

	void UpdateBigOniBoss(int i, class Player* player);

	//�T�̌Q��
	class WolfFlock wolfFlock;

	void UpdateWolfFlock(int i, class Player* player);

	//�C�m�V�V
	class Boar boar;

	void UpdateBoar(int i, class Player* player);

	//�_�u���C�m�V�V�{�X
	class TwinBoar twinBoar;

	void UpdateTwinBoar(int i, class Player* player);

	float twinTotalHP = 0;

private:
	const float nockPower = 0.8f;
	//HPUI
	Object::ObjectData hpOBJ;
	Object::ObjectData hpGaugeOBJ;
	Object::ObjectData hpSubOBJ;
	int hpSub;
	Object::ObjectData explosionOBJ;
	int hpGaugeGraph;
	int hpGraph;
	int explosionGraph;


	//�{�XUI
	SpriteData bossSprite;
	SpriteData bossHPSprite;
	SpriteData bossSubHPSPrite;
	int redColor;
	//���֌W�̕ϐ�

	Object::ObjectData Blood;	//����
	int BloodGraph;
	bool BloodFlag[eNumMax] = { false };
	Vec3 BloodPosition[eNumMax] = { Vec3(0,0,0) };

	Object::ObjectData BloodFrount;	//���ł��錌
	Object::ObjectData BloodBack;

	int Blood2Graph[3];
	int Blood2GraphBack[3];
	int Blood2Flag[eNumMax];
	int BloodTime[eNumMax];
	const int bloodTimeMax = 30;//��юU�錌�̉摜���o�Ă��鎞��


	//�j
	int delayCount[eNumMax];
	int explosionGraphCnt[eNumMax];
	bool explosionFlag[eNumMax] = { false };
	Vec3 explosionPosition[eNumMax] = { Vec3(0,0,0) };

	//�A���Ă���Ƃ��̔����񂾂����悤�ɂ���
	bool returnDamageFlag[7][eNumMax];

	//��
	Audio* audio = nullptr;
	//���f�[�^
	SoundData sound1;//����_
	SoundData sound2;//�|
	SoundData sound3;//�T
	SoundData sound4;//��
	SoundData sound5;//�{�X
	float volume = 1.0f;
	//�|��鏈���̎��Ԍv��
	const float fallDownTimeMax = 90.0f;
	
	int audioBoss = 5;
};