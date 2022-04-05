
#include "OniBow.h"
#include"Player.h"
#include"Shape.h"
OniBow::OniBow()
{
}

OniBow::~OniBow()
{
}

void OniBow::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	//debugField2 = Shape::CreateRect(attackField.x, attackField.y);

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	enemyObject = Object::Instance()->CreateOBJ("OniKari");

	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2");

	bowOBJ = Shape::CreateRect(bowSize.x, bowSize.y);
}

void OniBow::Draw(EnemyData *oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject, oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		switch (oniData->direction)
		{
		case Up:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + attackEnemies.y / 2),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Down:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - attackEnemies.y / 2),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Left:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x - attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Right:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x + attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		}
		break;
	case MOVE:
		Object::Instance()->Draw(enemyObject, oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case ATTACK:
		if (oniData->StatusTime >= attackMotionDamege)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[1], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	}


	//矢の描画


}

void OniBow::Move(EnemyData *oniData, Player *player)
{
	oniData->oldPosition = oniData->position;

	//プレイヤーとエネミーの位置の差
	Vec3 memoryPosition = player->GetPosition() - oniData->position;
	//長さを求める
	float length = memoryPosition.length();

	if (length < player2EnemyLength)
	{
		oniData->Status = ATTACK;
		oniData->attackDirection = oniData->direction;
		oniData->StatusTime = attackMotionTime;
	}
	else
	{
		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		oniData->position += direction * oniData->speed;
	}
}

void OniBow::SearchPlayer(EnemyData *oniData, Player *player)
{
	Box enemiesBox = SearchField(oniData);
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーと敵の距離
		float Length = Vec3(player->GetPosition() - oniData->position).length();

		if (Length > player2EnemyLength)
		{//プレイヤーが攻撃距離にいなかったら移動
			oniData->Status = MOVE;
			oniData->StatusTime = moveTime;
		}
		else
		{//プレイヤーが攻撃距離にいたら
			oniData->Status = ATTACK;
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = attackMotionTime;
		}
	}
}

void OniBow::Attack(EnemyData *oniData, Player *player)
{
	if (oniData == nullptr)
	{
		return;
	}

	//プレイヤーを狙う
	if (oniData->StatusTime >= attackMotionDamege)
	{
		oniData->bowAngle = atan2(player->GetPosition().z - oniData->position.z, player->GetPosition().x - oniData->position.x);
	}
	//プレイヤーに撃つ
	else if (oniData->StatusTime == attackMotionDamege)
	{
		oniData->bowFlag = true;
		oniData->bowPos = oniData->position;
		oniData->bowTime = bowTimeMax;
	}

	//時間が終わったら索敵にもどる
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
		oniData->bowPos = { 0.0f,0.0f,0.0f };
	}
}

void OniBow::BowUpdate(EnemyData *oniData, Player *player)
{
	//Box attackBox = AttackField(oniData);
	//oniData->bowPos.x += cosf(oniData->bowAngle) * bowSpeed;
	//oniData->bowPos.z += sinf(oniData->bowAngle) * bowSpeed;

	////攻撃範囲内にいたらプレイヤーにダメージ
	//if (Collision::CheckBox2Box(attackBox, player->GetBox()) && oniData->bowFlag == true)
	//{
	//	player->Damage();
	//	oniData->bowFlag = false;
	//}
	//oniData->bowTime--;
	//if (oniData->bowTime <= 0)
	//{
	//	oniData->bowFlag = false;
	//}
}

Box OniBow::SearchField(EnemyData *oniData)
{
	Box enemiesBox;
	switch (oniData->direction)
	{
	case Up:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.x / 2, oniData->position.y, oniData->position.z + attackEnemies.y, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.x / 2, oniData->position.y, oniData->position.z, 1);
		break;
	case Left:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.y, oniData->position.y, oniData->position.z - attackEnemies.x / 2, 1);
		break;
	case Right:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.y, oniData->position.y, oniData->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z - attackEnemies.x / 2, 1);
		break;
	case Down:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.x / 2, oniData->position.y, oniData->position.z, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.x / 2, oniData->position.y, oniData->position.z - attackEnemies.y, 1);
		break;
	default:
		enemiesBox.maxPosition = {};
		enemiesBox.minPosition = {};
	}
	return enemiesBox;
}

Vec3 OniBow::DirectionAngle(int direction)
{
	Vec3 angle = {};
	switch (direction)
	{
	case Up:
		angle = { 0.0f,90.0f,0.0f };
		break;
	case Left:
		angle = { 0.0f,0.0f,0.0f };
		break;
	case Right:
		angle = { 0.0f,180.0f,0.0f };
		break;
	case Down:
		angle = { 0.0f,270.0f,0.0f };
		break;
	default:
		angle = { 0.0f,0.0f,0.0f };
	}
	return angle;
}



