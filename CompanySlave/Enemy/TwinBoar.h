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

	//�|���`��
	void FallDownDraw(EnemyData* oniData);

	void Draw(EnemyData* oniData,int i);

	void PreDraw(EnemyData* oniData, int i);

	void LoopInit();
	//���G
	void SearchPlayer(EnemyData* oniData, class Player* player);

	int  SearchStatus() { return BOSSATTACK; }
	//�ːi�i���݁j
	void AttackRush(EnemyData* oniData, class Player* player, int num);
	//�ːi�i�Q�̓����j
	void AttackDoubleRush(EnemyData* oniData, class Player* player);
	//�Z���ːi
	void AttackShortRush(EnemyData* oniData, class Player* player, int ty);
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

	const float moveSpeed = 0.60f;					//�G�̃X�s�[�h

	time_t start_time, end_time;					//���Ԍv��

	const int boarNumMax = 2;						//�C�m�V�V�̐�
	int boarNum = 2;

	//�{�X�s���X�e�[�^�X
	//���G
	const Vec2 attackEnemies{ 300.0f,110.0f };		//���G�͈�:width,height
	const float player2EnemyLength = 500.0f;		//���G����
	bool searchEnemyFlag = false;					//�Е��ł������������ǂ���
	const Vec2 attackField{ 15.0f,15.0f };
	//�s�����s������ʒu
	const Vec3 fixedPosition[2] = { {250.0f,0.0f,-200.0f} ,{750.0f,0.0f,-200.0f} };
	float moveFixedSpeed = 1.2f;
	//���ːi
	const float sPlayer2EnemyLength = 50.0f;		//���ːi���s������
	const int sAttackMotionTime = 120;				//�U�����[�V�����S��
	const int sAttackHoldTime = 50;					//�U���\���鎞��
	const float sAttackSpeed = 1.3f;				//�U���X�s�[�h
	bool attackFlag = false;
	const int sTimeMax = 300;						//���U�������Ă�������/2
	int sTime = 0;
	bool sAttack[2] = { false,false };
	//2�̓���
	const int doublePreTimeMax = 100;				//�\�����쎞��
	int doublePreTime = 0;
	const int doubleTimeMax = 500;					//�����ːi���s������
	int doubleTime = 0;
	const int doubleAfterTimeMax = 50;				//�U����̍d������
	int doubleAfterTime = 0;
	int attackStats = PREOPERATION;
	const float dAttackSpeed = 4.5f;				//�U���X�s�[�h
	const int dAttackTimeMax = 100;					//�U������
	int dAttackTime = 0;
	//���ݓːi
	const int bPreTimeMax = 50;
	int bPreTime = 0;
	const int bTimeMax = 300;						//���ݓːi���s������
	int bTime = 0;
	const Vec2 attackAltField{ 40.0f,40.0f };		//�U���͈�
	int attackAltStatus = 0;						//�\������J�n
	bool altFlag = false;							//2�̂̂����ǂ��炩���ːi���
	int altAttackNum = 0;							//���ݓːi��A���ŉ���������
	bool bFlag[2] = { false,false };
	bool bFinishFlag[2]{ false,false };

	//�{�XOBJ
	Object::ObjectData enemyObject[5];					//�v���C���[�I�u�W�F�N�g
	Object::ObjectData attackOBJ[5];					//�G�l�~�[�A�^�b�N
	//�f�o�b�N�p
	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈͏�
	Object::ObjectData attackBigOBJ;//�U����͈͕̔\��
	int redColor;
	//�G�t�F�N�g�֌W

	bool AttackSound = false;


	int SoundCount = 0;

	float slowValue = 1;
};

