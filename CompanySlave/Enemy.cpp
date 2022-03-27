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
	//メモリ解放
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
	}
}

void Enemy::Init()
{

	eSpawner.push_back(new EnemySpawner);
	eSpawner[eSpawner.size() - 1]->position = { 0.0f,0.0f,0.0f };

	eSpawner.push_back(new EnemySpawner);
	eSpawner[eSpawner.size() - 1]->position = { 500.0f,0.0f,200.0f };

}

void Enemy::Update(Player *player)
{
	//敵生成
	Generation(player);

	//敵が重ならないようにする
	Enemy2Enemy(player);

	for (size_t i = 0; i < eData.size(); i++)
	{
		//移動
		Move(i, player);
		//ダメージ
		Damege(i, player);
		if (eData[i]->damegeTime > 0)
		{
			eData[i]->damegeTime--;
		}
	}
	//削除
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

	//プレイヤーに向かって動いていく
	
	
		//プレイヤーとエネミーの位置の差
		Vec3 memoryPosition = player->GetPosition() - eData[i]->position;
		//長さを求める
		float length = memoryPosition.length();

		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		eData[i]->position += direction * eData[i]->speed;
	

	//座標を合わせる
	eData[i]->eBox.minPosition = XMVectorSet(eData[i]->position.x - eData[i]->r, eData[i]->position.y - eData[i]->r, eData[i]->position.z - eData[i]->r, 1);
	eData[i]->eBox.maxPosition = XMVectorSet(eData[i]->position.x + eData[i]->r, eData[i]->position.y + eData[i]->r, eData[i]->position.z + eData[i]->r, 1);
	eData[i]->eSphere.center = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z, 1);
}

void Enemy::Damege(int i, Player *player)
{
	////プレイヤーの攻撃が終わったらダメージを食らう
	//if (player->GetAttackFlag() == false && player->GetComboTime() <= 0 && eData[i]->wasAttackFlag == true)
	//{
	//	eData[i]->damegeTime = 10;
	//	if (player->GetComboNum() == 0)
	//	{
	//		eData[i]->HP -= 1;
	//	}
	//	else
	//	{
	//		eData[i]->HP -= player->GetComboNum();
	//	}
	//	eData[i]->wasAttackFlag = false;
	//}
}

void Enemy::Generation(Player *player)
{
	for (int i = 0; i < eSpawner.size(); i++)
	{
		if (eSpawner[i]->spawnTime <= 0 && GetEnemySize() < eNumMax)
		{
			eSpawner[i]->spawnTime = eSpawner[i]->spawnTimemax;
			eData.push_back(new EnemyData);
			int eSize = eData.size() - 1;
			eData[eSize]->enemyObject = Object::Instance()->CreateOBJ("enemy");
			eData[eSize]->position = eSpawner[i]->position;
			eData[eSize]->oldPosition = eData[eData.size() - 1]->position;
			//座標を合わせる
			eData[eSize]->eBox.minPosition = XMVectorSet(eData[eSize]->position.x - eData[eSize]->r, eData[eSize]->position.y - eData[eSize]->r, eData[eSize]->position.z - eData[eSize]->r, 1);
			eData[eSize]->eBox.maxPosition = XMVectorSet(eData[eSize]->position.x + eData[eSize]->r, eData[eSize]->position.y + eData[eSize]->r, eData[eSize]->position.z + eData[eSize]->r, 1);
			eData[eSize]->eSphere.radius = eData[eSize]->r;
			eData[eSize]->eSphere.center = XMVectorSet(eData[eSize]->position.x, eData[eSize]->position.y, eData[eSize]->position.z, 1);
		}
		eSpawner[i]->spawnTime--;
	}



}

void Enemy::Delete()
{
	//敵が消える
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		if (eData[i]->HP <= 0)
		{
			delete eData[i];
			eData.erase(eData.begin() + i);
		}
	}
}

void Enemy::Enemy2Enemy(Player *player)
{
	for (size_t i = 0; i < GetEnemySize(); i++)
	{
		for (size_t j = GetEnemySize() - 1; j > i; j--)
		{
			if (i == j) { break; }

			if (Collision::CircleCollision(Vec2(eData[i]->position.x, eData[i]->position.z), Vec2(eData[j]->position.x, eData[j]->position.z), eData[i]->r, eData[j]->r)
				&& Collision::CircleCollision(Vec2(player->GetPosition().x, player->GetPosition().z), Vec2(eData[i]->position.x, eData[i]->position.z), 20.0f, eData[i]->r))
			{
				float lengthI = Vec3(eData[i]->position - player->GetPosition()).length();
				float lengthJ = Vec3(eData[j]->position - player->GetPosition()).length();
				if (lengthI > lengthJ)
				{
					eData[i]->position = eData[i]->oldPosition;
				}
				else
				{
					eData[j]->position = eData[j]->oldPosition;
				}
			}
		}
	}

}


