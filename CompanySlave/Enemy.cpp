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
}

void Enemy::Update(Player *player)
{
	//敵生成
	Generation(player);

	//敵が重ならないようにする
	Enemy2Enemy();

	for (size_t i = 0; i < eData.size(); i++)
	{
		//移動
		Move(i, player);
		//ダメージ
		Damege(i, player);
	}
	//削除

	Delete();
}

void Enemy::Draw()
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		Object::Instance()->Draw(eData[i]->enemyObject, eData[i]->position, eData[i]->scale, eData[i]->angle, eData[i]->color);
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

void Enemy::Move(int i, Player *player)
{
	eData[i]->oldPosition = eData[i]->position;

	//プレイヤーに向かって動いていく
	if (player->GetAttackFlag() == false)
	{
		//プレイヤーとエネミーの位置の差
		Vec3 memoryPosition = player->GetPosition() - eData[i]->position;
		//長さを求める
		float length = memoryPosition.length();
		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		eData[i]->position += direction * eData[i]->speed;
	}

	//座標を合わせる
	eData[i]->eBox.minPosition = XMVectorSet(
		eData[i]->position.x - eData[i]->r,
		eData[i]->position.y - eData[i]->r,
		eData[i]->position.z - eData[i]->r,
		1);
	eData[i]->eBox.maxPosition = XMVectorSet(
		eData[i]->position.x + eData[i]->r,
		eData[i]->position.y + eData[i]->r,
		eData[i]->position.z + eData[i]->r,
		1);
}

void Enemy::Damege(int i, Player *player)
{
	//プレイヤーの攻撃が終わったらダメージを食らう
	if (player->GetAttackFlag() == false && eData[i]->wasAttackFlag == true)
	{
		eData[i]->HP -= player->GetComboNum();
		eData[i]->wasAttackFlag = false;
	}
}

void Enemy::Generation(Player *player)
{
	//敵生成
	if (Input::Instance()->KeybordTrigger(DIK_E))
	{
		eData.push_back(new EnemyData);
		eData[eData.size() - 1]->enemyObject = Object::Instance()->CreateOBJ("enemy");
		eData[eData.size() - 1]->position = player->GetPosition() + Vec3(+10, 0, 0);
		eData[eData.size() - 1]->oldPosition = eData[eData.size() - 1]->position;
		//座標を合わせる
		eData[eData.size() - 1]->eBox.minPosition = XMVectorSet(
			eData[eData.size() - 1]->position.x - eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.y - eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.z - eData[eData.size() - 1]->r, 1);
		eData[eData.size() - 1]->eBox.maxPosition = XMVectorSet(
			eData[eData.size() - 1]->position.x + eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.y + eData[eData.size() - 1]->r,
			eData[eData.size() - 1]->position.z + eData[eData.size() - 1]->r, 1);
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

void Enemy::Enemy2Enemy()
{
	/*for (size_t i = 1; i < eData.size(); i++)
	{
		if (eData.size() <= 1) { break; }
		for (size_t j = 0; j < eData.size(); j++)
		{
			if (i == j)
			{
				break;
			}
			if (Collision::CheckBox2Box(eData[i]->eBox, eData[j]->eBox))
			{
				eData[i]->position = eData[i]->oldPosition;
			}
		}
	}*/
}


