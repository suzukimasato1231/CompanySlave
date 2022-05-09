#include "TwinBoar.h"
#include"Player.h"
#include"Shape.h"
TwinBoar::TwinBoar()
{}

TwinBoar::~TwinBoar()
{}

void TwinBoar::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	//attackBigOBJ = Shape::CreateRect(attackBigField.x, attackBigField.y);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//通常状態
	enemyObject[0] = Object::Instance()->CreateOBJ("OniKari1-0", "OniOBJ/");
	enemyObject[1] = Object::Instance()->CreateOBJ("OniKari1-1", "OniOBJ/");
	enemyObject[2] = Object::Instance()->CreateOBJ("OniKari1-2", "OniOBJ/");
	//攻撃モーション
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1", "OniOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2", "OniOBJ/");
	//ノックバック
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("OniNockback1");
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("OniNockback2");

	//ボスの数値
	bossData.HP = 168.0f;
	bossData.HPMax = 168.0f;
	bossData.scale = { 4.0f,4.0f,4.0f };
	bossData.r = 15.0f;
	bossData.bossFlag = true;
	bossData.nockPossibleFlag = false;
}

void TwinBoar::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case  BOSSATTACK://小突進
		if (oniData->StatusTime >= sAttackMotionTime)
		{//武器振り上げ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= sAttackHoldTime)
		{//武器振り下ろし
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//移動中
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	case BOSSATTACK2://同時突進
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
	case BOSSATTACK3://交互突進
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
		}
	}
}

void TwinBoar::AttackShortRush(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	//移動
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	if (oniData->StatusTime <= 0)
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
	else
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

	if (oniData->StatusTime == 0)
	{
		if (attackFlag == true)
		{
			oniData->Status = BOSSATTACK2;
			attackStats = PREOPERATION;
		}
		else
		{
			oniData->Status = BOSSATTACK3;
			altAttackNum = 0;
		}
		boarNum--;
		oniData->StatusTime = -1;
		if (boarNum <= 0)
		{//どっちの攻撃か切り替える
			if (attackFlag == true)
			{
				attackFlag = false;
			}
			else
			{
				attackFlag = true;
			}
			boarNum = boarNumMax;
			d_start = time(NULL);
		}
	}
}


void TwinBoar::AttackRush(EnemyData* oniData, Player* player, int num)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//攻撃を行っていない＆突進をまだしていなかったら
	if (altFlag == false && boarNum == num + 1)
	{
		altFlag = true;
		attackStats = PREOPERATION;
		d_start = time(NULL);
	}
	//攻撃中
	if (altFlag == true && boarNum == num + 1)
	{
		if (attackStats == PREOPERATION)
		{//攻撃予備動作
			d_end = time(NULL);

			int dTime = d_end - d_start;
			if (dTime >= 1)
			{
				attackStats = DOUBLEATTACK;
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
				player->Damage(3);
			}
			//移動攻撃
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
			dAttackTime -= 1;
			if (dAttackTime <= 0)
			{//突進が終わったら
				attackStats = DOUBLEAFTER;
				d_start = time(NULL);
			}
		}
		else
		{
			altFlag = false;
			boarNum--;
		}
	}

	//交互に突進３回終わったら
	if (altFlag == false && boarNum == 0 && altAttackNum >= 3)
	{
		oniData->Status = BOSSATTACK;
	}
	//交互に突進を３回行うための処理
	if (altFlag == false && boarNum == 0)
	{
		altAttackNum++;
		boarNum = boarNumMax;
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
		d_end = time(NULL);

		int dTime = d_end - d_start;
		if (dTime >= 2)
		{
			attackStats = DOUBLEATTACK;
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
			player->Damage(2);
		}
		//移動攻撃
		oniData->oldPosition = oniData->position;
		slowValue = player->GetSlow();
		oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
		dAttackTime -= 1;
		if (dAttackTime <= 0)
		{//突進が終わったら
			attackStats = DOUBLEAFTER;
			d_start = time(NULL);
		}
	}
	else
	{//攻撃後の隙の時間2秒
		d_end = time(NULL);
		int afterTime = d_end - d_start;

		if (afterTime >= 2)
		{
			oniData->Status = BOSSATTACK;
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
