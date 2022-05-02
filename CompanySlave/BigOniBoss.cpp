#include "BigOniBoss.h"
#include"Player.h"
#include"Shape.h"
BigOniBoss::BigOniBoss()
{}

BigOniBoss::~BigOniBoss()
{}

void BigOniBoss::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);
	attackBigOBJ = Shape::CreateRect(attackBigField.x, attackBigField.y);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//通常状態
	enemyObject = Object::Instance()->CreateOBJ("OniKari");
	//攻撃モーション
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2");
	//ノックバック
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("OniNockback1");
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("OniNockback2");

	//攻撃エフェクト
	//AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect9.png");
	//ボスの数値
	bossData.HP = 84.0f;
	bossData.HPMax = 84.0f;
	bossData.scale = { 4.0f,4.0f,4.0f };
	bossData.r = 15;
	bossData.bossFlag = true;
}

void BigOniBoss::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
#if _DEBUG
		if (oniData->direction == Up) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z + attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Down) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z - attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Left) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x - attackEnemies.y / 2, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Right) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x + attackEnemies.y / 2, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
#endif
	case  BOSSATTACK:
		if (attackSmallTime >= attackMotionDamege)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackSmallTime > 0)
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//移動中
			Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
#if _DEBUG
		if (attackSmallTime >= attackMotionDamege)
		{
			switch (oniData->attackDirection)
			{
			case Up:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, 0.0f, oniData->position.z + attackField.y / 2),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
				break;
			case Down:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, 0.0f, oniData->position.z - attackField.y / 2),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
				break;
			case Left:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x - attackField.y / 2, 0.0f, oniData->position.z),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
				break;
			case Right:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x + attackField.y / 2, 0.0f, oniData->position.z),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
				break;
			}
		}
#endif
		break;
	case BOSSATTACK2:
		if (attackBigStatus == PREOPERATION)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
			Object::Instance()->Draw(attackBigOBJ, oniData->position, Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
		else
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	case SUMMON:
		if (attackSmallTime >= attackMotionDamege)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackSmallTime > 0)
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//移動中
			Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	case NOCKBACK:
		if (oniData->nockbackTime >= 3)
		{
			Object::Instance()->Draw(nockBackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		else
		{
			Object::Instance()->Draw(nockBackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	}
}

void BigOniBoss::DrawHP(EnemyData* oniData)
{



}


void BigOniBoss::SearchPlayer(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	Box enemiesBox = SearchField(oniData);
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーと敵の距離
		float Length = Vec3(player->GetPosition() - oniData->position).length();
		if (Length < player2EnemyLength)
		{//プレイヤーが攻撃距離にいたら
			oniData->Status = BOSSATTACK;//小打撃
			attackSmallNum = 0;			//小攻撃回数リセット
			attackSmallTime = 0;

		}
	}
}

void BigOniBoss::AttackSmall(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	//破になったら攻撃をやめる
	if (oniData->explosionFlag == true)
	{
		attackSmallTime = 0;
	}

	//プレイヤーと敵の距離
	float Length = Vec3(player->GetPosition() - oniData->position).length();
	//攻撃中時間じゃなかったらこの処理を行う
	if (attackSmallTime <= 0 && attackSmallNum < 3)
	{
		if (Length > attackSmallLength)
		{//攻撃するためにプレイヤーに近づく処理
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			//プレイヤーとエネミーの位置の差
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//長さを求める
			float length = memoryPosition.length();
			//プレイヤーの向き
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
		else
		{
			//射程範囲にプレイヤーがいるので攻撃する処理	
			attackSmallNum++;
			attackSmallTime = attackSmallTimeMax;
			oniData->attackDirection = oniData->direction;

		}
	}
	//攻撃中
	if (attackSmallTime > 0)
	{
		Box attackBox = AttackField(oniData);
		//攻撃モーション中のダメージを与えるタイミング
		if (attackSmallTime == attackMotionDamege)
		{
			//攻撃範囲内にいたらプレイヤーにダメージ
			if (Collision::CheckBox2Box(attackBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(2);
			}
		}
		attackSmallTime--;
		if (attackSmallTime <= 0 && attackSmallNum == 3)
		{
			start_time = time(NULL);//計測開始
		}

		//攻撃を2回または破が成立したとき攻撃をやめる
		if (oniData->explosionFlag == true)
		{
			oniData->nockbackTime = EnemySupport::nockBackTimeMax;
		}
	}

	//3回小攻撃を終えたら次の攻撃へ移る
	if (attackSmallNum == 3 && attackSmallTime <= 0)//攻撃時間が０になったら
	{
		end_time = time(NULL);
		int smallTime = end_time - start_time;//時間計算
		if (smallTime > 3)
		{
			attackSmallNum = 0;
			if (attackTypeFlag == true)
			{//大攻撃へ
				oniData->Status = BOSSATTACK2;
				attackBigStatus = PREOPERATION;
				big_start = time(NULL);
				oniData->nockPossibleFlag = false;
				attackTypeFlag = false;
			}
			else
			{//召喚攻撃へ
				oniData->Status = SUMMON;
				firstSummonFlag = true;
				attackTypeFlag = true;
			}
		}
	}
}

void BigOniBoss::AttackBig(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	if (attackBigStatus == PREOPERATION)
	{
		big_end = time(NULL);
		int bigTime = big_end - big_start;
		//７秒経ったら攻撃
		if (bigTime >= 7)
		{
			attackBigStatus = BIGATTACK;
		}
	}
	else if (attackBigStatus == BIGATTACK)
	{
		//当たり判定
		if (Collision::CheckBox2Box(player->GetBox(), AttackBIG(oniData)))
		{
			player->Damage(4);
		}
		start_time = time(NULL);//計測開始
		attackBigStatus = BIGAFTER;
	}
	else if (attackBigStatus == BIGAFTER)
	{
		//攻撃がおわったら次の攻撃へ移る
		end_time = time(NULL);
		int nextTime = end_time - start_time;
		if (nextTime >= 3)
		{
			oniData->nockPossibleFlag = true;
			oniData->Status = BOSSATTACK;
		}
	}
}

void BigOniBoss::AttackSummon(EnemyData* oniData, Player* player, int eNum)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//最初だけ召喚する
	if (firstSummonFlag == true)
	{
		firstSummonFlag = false;
	}

	//プレイヤーと敵の距離
	float Length = Vec3(player->GetPosition() - oniData->position).length();
	//攻撃中時間じゃなかったらこの処理を行う
	if (attackSmallTime <= 0)
	{
		if (Length > attackSmallLength)
		{//攻撃するためにプレイヤーに近づく処理
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			//プレイヤーとエネミーの位置の差
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//長さを求める
			float length = memoryPosition.length();
			//プレイヤーの向き
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
		else
		{
			//射程範囲にプレイヤーがいるので攻撃する処理	
			attackSmallTime = attackSmallTimeMax;
			oniData->attackDirection = oniData->direction;

		}
	}
	//攻撃中
	if (attackSmallTime > 0)
	{
		Box attackBox = AttackField(oniData);
		//攻撃モーション中のダメージを与えるタイミング
		if (attackSmallTime == attackMotionDamege)
		{
			//攻撃範囲内にいたらプレイヤーにダメージ
			if (Collision::CheckBox2Box(attackBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(2);
			}
		}
		attackSmallTime--;
	}

	//子分がいなくなったら次の攻撃へ移る
	if (eNum <= 1 && attackSmallTime <= 0)
	{
		oniData->Status = BOSSATTACK;
	}
}


void BigOniBoss::EffectDraw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	/*if (AttackEffect == true)
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
	}*/
}

Box BigOniBoss::SearchField(EnemyData* oniData)
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

Vec3 BigOniBoss::DirectionAngle(int direction)
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

Box BigOniBoss::AttackField(EnemyData* oniData)
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

Box BigOniBoss::AttackBIG(EnemyData* oniData)
{
	Box attackBox;
	attackBox.maxPosition = XMVectorSet(oniData->position.x + attackBigField.x / 2, oniData->position.y, oniData->position.z + attackBigField.y, 1);
	attackBox.minPosition = XMVectorSet(oniData->position.x - attackBigField.x / 2, oniData->position.y, oniData->position.z - attackBigField.y, 1);
	return attackBox;
}
