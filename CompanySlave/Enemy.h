#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
#include"Player.h"
#include"LoadCSV.h"
#include"Direction.h"
enum spawnNo
{
	NOTSPAWN,
	ONESPAWN,
	TWOSPAWN,
};

//�G�̏��
enum Status
{
	NORMAL, //�ʏ���
	MOVE,	//����
	ATTACK,	//�U��
	ENEMIES,//�G����������
};

class Enemy
{
private://�\����
	//�G�̏��
	struct EnemyData
	{
		Object::ObjectData enemyObject;		//�v���C���[�I�u�W�F�N�g
		Box eBox;							//�G�{�b�N�X
		Sphere eSphere;						//�G��
		Vec3 position{ 50.0f,0.0f,0.0f };	//���W
		Vec3 oldPosition{};					//1�O�̍��W
		float speed = 0.5f;					//�G�X�s�[�h
		Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
		Vec3 angle{ 0.0f,0.0f,0.0f };		//�p�x
		Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
		int HP = 10;						//HP
		float r = 5;						//�傫��
		int Status = NORMAL;				//���
		int StatusTime = 0;					//��Ԏ���
		int direction = Down;				//�����Ă������
		int attackDirection = Down;			//�U���̌���
		int damegeTime = 0;					//�_�Ŏ���
	};
private:
	//�G�ő吔
	static const int eNumMax = 20;
	//�G�z��
	std::vector<EnemyData *>eData;

public:
	Enemy();//�R���X�g���N�^

	~Enemy();//�ł��R���X�g���N�^

	void Init();//������

	void Update(class Player *player);//�X�V

	void Draw();//�`��

	//TRUE�ɂ���
	void WasAttack(int i);
	//�G�̈ʒu��ݒ�
	void SetPosition(int i, Vec3 position);
	/// <summary>
	/// �v���C���[�̒ʏ�U���_���[�W
	/// </summary>
	void DamegeNormal(int i);
private:
	//�ړ�
	void Move(int i, Player *player);
	//���G
	void SearchPlayer(int i, class Player *player);
	//�U��
	void Attack(int i, class Player *player);
	//�v���C���[����������
	void Enemies(int i, class Player *player);
	//�폜
	void Delete();
	//�����ɂ���č��G�͈͂̌���
	Box SearchField(int i);
	//�����ɂ���čU���͈͂̌���
	Box AttackField(int i);
	//�G�������Ă������
	int Direction(int i, class Player *player);
public://�擾�n
	// ���W�擾
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//�G�l�~�[���擾
	int GetEnemySize() { return eData.size(); }
	//�U�����ꂽ�����擾
	//bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//�����n�̃}�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu
private:
	const Vec2 attackEnemies{ 40.0f,40.0f };		//���G�͈�:width,height

	const Vec2 attackField{ 10.0f,10.0f };			//�U���͈�

	const int moveTime = 40;						//�ړ�����

	const int attackMotionTime = 60;				//�U�����[�V�����̎���
	const int attackMotionDamege = 10;				//�U�����[�V�������̂ǂ̎��ԂŃ_���[�W��^���邩

	const float player2EnemyLength = 10.0f;			//�v���C���[�ƓG�̋���

	Object::ObjectData debugField;	//���G
	Object::ObjectData debugField2;//�U���͈�
	int redColor;
};