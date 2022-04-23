#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
#include"Sprite.h"
#include"Direction.h"
#include <array>
/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
private:

public:

	Player();//�R���X�g���N�^

	~Player();//�ł��R���X�g���N�^

	void Init();//������

	void StageInit(int stageNum);//�X�e�[�W���Ƃ̏�����

	void Update(class Enemy* enemy);//�X�V

	void Draw();//�`��

	void SetPosition(Vec3 position) { this->position = position; }

	void SetSwordAttack(int i) { this->isSwordAttack[i] = false; }

	void SetSwordReverse(float reverseValue, int i) { this->reverseValue[i] = reverseValue; }

	void SetSwordAngle(Vec3 swordAngle, int i) { this->swordAngle[i] = swordAngle; }

	void SetSwordStop(int i) { this->swordStop[i] = true; }

	int GetHP() { return HP; }
	/// <summary>
	/// �_���[�W
	/// </summary>
	void Damage();
	//�v���C���[UI�`��
	void UIDraw();
private:
	/// <summary>
	/// �v���C���[�ړ�
	/// </summary>
	void Move();

	//�ʏ�U��
	void NormalAttack(class Enemy* enemy);

	//���U��
	void SwordAttack(class Enemy* enemy);

	//�U���p�x����
	void Angle();
	//���̌��������߂�
	void SwordAngle();

	//�v���C���[�̌��������߂�
	void PDirection();

	//���
	void Avoidance();

	/// <summary>
	/// �ʏ�U���͈̔͂����߂�
	/// </summary>
	void NormalFieldDirection();

	//�f�o�b�N�`��
	void DebugDraw();

	//�G�t�F�N�g�`��
	void EffectDraw();

	//�v���C���[�Ɉ�ԋ߂��G�����߂�
	int  EnemyNeedNumber(Enemy* enemy);
public://�擾�n
	//���W
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//�v���C���[��Box
	Box GetBox() { return pBox; }

	bool GetMoveFlag() { return moveFlag; }
	Vec3 GetAngle() { return angle; }
	//���W
	Vec3 GetSwordPosition(int No) { return swordPosition[No]; }

	Vec3 GetSwordAngle(int i) { return swordAngle[i]; }
	//�v���C���[��Box
	Box GetSwordBox(int No) { return swordAttackBox[No]; }

	float GetSlow() { return slowValue; }
	float GetenemyDamegeTime(int i) { return enemyDamegeTime[i]; }
	int GetDirection() { return direction; }
private:
	Object::ObjectData playerSwordWalkObject[4];	//�v���C���[�����I�u�W�F�N�g(������)
	Object::ObjectData playerAttackObject[9];	//�v���C���[�U��
	int walkCount = 0;	//�`��p�J�E���g
	int walkNo = 0;	//�`�悷��NO
	int attackCount = 0;	//�`��p�J�E���g
	int attackNo = 0;	//�`�悷��NO
	bool attackMode = false;
	Box pBox;							//�v���C���[�̒����`
	Sphere pSphere;						//�v���C���[�̋�
	Vec3 position{ 10.0f,0.0f,-10.0f };	//���W
	Vec3 oldPosition{};					//1�O�̍��W
	Vec3 speed{ 1.5f,1.5f,1.5f };		//�v���C���[�X�s�[�h
	Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
	Vec3 angle{ 0.0f,0.0f,0.0f };		//�p�x
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
	float r = 3;						//�v���C���[�̔��a
	int  direction = 0;					//�v���C���[�̌���
	float sinRad = 0;
	float cosRad = 0;
	float RsinRad = 0;
	float RcosRad = 0;
	Vec3 Rangle = { 0.0f ,0.0f,0.0f };
	const float HPMAX = 10;
	float HP = 10;						//�v���C���[HP
	const int damageTimeMax = 20;
	int damageTime = 0;					//�_���[�W�H��������̌����ڗp

	//�����Ă��邩�ǂ���
	bool moveFlag = false;

	//���
	bool avoidanceFlag = false;		//��𒆂�
	const int avoidanceTimeMax = 10;//�������
	int avoiDirection = 0;			//�������
	int avoidanceTime = 0;			//���������
	float avoiSpeed = 4.0f;			//����X�s�[�h
	const int avoiCoolTimeMax = 20;	//����N�[���^�C��
	int avoiCoolTime = 0;			//������N�[���^�C��
	float radDir = 0.0f;
	bool invincibleFlag = false;	//���G����


	//�ʏ�U��
	std::array <bool, 3> normalAttackFlag = { false,false,false };	//�ʏ�U���\��
	float normalLength = 20.0f;							//�U���̔��a
	float normalLengthSub = 15.0f;						//�����Ă��Ȃ������̍U���̔��a
	const int normalAttackTimeMax = 20;					//�U���ƍU���̊Ԃ̎���
	int normalAttackTime = 0;							//�U���ƍU���̊�
	int normalDirection = 0;							//�U���̌���
	Box normalAttackBox;								//���ۂ̍U���̓����蔻��
	int normalAttackCount = 0;							//�ʏ�U���̉���ڂ�
	const int normalGraceTimeMax = 50;					//�A���؂�܂ł̗P�\
	int normalGraceTime = 0;							//�A���؂�܂ł̗P�\
	float attackMoveHighSpeed = 1.0f;					//�R���ڂ̐i�ރX�s�[�h
	float attackMoveSpeed = 0.5f;						//�U�������֐i�ރX�s�[�h

	//���U��
	Object::ObjectData swordObject;//��
	Object::ObjectData tornadoObject;//��
	int cursorGraph;	//�J�[�\��
	Object::ObjectData cursorObject;//�J�[�\��

	std::array <Vec3,7> swordPosition = { Vec3{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f } };	//���W
	Object::ObjectData swordEffectObject;//��
	std::array<Vec3,7> swordAngle = { Vec3{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } };		//�p�x
	float swordSpeed = 7;//�X�s�[�h
	std::array<float,7> swordAngleVec = { 0,0,0,0,0,0,0 };//��΂�����
	std::array<bool,7> isSwordAttack = { false,false,false,false,false,false,false };//�A�^�b�N�t���O
	std::array<int,7> stingCnt = { 0,0,0,0,0,0,0 };//�h����܂ł̎���
	std::array<bool,7> haveSword = { true,true,true,true,true,true,true };//�����Ă錕
	bool isEnemySting[7][4];//�G�Ɏh�����Ă邩
	std::array < Box, 7> swordAttackBox;	//���̓����蔻��
	int shotNo = 0;//�ǂ̌���
	bool returnFlag = false;//�����߂鎞�̃t���O
	float nowTime = 0;//�����߂鎞�̃��[�v
	float endTime = 3;//�����߂鎞�̃��[�v
	float timeRate = 0;//�����߂鎞�̃��[�v
	std::array<float, 7> reverseValue = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	std::array<int, 7>  reverseAngle = { 0,0,0,0,0,0,0 };
	std::array<bool,7>  swordStop = { false,false,false,false,false,false,false };
	std::array<bool,7>  holdingFlag = { true,true,true,true,true,true,true };
	std::array<bool,7>  explosion = { false,false,false,false,false,false,false };
	std::array<int,7> explosionCount = { 0,0,0,0,0,0,0 };
	std::array<float, 7> explosionAngle = { 0,0,0,0,0,0,0 };
	Vec3 havePosition = { 10.0f, 0.0f, 0.0f };
	Box haveBox;
	float tornadoAngle = 0;
	float tornadoScale = 0.5;
	float slowValue = 1;
	int slowCount = 0;
	bool slowFlag = false;

	//�G�t�F�N�g�֌W
	bool AttackEffect = false;
	float AttackEffectSize = 5.0f;
	Vec3 AttackScale{ 1.0f,1.0f,1.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };
	int effectTime = 10;
	int effectCount = 0;

	std::array<float, 20 >enemyDamegeTime = { 60,60,60,60,60,60,60,60,60,60 };


	//UI
	Vec3 UIAngle{ 90.0f,0.0f,0.0f };

	//�v���C���[HP
	SpriteData HPGraph;
	SpriteData HPGaugeSub;
	SpriteData HPGaugeMain;

	//�\�[�h�Q�[�W
	SpriteData swordGraph;
	SpriteData swordGargeSub;
	SpriteData swordGargeMain;

	//Object::ObjectData comboPolygon;
	//Object::ObjectData  comboNumberObj;//�P����
	//Object::ObjectData black;
	/*int comboGraph;
	int comboNumberGraph[10];*/
	int yellowColor;



#if _DEBUG
	Object::ObjectData attackField;//�U���͈͉���
	int redColor;

	Object::ObjectData normalFieldOBJ;//�ʏ�U��
	//int normalGraph;

#endif
	Object::ObjectData AttackEffectOBJ;//�ʏ�U���G�t�F�N�g
	int AttackEffectGraph[9];
};
