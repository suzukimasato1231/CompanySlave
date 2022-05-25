#include "WOLF.h"
#include"Player.h"
#include"Shape.h"
Wolf::Wolf()
{}

Wolf::~Wolf()
{}

void Wolf::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//通常状態
	enemyObject[0] = Object::Instance()->CreateOBJ("wolf1-0","WolfOBJ/", true);
	enemyObject[1] = Object::Instance()->CreateOBJ("wolf1-1", "WolfOBJ/", true);
	enemyObject[2] = Object::Instance()->CreateOBJ("wolf1-2", "WolfOBJ/", true);
	enemyObject[3] = Object::Instance()->CreateOBJ("wolf1-3", "WolfOBJ/", true);
	enemyObject[4] = Object::Instance()->CreateOBJ("wolf1-4", "WolfOBJ/", true);
	//攻撃モーション
	attackOBJ[0] = Object::Instance()->CreateOBJ("wolf2-1", "WolfOBJ/", true);
	attackOBJ[1] = Object::Instance()->CreateOBJ("wolf2-2", "WolfOBJ/", true);
	attackOBJ[2] = Object::Instance()->CreateOBJ("wolf2-3", "WolfOBJ/", true);
	//ノックバック
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("wolf1-0", "WolfOBJ/", true);
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("wolf1-0", "WolfOBJ/", true);

	//攻撃エフェクト
	AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect9.png");

	wolfData.HP = 4;
	wolfData.HPMax = 4;
	wolfData.RushFlag = false;
	wolfData.scale = { 0.5f,0.5f,0.5f };
}

void Wolf::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
#if _DEBUG
		switch (oniData->direction)
		{
		case Up:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Down:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Left:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x - attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Right:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x + attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		}
#endif
		break;
	case MOVE:
		Object::Instance()->Draw(enemyObject[oniData->walkNum], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case ATTACK:
		if (oniData->StatusTime >= attackMotionTime - attackHoldTime)
		{//飛びつき構え
			Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - attackHoldTime - attackMoveTime / 2)
		{//飛びつき
			Object::Instance()->Draw(attackOBJ[1], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= oniData->StatusTime >= attackMotionTime - attackHoldTime - attackMoveTime)
		{///飛びつき着地
			Object::Instance()->Draw(attackOBJ[2], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//攻撃後の硬直
			Object::Instance()->Draw(enemyObject[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		//EffectDraw(oniData);

		break;
	case NOCKBACK:
		if (oniData->nockbackTime >= 3)
		{
			Object::Instance()->Draw(nockBackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		else
		{
			Object::Instance()->Draw(nockBackOBJ[1], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	}
}
void Wolf::FallDownDraw(EnemyData* oniData)
{//倒れる敵の描画
	Object::Instance()->Draw(enemyObject[0], oniData->position, oniData->scale,
		oniData->angle + DirectionAngle(oniData->direction), oniData->color);
}
void Wolf::Move(EnemyData* oniData, Player* player)
{
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	//プレイヤーとエネミーの位置の差
	Vec3 memoryPosition = player->GetPosition() - oniData->position;
	//長さを求める
	float length = memoryPosition.length();

	if (length < player2EnemyLength)
	{
		//プレイヤーとエネミーの位置の差
		Vec3 memoryPosition = player->GetPosition() - oniData->position;
		//プレイヤーの向き
		oniData->pDirection = memoryPosition.normalize();
		oniData->Status = ATTACK;
		oniData->RushFlag = true;
		oniData->attackDirection = oniData->direction;
		oniData->StatusTime = attackMotionTime;
	}
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
		if (oniData->walkNum >= 5)
		{
			oniData->walkNum = 0;
			oniData->walkTime = 0;
		}
	}
	oniData->walkTime++;
}

void Wolf::SearchPlayer(EnemyData* oniData, Player* player)
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
			oniData->RushFlag = false;
			oniData->StatusTime = moveTime;
		}
		else
		{//プレイヤーが攻撃距離にいたら
			//プレイヤーとエネミーの位置の差
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//プレイヤーの向き
			oniData->pDirection = memoryPosition.normalize();
			oniData->Status = ATTACK;
			oniData->RushFlag = true;
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = attackMotionTime;
		}
	}
}

void Wolf::Attack(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		return;
	}
	//攻撃を構える時間
	if (oniData->StatusTime >= attackMotionTime - attackHoldTime)
	{

	}
	//攻撃時間中
	else if (oniData->StatusTime >= attackMotionTime - attackHoldTime - attackMoveTime)
	{
		//攻撃範囲内にいたらプレイヤーにダメージ
		if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
		{
			//エフェクトのフラグTRUE
			AttackEffect = true;
			player->Damage(1);
		}
		//移動攻撃
		oniData->oldPosition = oniData->position;
		slowValue = player->GetSlow();
		oniData->position += oniData->pDirection * attackMoveSpeed * slowValue;
	}


	//エフェクト関係
	if (AttackEffect == true) {

		if (effectCount < 8) {
			effectCount++;
		}
		else if (effectCount == 8) {
			effectCount = 0;
			AttackEffect = false;
			oniData->attakWFlag = true;
		}
	}
	else {
		oniData->attakWFlag = false;
	}
	//時間が終わったら索敵にもどる
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
	}
}

void Wolf::EffectDraw(EnemyData* oniData)
{
	if (AttackEffect == true)
	{
		switch (oniData->attackDirection)
		{
		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + oniData->r + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - oniData->r - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - oniData->r - AttackEffectSize, oniData->position.y, oniData->position.z), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + oniData->r + AttackEffectSize, oniData->position.y, oniData->position.z), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + AttackEffectSize, oniData->position.y, oniData->position.z + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - AttackEffectSize, oniData->position.y, oniData->position.z + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + AttackEffectSize, oniData->position.y, oniData->position.z - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - AttackEffectSize, oniData->position.y, oniData->position.z - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		}
	}
}

Box Wolf::SearchField(EnemyData* oniData)
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

Vec3 Wolf::DirectionAngle(int direction)
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

