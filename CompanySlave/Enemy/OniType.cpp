#include "OniType.h"
#include"Player.h"
#include"Shape.h"
OniType::OniType()
{
}

OniType::~OniType()
{
}

void OniType::Init()
{
#if _DEBUG
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);
#endif

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//通常状態
	enemyObject[0] = Object::Instance()->CreateOBJ("OniKari1-0", "OniOBJ/", true);
	enemyObject[1] = Object::Instance()->CreateOBJ("OniKari1-1", "OniOBJ/", true);
	enemyObject[2] = Object::Instance()->CreateOBJ("OniKari1-2", "OniOBJ/", true);
	//攻撃モーション
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1", "OniOBJ/", true);
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2", "OniOBJ/", true);
	//ノックバック
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("OniNockback1", "", true);
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("OniNockback2", "", true);

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
}

void OniType::Draw(EnemyData* oniData)
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
#endif
		break;
	case MOVE:
		Object::Instance()->Draw(enemyObject[oniData->walkNum], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
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
		EffectDraw(oniData);
#if _DEBUG
		switch (oniData->attackDirection)
		{
		case Up:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + attackField.y / 2),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Down:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - attackField.y / 2),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Left:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x - attackField.y / 2, oniData->position.y, oniData->position.z),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Right:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x + attackField.y / 2, oniData->position.y, oniData->position.z),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		}
#endif
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

void OniType::FallDownDraw(EnemyData* oniData)
{//倒れる敵の描画
	Object::Instance()->Draw(enemyObject[0], oniData->position, oniData->scale, 
		oniData->angle + DirectionAngle(oniData->direction), oniData->color);
}

void OniType::Move(EnemyData* oniData, Player* player)
{
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
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
}

void OniType::SearchPlayer(EnemyData* oniData, Player* player)
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

void OniType::Attack(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		return;
	}

	Box attackBox = AttackField(oniData);
	//攻撃モーション中のダメージを与えるタイミング
	if (oniData->StatusTime == attackMotionDamege && player->GetInvincivleTime() == 0)
	{
		//攻撃範囲内にいたらプレイヤーにダメージ
		if (Collision::CheckBox2Box(attackBox, player->GetBox()))
		{
			//エフェクトのフラグTRUE
			AttackEffect = true;
			player->Damage(1);
		}
	}
	//エフェクト関係
	if (AttackEffect == true) {
		effectCount++;
		if (effectCount < 9) {
			oniData->attakFlag = false;
		}
		else if (effectCount == 9) {
			effectCount = 0;
			AttackEffect = false;
			oniData->attakFlag = true;
		}
	}
	else {
		oniData->attakFlag = false;
	}
	//時間が終わったら索敵にもどる
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
	}
}

void OniType::EffectDraw(EnemyData* oniData)
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

Box OniType::SearchField(EnemyData* oniData)
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

Vec3 OniType::DirectionAngle(int direction)
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

Box OniType::AttackField(EnemyData* oniData)
{
	Box attackBox;
	switch (oniData->attackDirection)
	{
	case Up:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.x / 2, oniData->position.y, oniData->position.z + attackField.y, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.x / 2, oniData->position.y, oniData->position.z, 1);
		break;
	case Left:
		attackBox.maxPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.y, oniData->position.y, oniData->position.z - attackField.x / 2, 1);
		break;
	case Right:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.y, oniData->position.y, oniData->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z - attackField.x / 2, 1);
		break;
	case Down:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.x / 2, oniData->position.y, oniData->position.z, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.x / 2, oniData->position.y, oniData->position.z - attackField.y, 1);
		break;
	default:
		attackBox.maxPosition = {};
		attackBox.minPosition = {};
	}
	return attackBox;
}
