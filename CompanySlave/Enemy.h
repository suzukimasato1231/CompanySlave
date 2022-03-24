#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
#include"Player.h"
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
		bool wasAttackFlag = false;			//�U�����ꂽ���ǂ���
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
	//�_���[�W����
	void Damege(int i, class Player *player);
	//����
	void Generation(class Player *player);
	//�폜
	void Delete();
	//�G�l�~�[�ƃG�l�~�[�̉����߂�
	void Enemy2Enemy();
public://�擾�n
	// ���W�擾
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//�G�l�~�[���擾
	int GetEnemySize() { return eData.size(); }
	//�U�����ꂽ�����擾
	bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }
};