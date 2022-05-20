#include "TwinBoar.h"
#include"Player.h"
#include"Shape.h"
TwinBoar::TwinBoar()
{}

TwinBoar::~TwinBoar()
{}

void TwinBoar::Init()
{
#if _DEBUG
	debugField = Shape::CreateRect(attackField.y * 2, attackField.x * 2);
#endif
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//通常状態
	enemyObject[0] = Object::Instance()->CreateOBJ("Boar1-0", "BoarOBJ/");
	enemyObject[1] = Object::Instance()->CreateOBJ("Boar1-1", "BoarOBJ/");
	enemyObject[2] = Object::Instance()->CreateOBJ("Boar1-2", "BoarOBJ/");
	enemyObject[3] = Object::Instance()->CreateOBJ("Boar1-3", "BoarOBJ/");
	enemyObject[4] = Object::Instance()->CreateOBJ("Boar1-4", "BoarOBJ/");
	//攻撃モーション
	attackOBJ[0] = Object::Instance()->CreateOBJ("Boar1-0", "BoarOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("Boar1-1", "BoarOBJ/");
	attackOBJ[2] = Object::Instance()->CreateOBJ("Boar1-2", "BoarOBJ/");
	attackOBJ[3] = Object::Instance()->CreateOBJ("Boar1-3", "BoarOBJ/");
	attackOBJ[4] = Object::Instance()->CreateOBJ("Boar1-4", "BoarOBJ/");


	//ボスの数値
	bossData.HP = 168.0f;
	bossData.HPMax = 168.0f;
	bossData.scale = { 1.5f,1.5f,1.5f };
	bossData.r = 10.0f;
	bossData.bossFlag = true;
	bossData.nockPossibleFlag = false;
}

void TwinBoar::Draw(EnemyData* oniData, int i)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case  BOSSATTACK://小突進
		if (oniData->StatusTime >= sAttackMotionTime - sAttackHoldTime)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= 0)
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else
		{//移動中
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	case BOSSATTACK2://同時突進
		if (attackStats == PREOPERATION)//攻撃準備
		{
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackStats == DOUBLEATTACK)//攻撃中
		{
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else if (attackStats == DOUBLEAFTER)//攻撃後の隙
		{
			Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	case BOSSATTACK3://交互突進
		if (attackStats == PREOPERATION && bFlag[i] == true && bFinishFlag[i] == false)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackStats == DOUBLEATTACK && bFlag[i] == true && bFinishFlag[i] == false)
		{
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else
		{
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	}
}

void TwinBoar::LoopInit()
{
	searchEnemyFlag = false;
}

void TwinBoar::SearchPlayer(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	Box enemiesBox = SearchField(oniData);
	oniData->direction = Down;
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーと敵の距離
		float Length = Vec3(player->GetPosition() - oniData->position).length();
		if (Length < player2EnemyLength)
		{//プレイヤーが攻撃距離にいたら
			oniData->Status = BOSSATTACK;//小突撃
			searchEnemyFlag = true;
			boarNum = boarNumMax;
			sTime = sTimeMax;
			sAttack[0] = false;
			sAttack[1] = false;
		}
	}
}

void TwinBoar::AttackShortRush(EnemyData* oniData, Player* player, int i)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	sTime--;
	//移動
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	if (oniData->StatusTime <= 0 && sTime >= 0)
	{
		//プレイヤーとエネミーの位置の差
		Vec3 memoryPosition = player->GetPosition() - oniData->position;
		//長さを求める
		float length = memoryPosition.length();
		if (length < sPlayer2EnemyLength)
		{
			//プレイヤーとエネミーの位置の差
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//プレイヤーの向き
			oniData->pDirection = memoryPosition.normalize();
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = sAttackMotionTime;
		}
		else
		{
			//プレイヤーの向き
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
	}
	//攻撃
	//攻撃を構える時間
	if (oniData->StatusTime >= sAttackMotionTime - sAttackHoldTime)
	{

	}
	//攻撃時間中
	else if (oniData->StatusTime > 0)
	{
		//攻撃範囲内にいたらプレイヤーにダメージ
		if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
		}
		//移動攻撃
		oniData->oldPosition = oniData->position;
		slowValue = player->GetSlow();
		oniData->position += oniData->pDirection * sAttackSpeed * slowValue;
	}

	//時間が終わったら突進交互か２体同時突進
	oniData->StatusTime--;

	if (oniData->StatusTime <= 0 && sTime <= 0)
	{
		if (fixedPosition[i].x - 5.0f < oniData->position.x && fixedPosition[i].x + 5.0f > oniData->position.x &&
			fixedPosition[i].z - 5.0f < oniData->position.z && fixedPosition[i].z + 5.0f > oniData->position.z)
		{//所定の位置へ移動したら
			sAttack[i] = true;
		}
		else
		{//所定の位置へ移動		
			oniData->bowAngle = atan2(fixedPosition[i].z - oniData->position.z, fixedPosition[i].x - oniData->position.x);
			Vec3 memoryPosition = fixedPosition[i] - oniData->position;
			oniData->pDirection = memoryPosition.normalize();
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveFixedSpeed * slowValue;
		}
	}

	if (sTime <= 0 && sAttack[0] == true && sAttack[1] == true)
	{
		if (attackFlag == true)
		{
			oniData->Status = BOSSATTACK2;
			attackStats = PREOPERATION;
			doublePreTime = doublePreTimeMax;
		}
		else
		{
			oniData->Status = BOSSATTACK3;
			altAttackNum = 0;
			bFlag[0] = false;
			bFlag[1] = false;
			bFinishFlag[0] = false;
			bFinishFlag[1] = false;
		}
		boarNum = boarNumMax;
	}
}


void TwinBoar::AttackRush(EnemyData* oniData, Player* player, int num)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//攻撃を行っていない＆突進をまだしていなかったら
	if (bFlag[num] == false && boarNum > 0 && attackFlag == false)
	{
		attackStats = PREOPERATION;
		bPreTime = bPreTimeMax;
		bFlag[num] = true;
		attackFlag = true;
		oniData->attackDirection = oniData->direction;
	}
	//攻撃中
	if (bFlag[num] == true && bFinishFlag[num] == false)
	{
		if (attackStats == PREOPERATION)
		{//攻撃予備動作

			bPreTime--;
			if (bPreTime <= 0)
			{
				attackStats = DOUBLEATTACK;
				oniData->attackDirection = oniData->direction;
				//プレイヤーとエネミーの位置の差
				Vec3 memoryPosition = player->GetPosition() - oniData->position;
				//プレイヤーの向き
				oniData->pDirection = memoryPosition.normalize();
				dAttackTime = dAttackTimeMax;
			}
		}
		else if (attackStats == DOUBLEATTACK)
		{
			//攻撃範囲内にいたらプレイヤーにダメージ
			if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(1);
			}
			//移動攻撃
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
			dAttackTime -= 1;
			if (dAttackTime <= 0)
			{//突進が終わったら
				attackStats = DOUBLEAFTER;
			}
		}
		else
		{
			boarNum--;
			attackFlag = false;
			bFinishFlag[num] = true;
		}
	}

	//交互に突進３回終わったら
	if (boarNum <= 0 && altAttackNum >= 3)
	{
		oniData->Status = BOSSATTACK;
		sTime = sTimeMax;
		sAttack[0] = false;
		sAttack[1] = false;
		attackFlag = true;
	}
	//交互に突進を３回行うための処理
	if (boarNum <= 0 && altAttackNum < 3)
	{
		altAttackNum++;
		boarNum = boarNumMax;
		bFlag[0] = false;
		bFlag[1] = false;
		bFinishFlag[0] = false;
		bFinishFlag[1] = false;
	}
}

void TwinBoar::AttackDoubleRush(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	if (attackStats == PREOPERATION)
	{//攻撃予備動作
		doublePreTime--;
		//プレイヤーとエネミーの位置の差
		Vec3 memoryPosition = player->GetPosition() - oniData->position;
		//プレイヤーの向き
		oniData->pDirection = memoryPosition.normalize();
		oniData->attackDirection = oniData->direction;
		if (doublePreTime <= 0)
		{//準備時間が終わったら攻撃モードに映る
			attackStats = DOUBLEATTACK;
			doubleTime = doubleTimeMax;
			boarNum = boarNumMax;

		}
	}
	else if (attackStats == DOUBLEATTACK)//攻撃中
	{
		//攻撃範囲内にいたらプレイヤーにダメージ
		if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
		}
		if (boarNum == 2)
		{
			//移動攻撃
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
		}
		doubleTime -= 1;
		if (doubleTime <= 0)
		{//突進が終わったら
			attackStats = DOUBLEAFTER;
			boarNum--;
			//2体とも通ったら
			if (boarNum == 0)
			{
				doubleAfterTime = doubleAfterTimeMax;
			}
		}
	}
	else
	{//攻撃後の隙の時間2秒

		doubleAfterTime--;
		if (doubleAfterTime <= 0)
		{
			attackFlag = false;
			oniData->Status = BOSSATTACK;
			sTime = sTimeMax;
			sAttack[0] = false;
			sAttack[1] = false;
		}
	}
}

Box TwinBoar::SearchField(EnemyData* oniData)
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

Vec3 TwinBoar::DirectionAngle(int direction)
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
