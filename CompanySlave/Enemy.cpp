#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	for (int i = (int)enemyData.size() - 1; i >= 0; i--)
	{
		delete enemyData[i];
		enemyData.erase(enemyData.begin() + i);
	}
}

void Enemy::Init()
{
	enemyData.push_back(new EnemyData);
	enemyData[enemyData.size() - 1]->enemyObject = Object::Instance()->CreateOBJ("enemy");
	enemyData[enemyData.size() - 1]->eBox.minPosition = XMVectorSet(0, 2, 0, 1);
	enemyData[enemyData.size() - 1]->eBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	enemyData[enemyData.size() - 1]->oldPosition = enemyData[enemyData.size() - 1]->position;
}

void Enemy::Update(Vec3 pPos)
{
	for (size_t i = 0; i < enemyData.size(); i++)
	{
		enemyData[i]->oldPosition = enemyData[i]->position;

		//���W�����킹��
		enemyData[i]->eBox.minPosition = XMVectorSet(enemyData[i]->position.x - enemyData[i]->r,
			enemyData[i]->position.y - enemyData[i]->r,
			enemyData[i]->position.z - enemyData[i]->r,
			1);
		enemyData[i]->eBox.maxPosition = XMVectorSet(enemyData[i]->position.x + enemyData[i]->r,
			enemyData[i]->position.y + enemyData[i]->r,
			enemyData[i]->position.z + enemyData[i]->r,
			1);
	}

	if (Input::Instance()->KeybordTrigger(DIK_E))
	{
		enemyData.push_back(new EnemyData);
		enemyData[enemyData.size() - 1]->enemyObject = Object::Instance()->CreateOBJ("enemy");
		enemyData[enemyData.size() - 1]->eBox.minPosition = XMVectorSet(0, 2, 0, 1);
		enemyData[enemyData.size() - 1]->eBox.maxPosition = XMVectorSet(0, 2, 0, 1);
		enemyData[enemyData.size() - 1]->position = pPos + Vec3(+10, 0, 0);
		enemyData[enemyData.size() - 1]->oldPosition = enemyData[enemyData.size() - 1]->position;
	}
}

void Enemy::Draw()
{
	for (size_t i = 0; i < enemyData.size(); i++)
	{
		Object::Instance()->Draw(enemyData[i]->enemyObject, enemyData[i]->position, enemyData[i]->scale, enemyData[i]->angle, enemyData[i]->color);
	}
}

void Enemy::PushBlock(int num, Vec3 BPos, float blockSize, const int up, const int down)
{
	//�v���C���[���W
	float PRight = enemyData[num]->position.x + enemyData[num]->r, PLeft = enemyData[num]->position.x - enemyData[num]->r;
	float PUp = enemyData[num]->position.z + enemyData[num]->r, PDown = enemyData[num]->position.z - enemyData[num]->r;
	//�v���C���[�̑O�̍��W
	float oldRight = enemyData[num]->oldPosition.x + enemyData[num]->r, oldLeft = enemyData[num]->oldPosition.x - enemyData[num]->r;
	float oldUp = enemyData[num]->oldPosition.z + enemyData[num]->r, oldDown = enemyData[num]->oldPosition.z - enemyData[num]->r;
	//�u���b�N���W
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;


	//����
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
	}
	//����
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			enemyData[num]->position.z = BUp + enemyData[num]->r;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			enemyData[num]->position.z = BUp + enemyData[num]->r;
		}
	}
	else
	{
		//�E
		if (oldRight <= BLeft)
		{
			enemyData[num]->position.x = BLeft - enemyData[num]->r;
		}
		//��
		if (oldLeft >= BRight)
		{
			enemyData[num]->position.x = BRight + enemyData[num]->r;
		}
		//��
		if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
		//��
		if (oldDown >= BUp)
		{
			enemyData[num]->position.z = BUp + enemyData[num]->r;
		}
	}
}

Vec3 Enemy::GetPosition(int i)
{
	return enemyData[i]->position;
}

Box Enemy::GetBox(int i)
{
	return enemyData[i]->eBox;
}

int Enemy::GetEnemySize()
{
	return enemyData.size();
}
