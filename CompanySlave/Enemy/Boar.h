#pragma once
#include"EnemyHelper.h"

class Player;
/// <summary>
/// �T
/// </summary>
class Boar
{
public:
	Boar();
	~Boar();

	void Init();

	void Draw(EnemyData* oniData);
	//�|���`��
	void FallDownDraw(EnemyData* oniData);
	//�ړ�
	void Move(EnemyData* oniData, class  Player* player);
	//���G
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//�U��
	void Attack(EnemyData* oniData, class Player* player);

	EnemyData BoarData() { return boarData; }
private:
	//�G�̍U���G�t�F�N�g
	void EffectDraw(EnemyData* oniData);

	//�����ɂ���č��G�͈͂̌���
	Box SearchField(EnemyData* oniData);
	//�����ɂ���ēG�̕`��p�x���擾
	Vec3 DirectionAngle(int direction);
private:
	EnemyData boarData;

	Object::ObjectData enemyObject[5];					//�v���C���[�I�u�W�F�N�g

	Object::ObjectData attackOBJ[5];					//�G�l�~�[�A�^�b�N

	Object::ObjectData nockBackOBJ[2];				//�m�b�N�o�b�N�I�u�W�F�N�g

	const Vec2 attackEnemies{ 100.0f,80.0f };		//���G�͈�:width,height

	const Vec2 attackField{ 15.0f,15.0f };			//�U���͈�

	const int moveTime = 40;						//�ړ�����

	const int attackMotionTime = 100;				//�U�����[�V�����̎���
	const int attackHoldTime = 50;					//�U�����\���鎞��
	//const int attackMoveTime = 40;				//�U���̈ړ�����
	//const int attackMotionDamege = 10;			//�U�����[�V�������̂ǂ̎��ԂŃ_���[�W��^���邩

	const float player2EnemyLength = 50.0f;			//�v���C���[�ƓG�̋���

	const float moveSpeed = 0.8f;					//�G�̃X�s�[�h
	const float attackMoveSpeed = 1.5f;				//�U�����s���Ƃ��̃X�s�[�h

	//�f�o�b�N�p
	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈�
	int redColor;


	//�G�l�~�[�A�^�b�N
	Object::ObjectData AttackEffectOBJ;//�ʏ�U���G�t�F�N�g
	int AttackEffectGraph[9];
	//�G�t�F�N�g�֌W
	bool AttackEffect = false;
	float AttackEffectSize = 5.0f;
	Vec3 AttackScale{ 2.0f,2.0f,2.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };
	int effectTime = 10;
	int effectCount = 0;


	float slowValue = 1;
};

