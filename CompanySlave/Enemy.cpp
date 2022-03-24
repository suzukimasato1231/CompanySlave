#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//���������
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
	}
}

void Enemy::Init()
{
}

void Enemy::Update(Player *player)
{
	//�G����
	Generation(player);

	//�G���d�Ȃ�Ȃ��悤�ɂ���
	Enemy2Enemy();

	for (size_t i = 0; i < eData.size(); i++)
	{
		//�ړ�
		Move(i, player);
		//�_���[�W
		Damege(i, player);
		if (eData[i]->damegeTime > 0)
		{
			eData[i]->damegeTime--;
		}
	}
	//�폜
	Delete();
}

void Enemy::Draw()
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->damegeTime % 2 == 0)
		{
			Object::Instance()->Draw(eData[i]->enemyObject, eData[i]->position, eData[i]->scale, eData[i]->angle, eData[i]->color);
		}
	}
}


void Enemy::WasAttack(int i)
{
	eData[i]->wasAttackFlag = true;
}

void Enemy::SetPosition(int i, Vec3 position)
{
	eData[i]->position = position;
}

void Enemy::DamegeNormal(int i)
{
	eData[i]->HP -= 2;
	eData[i]->damegeTime = 10;
}

void Enemy::Move(int i, Player *player)
{
	eData[i]->oldPosition = eData[i]->position;

	//�v���C���[�Ɍ������ē����Ă���
	if (player->GetAttackFlag() == false && player->GetComboTime() <= 0)
	{
		//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
		Vec3 memoryPosition = player->GetPosition() - eData[i]->position;
		//���������߂�
		float length = memoryPosition.length();
		//�v���C���[�̌���
		Vec3 direction = memoryPosition.normalize();
		eData[i]->position += direction * eData[i]->speed;
	}

	//���W�����킹��
	eData[i]->eBox.minPosition = XMVectorSet(eData[i]->position.x - eData[i]->r,eData[i]->position.y - eData[i]->r,eData[i]->position.z - eData[i]->r,1);
	eData[i]->eBox.maxPosition = XMVectorSet(eData[i]->position.x + eData[i]->r,eData[i]->position.y + eData[i]->r,eData[i]->position.z + eData[i]->r,1);
	eData[i]->eSphere.center = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z, 1);
}

void Enemy::Damege(int i, Player *player)
{
	//�v���C���[�̍U�����I�������_���[�W��H�炤
	if (player->GetAttackFlag() == false && player->GetComboTime() <= 0 && eData[i]->wasAttackFlag == true)
	{
		eData[i]->damegeTime = 10;
		if (player->GetComboNum() == 0)
		{
			eData[i]->HP -= 1;
		}
		else
		{
			eData[i]->HP -= player->GetComboNum();
		}
		eData[i]->wasAttackFlag = false;
	}
}

void Enemy::Generation(Player *player)
{
	//�G����
	if (Input::Instance()->KeybordTrigger(DIK_E))
	{
		eData.push_back(new EnemyData);
		eData[eData.size() - 1]->enemyObject = Object::Instance()->CreateOBJ("enemy");
		eData[eData.size() - 1]->position = player->GetPosition() + Vec3(+10, 0, 0);
		eData[eData.size() - 1]->oldPosition = eData[eData.size() - 1]->position;
		//���W�����킹��
		eData[eData.size() - 1]->eBox.minPosition = XMVectorSet(
			eData[eData.size() - 1]->position.x - eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.y - eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.z - eData[eData.size() - 1]->r, 1);
		eData[eData.size() - 1]->eBox.maxPosition = XMVectorSet(
			eData[eData.size() - 1]->position.x + eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.y + eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.z + eData[eData.size() - 1]->r, 1);
		eData[eData.size() - 1]->eSphere.radius = eData[eData.size() - 1]->r;
		eData[eData.size() - 1]->eSphere.center = XMVectorSet(
			eData[eData.size() - 1]->position.x,
			eData[eData.size() - 1]->position.y,
			eData[eData.size() - 1]->position.z, 1);
	}
}

void Enemy::Delete()
{
	//�G��������
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		if (eData[i]->HP <= 0)
		{
			delete eData[i];
			eData.erase(eData.begin() + i);
		}
	}
}

void Enemy::Enemy2Enemy()
{

}


