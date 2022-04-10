#pragma once
#include<vector>
#include"LoadCSV.h"
#include"Player.h"
#include"EnemyHelper.h"
#include"OniType.h"
#include"OniBow.h"


class Enemy
{
private:
	//�G�ő吔
	static const int eNumMax = 20;
	//�G�z��
	std::vector<EnemyData *>eData;

public:
	Enemy();//�R���X�g���N�^

	~Enemy();//�ł��R���X�g���N�^

	void Init();//������

	//�X�e�[�W���Ƃ̏�����
	void StageInit(int stageNum);

	void Update(class Player *player);//�X�V

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
	void DamegeNormal(int i);

	void DamegeSword(int i);
private:
	//�폜
	void Delete();
	//�G�������Ă������
	int Direction(int i, class Player *player);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
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

	bool GetParticleFlag(int i) { return particleFlag[i]; }
	int GetParticleTime(int i) { return particleTime[i]; }

	bool GetDamegeFlag(int i) { return eData[i]->DamegeFlag; }
	bool SetDamegeFlag(int i, bool DamegeFlag);

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//�����n�̃}�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu
private://�G�l�~�[�P
	class OniType oniType;

	void UpdateOni(int i, class Player *player);
	//�G�l�~�[�|
	class OniBow oniBow;

	void UpdateBow(int i, class Player *player);
private:
	//HPUI
	Object::ObjectData hpOBJ;
	Object::ObjectData hpGaugeOBJ;
	int hpGaugeGraph;
	int hpGraph;

	int redColor;
	//���֌W�̕ϐ�
	Object::ObjectData Blood;
	int BloodGraph;
	bool BloodFlag[eNumMax] = { false };
	Vec3 BloodPosition[eNumMax] = { Vec3(0,0,0) };

	int particleTime[eNumMax];
	bool particleFlag[eNumMax];
};