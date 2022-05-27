#pragma once
#pragma once
#include "Object.h"
#include"Collision.h"
#include"Direction.h"
enum EnemyType
{
	Oni = 1,	//�ߐڋS
	OniBow,		//�|�S
	WolfType,	//�T,
	BoarType,	//�C�m�V�V
	BossBigOni,	//����S
	BossWolfFlock,//�T�Q��
	BossTwinBoar,
};

enum spawnNo
{
	ONIUP = 10,
	ONIDOWN,
	ONILEFT,
	ONIRIGHT,
	ONIBOWUP = 20,
	ONIBOWDOWN,
	ONIBOWLEFT,
	ONIBOWRIGHT,
	WOLFUP = 30,
	WOLFDOWN,
	WOLFRIGHT,
	WOLFLEFT,
	BOARUP = 40,
	BOARDOWN,
	BOARLEFT,
	BOARRIGHT,
	BossOni = 50,
	WOLFFLOCK = 60,
	TWINBOAR = 70,
};

//�G�̏��
enum Status
{
	NORMAL, //�ʏ���
	MOVE,	//����
	ATTACK,	//�U��
	ENEMIES,//�G����������
	NOCKBACK,//�m�b�N�o�b�N��
	//�{�X�̓���
	BOSSATTACK,		//�{�X�U���P
	BOSSATTACK2,	//�{�X�U���Q
	BOSSATTACK3,	//�{�X�U���R
	SUMMON			//��������
};

struct EnemySupport
{
	//�m�b�N�o�b�N�X�e�[�^�X
	static const int nockBackTimeMax = 5;
};

//�G�̏��
struct EnemyData
{
	Box eBox;							//�G�{�b�N�X
	Sphere eSphere;						//�G��
	Vec3 position{ 0.0f,5.0f,0.0f };	//���W
	Vec3 oldPosition{};					//1�O�̍��W
	Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
	Vec3 angle{};						//�`��p�x
	float HPMax = 7;					//�ő�HP
	float HP = 7;						//HP
	float subHP = 7;					//�����ڕύX�ۑ��pHP
	float r = 5;						//�傫��
	int Status = NORMAL;				//���
	int memoryStatus = NORMAL;
	int StatusTime = 0;					//��Ԏ���
	int direction = Down;				//�����Ă������
	int attackDirection = Down;			//�U���̌���
	int damegeTime = 0;					//�_�Ŏ���
	bool DamegeFlag = false;			//�_���[�W���󂯂���
	bool attakFlag = false;				//�A�^�b�N���̉��p�t���O
	Vec3 pDirection = {};				//�v���C���[�̂������
	int type = 0;						//�G�̎��
	bool bossFlag = false;				//�{�X���ǂ���
	//�m�b�N�o�b�N���l
	int nockbackTime = 0;				//�m�b�N�o�b�N����
	int nockDirection = 0;				//�m�b�N�o�b�N�������
	bool nockPossibleFlag = true;		//�m�b�N�o�b�N���\���ǂ���
	//�������p���l
	float bowAngle = 0.0f;				//�_���p�x
	Vec3 bowPos = {};					//��̍��W
	Vec3 bowOldPos = {};
	bool bowFlag = false;				//�|����������
	bool bowMove = false;
	int bowTime = 0;					//����ł�������
	bool bowAFlag = false;				//�|�̉��p�t���O
	//�T
	bool attakWFlag = false;			//�A�^�b�N���̉��p�t���O
	bool RushWFlag = false;				//�ːi���̃G�t�F�N�g�p�t���O
	//��
	bool attakBFlag = false;			//�A�^�b�N���̉��p�t���O
	bool RushFlag = false;				//�ːi���̃G�t�F�N�g�p�t���O
	//�j�̂��
	bool explosionFlag = false;
	int explosionCount = 3;
	bool explosionDelay = 0;

	//�`��֌W
	int walkNum = 0;					//����
	int walkTime = 0;

	//FallDown
	float fallDownTime = 0;
};