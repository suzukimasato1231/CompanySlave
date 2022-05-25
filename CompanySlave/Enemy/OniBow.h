#pragma once
#include"EnemyHelper.h"
class Player;
/// <summary>
/// �S�������N���X
/// </summary>
class OniBow
{
public:
	OniBow();
	~OniBow();
	//������
	void Init();
	//�`��
	void Draw(EnemyData *oniData);
	//�|���`��
	void FallDownDraw(EnemyData* oniData);
	//�ړ�
	void Move(EnemyData *oniData, class  Player *player);
	//���G
	void SearchPlayer(EnemyData *oniData, class Player *player);
	//�U��
	void Attack(EnemyData *oniData, class Player *player);
	//��̃A�b�v�f�[�g
	void BowUpdate(EnemyData *oniData, Player *player);
private:
	//�����ɂ���č��G�͈͂̌���
	Box SearchField(EnemyData *oniData);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
private:

	Object::ObjectData enemyObject;					//�v���C���[�I�u�W�F�N�g

	Object::ObjectData attackOBJ[5];				//�G�l�~�[�A�^�b�N

	Object::ObjectData enemyWalk[3];				//�G�l�~�[����

	Object::ObjectData enemyDeadObject;					//�v���C���[�I�u�W�F�N�g

	const Vec2 attackEnemies{ 100.0f,80.0f };		//���G�͈�:width,height

	const int moveTime = 60;						//�ړ�����

	const int attackMotionTime = 200;				//�U�����[�V�����̎���
	const int attackMotionDamege = 100;				//�U�����[�V�������̂ǂ̎��ԂŖ�����

	const float player2EnemyLength = 100.0f;		//�v���C���[�ƓG�̋���

	const float player2EnemyDislikeLength = 40.0f;	//�G���v���C���[�Ƌ�����u������

	const float moveSpeed = 0.3f;					//�G�̃X�s�[�h



	const float bowSpeed = 2.5f;					//����ł����X�s�[�h
	const int bowTimeMax = 100;						//����݂��Ă��鎞��
	const Vec3 bowSize{ 4.0f,1.0f,8.0f };			//��̃T�C�Y
	Object::ObjectData bowOBJ;						//��̕`��
	Object::ObjectData bowRaysOBJ;					//��̎ː�


	//�f�o�b�N�p
	Object::ObjectData debugField;					//���G
	Object::ObjectData debugField2;					//�U���͈�
	int redColor;

	float slowValue= 1;
};

