#pragma once
#pragma once
#include"EnemyHelper.h"
#include<time.h>
class Player;

/// <summary>
/// �S�ߐڃN���X
/// </summary>
class BigOniBoss
{
private:
	enum BigStatus
	{
		PREOPERATION,//�\������
		BIGATTACK,	//�U��
		BIGAFTER,//�U����
	};
public:
	BigOniBoss();
	~BigOniBoss();
	//������
	void Init();

	void Draw(EnemyData* oniData);
	//���G
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//�Ō���
	void AttackSmall(EnemyData* oniData, class Player* player);
	//�Ō���
	void AttackBig(EnemyData* oniData, class Player* player);
	//�����U��
	void AttackSummon(EnemyData* oniData, class Player* player, int eNum);
	//�{�X���l��
	EnemyData GetBossData() { return bossData; }

	bool GetFirstSummonFlag() { return firstSummonFlag; }
private:
	//�G�̍U���G�t�F�N�g
	void EffectDraw(EnemyData* oniData);

	//�����ɂ���č��G�͈͂̌���
	Box SearchField(EnemyData* oniData);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);

	//�����ɂ���čU���͈͂̌���
	Box AttackField(EnemyData* oniData);

	Box AttackBIG(EnemyData* oniData);
private:
	//�{�X�X�e�[�^�X
	EnemyData bossData;

	//�ǂ̍U�������Ă��邩(�傩�����������Ƃ��ǂ���)
	bool attackTypeFlag = false;

	time_t start_time, end_time;	//���Ԍv��



	//�{�X�s���X�e�[�^�X
	//���G
	const Vec2 attackEnemies{ 300.0f,110.0f };		//���G�͈�:width,height
	const float player2EnemyLength = 500.0f;		//���G����
	//���U��
	const float attackSmallLength = 30.0f;			//�U���͈�
	const Vec2 attackField{ 30.0f,30.0f };			//�U���͈�
	const float moveSpeed = 0.30f;					//�G�̃X�s�[�h
	const int attackSmallTimeMax = 200;				//�U�����ԍő�
	int attackSmallTime = 0;						//�U������
	const int attackMotionDamege = 50;				//�U�����Ԓ��Ƀ_���[�W��^���鎞��
	int attackSmallNum = 0;							//���U��������s������
	time_t small_start, small_end;
	//��U��
	time_t big_start, big_end;						//�\�����쎞��
	const Vec2 attackBigField{ 40.0f,40.0f };		//�U���͈�
	int attackBigStatus = 0;						//�\������J�n
	//����
	bool firstSummonFlag = false;



	//�{�XOBJ
	Object::ObjectData enemyObject[3];					//�v���C���[�I�u�W�F�N�g
	Object::ObjectData attackOBJ[2];				//�G�l�~�[�A�^�b�N
	Object::ObjectData nockBackOBJ[2];				//�m�b�N�o�b�N�I�u�W�F�N�g
	//�f�o�b�N�p
	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈͏�
	Object::ObjectData attackBigOBJ;//�U����͈͕̔\��
	int redColor;

	//�G�l�~�[�A�^�b�N
	Object::ObjectData AttackEffectOBJ;//�ʏ�U���G�t�F�N�g
	int AttackEffectGraph[9];
	//�G�t�F�N�g�֌W

	float slowValue = 1;

	//���֌W
	bool attakAFlag = false;
	int attakCount = 0;
};

