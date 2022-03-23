#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
#include"Sprite.h"
//�v���C���[�̌����Ă������
enum Direction
{
	Up,
	Down,
	Left,
	Right,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight
};
/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	Player();//�R���X�g���N�^

	~Player();//�ł��R���X�g���N�^

	void Init();//������

	void Update(class Enemy *enemy);//�X�V

	void Draw();//�`��

	void SetPosition(Vec3 position) { this->position = position; }
private:
	/// <summary>
	/// �v���C���[�ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[�ƃG�l�~�[�Ƃ̍ŏ������̓G��������
	/// </summary>
	/// <param name="enemy">�G�l�~�[�N���X</param>
	void PlayerAttack(class Enemy *enemy);

	/// <summary>
	/// �U�����~�߂�
	/// </summary>
	void StopAttack();
	//�U���p�x����
	float Angle();

	//�v���C���[�̌��������߂�
	void PDirection();

	//���
	void Avoidance();

	//�f�o�b�N�`��
	void DebugDraw();
public://�擾�n
	//���W
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//�v���C���[��Box
	Box GetBox() { return pBox; }

	bool GetAttackFlag() { return attackFlag; }
	bool GetMoveFlag() { return moveFlag; }

	int GetComboNum() { return comboNum; }

	int GetComboTime() { return nowComboTime; }

	int GetCoolTime() { return coolTime; }
private:
	Object::ObjectData playerObject;	//�v���C���[�I�u�W�F�N�g
	Box pBox;							//�v���C���[�̒����`
	Vec3 position{ 10.0f,0.0f,0.0f };	//���W
	Vec3 oldPosition{};					//1�O�̍��W
	Vec3 speed{ 1.5f,1.5f,1.5f };		//�v���C���[�X�s�[�h

	Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
	Vec3 angle{ 0.0f,0.0f,0.0f };		//�p�x
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
	float r = 3;

	//�v���C���[HP
	int HP = 10;

	//�G��|�����߂̃p�����[�^
	const float comboMaxTime = 30;	//�R���{���ԍŏ�
	float comboTime = 30;			//�R���{����
	float nowComboTime = 0;			//���݂̃R���{����
	bool comboFlag = false;
	int comboNum = 0;				//�R���{��

	//�G���a��ɍs���܂ł̃X�s�[�h
	const float attackMaxTime = 10.0f;
	float attackTime = 0;
	Vec3 attackSpeed = { 4.0f,4.0f,4.0f };

	//�����Ă��邩�ǂ���
	bool moveFlag = false;

	//�A���؂�̍Œ����ǂ���
	bool attackFlag = false;
	//�N�[���^�C��
	const int coolTimeMax = 100;
	int coolTime = 0;

	int  direction = 0;		 //�v���C���[�̌���
	float attackAngle = 0.0f;//�U������

	//���
	bool avoidanceFlag = false;		//��𒆂�
	const int avoidanceTimeMax = 10;//�������
	int avoiDirection = 0;			//�������
	int avoidanceTime = 0;			//���������
	float avoiSpeed = 5.0f;			//����X�s�[�h
	const int avoiCoolTimeMax = 20;	//����N�[���^�C��
	int avoiCoolTime = 0;			//������N�[���^�C��

	bool invincibleFlag = false;	//���G����



	//UI
	Vec3 UIAngle{ 90.0f,0.0f,0.0f };
	Object::ObjectData comboPolygon;
	Object::ObjectData  comboNumberObj;//�P����
	Object::ObjectData black;
	int comboGraph;
	int comboNumberGraph[10];
	int yellowColor;

#if _DEBUG
	Object::ObjectData attackField;//�U���͈͉���
	int redColor;
#endif
};