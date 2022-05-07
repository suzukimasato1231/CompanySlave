
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

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//敵の読み込み
	//enemyObject = Object::Instance()->CreateOBJ("OniKari");
	//敵の攻撃状態の読み込み
	attackOBJ[0] = Object::Instance()->CreateOBJ("Oniyumi2-0","OniyumiOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("Oniyumi2-1", "OniyumiOBJ/");
	attackOBJ[2] = Object::Instance()->CreateOBJ("Oniyumi2-2", "OniyumiOBJ/");
	attackOBJ[3] = Object::Instance()->CreateOBJ("Oniyumi2-3", "OniyumiOBJ/");
	attackOBJ[4] = Object::Instance()->CreateOBJ("Oniyumi2-4", "OniyumiOBJ/");

	//敵弓歩き
	enemyWalk[0] = Object::Instance()->CreateOBJ("Oniyumi1-0", "OniyumiOBJ/");
	enemyWalk[1] = Object::Instance()->CreateOBJ("Oniyumi1-1", "OniyumiOBJ/");
	enemyWalk[2] = Object::Instance()->CreateOBJ("Oniyumi1-2", "OniyumiOBJ/");

	//矢のobj読込
	bowOBJ = Object::Instance()->CreateOBJ("enemyArrow","OniyumiOBJ/");

	bowRaysOBJ = Shape::CreateRect(2.0f, 10.0f);
}

void OniBow::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
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
		Object::Instance()->Draw(enemyWalk[oniData->walkNum], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case ATTACK:
		if (oniData->StatusTime >= attackMotionTime - 25)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 50)
		{
			Object::Instance()->Draw(attackOBJ[1], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 75)
		{
			Object::Instance()->Draw(attackOBJ[2], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 100)
		{
			Object::Instance()->Draw(attackOBJ[3], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[4], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		break;
	}


	//矢の描画
	if (oniData->bowTime > 0)
	{
		Object::Instance()->Draw(bowOBJ, oniData->bowPos, Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 90.0f, 0.0f));
	}
	//矢の射線方向の描画
	if (oniData->StatusTime >= attackMotionDamege)
	{
		//Object::Instance()->Draw(bowRaysOBJ, Vec3(oniData->position.x, 0.0f, oniData->position.z), Vec3(2.0f, 2.0f, 2.0f), Vec3(90.0f, -XMConvertToDegrees(oniData->bowAngle) + 90.0f, 0.0f));
	}
}

void OniBow::Move(EnemyData* oniData, Player* player)
{
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();

	//プレイヤーとエネミーの位置の差
	Vec3 memoryPosition = player->GetPosition() - oniData->position;
	//長さを求める
	float length = memoryPosition.length();
	//プレイヤーと敵が近すぎたら離れる
	if (length < player2EnemyDislikeLength)
	{
		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		oniData->position -= direction * moveSpeed * slowValue;
		if (oniData->StatusTime <= 0)
		{
			oniData->Status = ATTACK;
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = attackMotionTime;
		}
	}
	//プレイヤーと敵が良い距離だったら攻撃
	else if (length < player2EnemyLength)
	{
		oniData->Status = ATTACK;
		oniData->attackDirection = oniData->direction;
		oniData->StatusTime = attackMotionTime;
	}
	//プレイヤーと離れすぎたら攻撃
	else
	{
		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		oniData->position += direction * moveSpeed * slowValue;
	}
	//歩き描画の数字
	if (oniData->walkTime % 10 == 0)
	{
		oniData->walkNum++;
		if (oniData->walkNum >= 3)
		{
			oniData->walkNum = 0;
			oniData->walkTime = 0;
		}
	}
	oniData->walkTime++;
	oniData->StatusTime--;
}

void OniBow::SearchPlayer(EnemyData* oniData, Player* player)
{
	Box enemiesBox = SearchField(oniData);
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーと敵の距離
		float Length = Vec3(player->GetPosition() - oniData->position).length();

		if (Length < player2EnemyLength)
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

void OniBow::Attack(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		return;
	}

	//プレイヤーを狙う
	if (oniData->StatusTime > attackMotionDamege)
	{

		oniData->bowAngle = atan2(player->GetPosition().z - oniData->position.z, player->GetPosition().x - oniData->position.x);
	}
	//プレイヤーに撃つ
	else if (oniData->StatusTime == attackMotionDamege && oniData->bowTime <= 0)
	{
		oniData->bowFlag = true;
		oniData->bowMove = true;
		oniData->bowPos = oniData->position;
		oniData->bowTime = bowTimeMax;
	}

	//時間が終わったら索敵にもどる
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
		oniData->StatusTime = moveTime;
		oniData->bowPos = { 0.0f,0.0f,0.0f };
	}
	if (oniData->bowTime == bowTimeMax)
	{
		oniData->bowAFlag = true;
	}
	else {
		oniData->bowAFlag = false;
	}
}

void OniBow::BowUpdate(EnemyData* oniData, Player* player)
{
	if (oniData->bowTime >= 0)
	{
		OBB playerOBB;
		playerOBB.Initilize(player->GetPosition(), player->GetAngle(), Vec3(player->GetPSize(), player->GetPSize(), player->GetPSize()));
		OBB bowOBB;
		bowOBB.Initilize(oniData->bowPos, Vec3(0.0f, 0.0f, oniData->bowAngle), bowSize);
		oniData->bowOldPos = oniData->bowPos;
		if (oniData->bowMove == true)
		{
			oniData->bowPos.x += cosf(oniData->bowAngle) * bowSpeed;
			oniData->bowPos.z += sinf(oniData->bowAngle) * bowSpeed;
		}

		//攻撃範囲内にいたらプレイヤーにダメージ
		if (OBBCollision::ColOBBs(playerOBB, bowOBB) && oniData->bowFlag == true && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
			oniData->bowFlag = false;
		}
		oniData->bowTime--;
		if (oniData->bowTime <= 0)
		{
			oniData->bowFlag = false;
		}
	}
}

Box OniBow::SearchField(EnemyData* oniData)
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
		angle = { 0.0f,270.0f,0.0f };
		break;
	case Left:
		angle = { 0.0f,180.0f,0.0f };
		break;
	case Right:
		angle = { 0.0f,0.0f,0.0f };
		break;
	case Down:
		angle = { 0.0f,90.0f,0.0f };
		break;
	default:
		angle = { 0.0f,0.0f,0.0f };
	}
	return angle;
}



