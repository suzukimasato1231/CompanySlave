#pragma once
#pragma once
#include"EnemyHelper.h"
#include<time.h>
class Player;

/// <summary>
/// �C�m�V�V
/// </summary>
class TwinBoar
{
private:
	enum BigStatus
	{
		PREOPERATION,//�\������
		DOUBLEATTACK,	//�U��
		DOUBLEAFTER,//�U����
	};
public:
	TwinBoar();
	~TwinBoar();
	//������
	void Init();

	void Draw(EnemyData* oniData);

	void LoopInit();
	//���G
	void SearchPlayer(EnemyData* oniData, class Player* player);

	int  SearchStatus() { return BOSSATTACK; }
	//�ːi�i���݁j
	void AttackRush(EnemyData* oniData, class Player* player,int num);
	//�ːi�i�Q�̓����j
	void AttackDoubleRush(EnemyData* oniData, class Player* player);
	//�Z���ːi
	void AttackShortRush(EnemyData* oniData, class Player* player);
	//�{�X���l��
	EnemyData GetBossData() { return bossData; }

	bool GetSerachEnemyFlag() { return searchEnemyFlag; }
private:
	//�����ɂ���č��G�͈͂̌���
	Box SearchField(EnemyData* oniData);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
private:
	//�{�X�X�e�[�^�X
	EnemyData bossData;

	const float moveSpeed = 0.30f;					//�G�̃X�s�[�h

	time_t start_time, end_time;	//���Ԍv��

	const int boarNumMax = 2;//�C�m�V�V�̐�
	int boarNum = 2;

	//�{�X�s���X�e�[�^�X
	//���G
	const Vec2 attackEnemies{ 300.0f,110.0f };		//���G�͈�:width,height
	const float player2EnemyLength = 500.0f;		//���G����
	bool searchEnemyFlag = false;					//�Е��ł������������ǂ���

	const Vec2 attackField{ 15.0f,15.0f };
	//���ːi
	const float sPlayer2EnemyLength = 50.0f;		//���ːi���s������
	const int sAttackMotionTime = 100;				//�U�����[�V�����S��
	const int sAttackHoldTime = 50;					//�U���\���鎞��
	const float sAttackSpeed = 0.8f;				//�U���X�s�[�h
	bool attackFlag = false;
	//2�̓���
	time_t d_start, d_end;							//�\�����쎞��
	int attackBigStatus = 0;						//�\������J�n
	int attackStats = PREOPERATION;
	const float dAttackSpeed = 1.2f;				//�U���X�s�[�h
	const int dAttackTimeMax = 100;					//�U������
	int dAttackTime = 0;

	//���ݓːi
	time_t alt_start, alt_end;						//�\�����쎞��
	const Vec2 attackAltField{ 40.0f,40.0f };		//�U���͈�
	int attackAltStatus = 0;						//�\������J�n
	bool altFlag = false;							//2�̂̂����ǂ��炩���ːi���
	int altAttackNum = 0;							//���ݓːi��A���ŉ���������

	//�{�XOBJ
	Object::ObjectData enemyObject[3];					//�v���C���[�I�u�W�F�N�g
	Object::ObjectData attackOBJ[2];				//�G�l�~�[�A�^�b�N
	Object::ObjectData nockBackOBJ[2];				//�m�b�N�o�b�N�I�u�W�F�N�g
	//�f�o�b�N�p
	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈͏�
	Object::ObjectData attackBigOBJ;//�U����͈͕̔\��
	int redColor;
	//�G�t�F�N�g�֌W

	float slowValue = 1;
};

