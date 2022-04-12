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

	void Init();

	void Draw(EnemyData *oniData);
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

	Object::ObjectData attackOBJ[2];				//�G�l�~�[�A�^�b�N

	const Vec2 attackEnemies{ 100.0f,80.0f };		//���G�͈�:width,height

	const int moveTime = 40;						//�ړ�����

	const int attackMotionTime = 200;				//�U�����[�V�����̎���
	const int attackMotionDamege = 100;				//�U�����[�V�������̂ǂ̎��ԂŃ_���[�W��^���邩

	const float player2EnemyLength = 50.0f;			//�v���C���[�ƓG�̋���


	const float bowSpeed = 2.0f;					//����ł����X�s�[�h
	const int bowTimeMax = 100;
	const Vec3 bowSize{ 4.0f,1.0f,8.0f };			//��̃T�C�Y
	Object::ObjectData bowOBJ;//��̕`��
	Object::ObjectData bowRaysOBJ;//��̎ː�


	//�f�o�b�N�p
	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈�
	int redColor;

	float slowValue= 1;
};

