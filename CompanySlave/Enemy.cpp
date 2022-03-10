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

		//座標を合わせる
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
	//プレイヤー座標
	float PRight = enemyData[num]->position.x + enemyData[num]->r, PLeft = enemyData[num]->position.x - enemyData[num]->r;
	float PUp = enemyData[num]->position.z + enemyData[num]->r, PDown = enemyData[num]->position.z - enemyData[num]->r;
	//プレイヤーの前の座標
	float oldRight = enemyData[num]->oldPosition.x + enemyData[num]->r, oldLeft = enemyData[num]->oldPosition.x - enemyData[num]->r;
	float oldUp = enemyData[num]->oldPosition.z + enemyData[num]->r, oldDown = enemyData[num]->oldPosition.z - enemyData[num]->r;
	//ブロック座標
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;


	//左上
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
	}
	//左下
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			enemyData[num]->position.z = BUp + enemyData[num]->r;
		}
	}
	//右上
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
	}
	//右下
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			enemyData[num]->position.z = BUp + enemyData[num]->r;
		}
	}
	else
	{
		//右
		if (oldRight <= BLeft)
		{
			enemyData[num]->position.x = BLeft - enemyData[num]->r;
		}
		//左
		if (oldLeft >= BRight)
		{
			enemyData[num]->position.x = BRight + enemyData[num]->r;
		}
		//上
		if (oldUp <= BDown)
		{
			enemyData[num]->position.z = BDown - enemyData[num]->r;
		}
		//下
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
