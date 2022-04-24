#pragma once
#include<vector>
#include"LoadCSV.h"
#include"Player.h"
#include"EnemyHelper.h"
#include"OniType.h"
#include"OniBow.h"
#include"WOLF.h"

class Enemy
{
private:
	//�G�ő吔
	static const int eNumMax = 20;
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
	//�����`��
	void BloodDraw();

	//TRUE�ɂ���
	void WasAttack(int i);
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
	void DamegeSword(int i);
private:
	//�폜
	void Delete();
	//�G�������Ă������
	int Direction(int i, class Player* player);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
	//�m�b�N�o�b�N����
	void NockBack(int i);
public://�擾�n
	// ���W�擾
	Vec3 GetPosition(int i) { return eData[i]->position; }

	float GetHP(int i) { return eData[i]->HP; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//�G�l�~�[���擾
	int GetEnemySize() { return eData.size(); }
	//�U�����ꂽ�����擾
	//bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }

	int GetTime(int i) { return BloodTime[i]; }

	bool GetDamegeFlag(int i) { return eData[i]->DamegeFlag; }
	bool SetDamegeFlag(int i, bool DamegeFlag);

	bool GetExplosionFlag(int i) { return eData[i]->explosionFlag; }
	bool SetExplosionFlag(int i);

	bool GetExplosionCount(int i) { return eData[i]->explosionCount; }
	bool SetExplosionCount(int i);

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//�����n�̃}�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu
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

private:
	//�m�b�N�o�b�N�X�e�[�^�X
	const float nockPower = 0.8f;
	const int nockBackTimeMax = 5;
	//HPUI
	Object::ObjectData hpOBJ;
	Object::ObjectData hpGaugeOBJ;
	Object::ObjectData explosionOBJ;
	int hpGaugeGraph;
	int hpGraph;
	int explosionGraph;

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
};