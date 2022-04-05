#pragma once
#pragma once
#include "Object.h"
#include"Collision.h"
#include"Direction.h"
enum EnemyType
{
	Oni = 1,//�ߐڋS
	OniBow,//�|�S
};

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
//�G�̏��
struct EnemyData
{
	Box eBox;							//�G�{�b�N�X
	Sphere eSphere;						//�G��
	Vec3 position{ 50.0f,0.0f,0.0f };	//���W
	Vec3 oldPosition{};					//1�O�̍��W
	float speed = 0.3f;					//�G�X�s�[�h
	Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
	float HPMax = 10;					//�ő�HP
	float HP = 10;						//HP
	float r = 5;						//�傫��
	int Status = NORMAL;				//���
	int StatusTime = 0;					//��Ԏ���
	int direction = Down;				//�����Ă������
	int attackDirection = Down;			//�U���̌���
	int damegeTime = 0;					//�_�Ŏ���
	int type = 0;						//�G�̎��
	//�������p���l
	float bowAngle = 0.0f;				//�_���p�x
	Vec3 bowPos = {};					//��̍��W
	bool bowFlag = false;				//�|����������
	int bowTime = 0;					//����ł�������
};