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
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//敵HP
	hpGaugeOBJ = Shape::CreateRect(12.0f, 1.8f);
	hpOBJ = Shape::CreateRect(15.0f, 2.0f);
	hpGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/EnemyGauge.png");
	hpGaugeGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/HP.png");
	//血
	Blood = Shape::CreateRect(10, 10);
	BloodGraph = Object::Instance()->LoadTexture(L"Resources/Blood/Blood.png");

	//敵エネミー攻撃
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2");
	
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

void Enemy::StageInit(int stageNum)
{
	//メモリ解放
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
		//血の初期化
		BloodFlag[i] = false;

	}

	char *Filepath = (char *)"";
	switch (stageNum)
	{
	case 1:
		Filepath = (char *)"Resources/map/stage01spawnMap.csv";
		break;
	case 2:
		Filepath = (char *)"Resources/map/stage01spawnMap.csv";
		break;
	default:
		break;
	}

	LoadCSV(spawnMap, Filepath);
	int  num = 0;
	for (size_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (size_t i = 0; i < MAP_WIDTH; i++)
		{
			switch (spawnMap[j][i])
			{
			case ONESPAWN:
				eData.push_back(new EnemyData);
				num = eData.size() - 1;
				eData[num]->enemyObject = Object::Instance()->CreateOBJ("OniKari");
				eData[num]->position = { basePosition.x + i * mapSize, 0, basePosition.y + j * (-mapSize) };
				eData[num]->oldPosition = eData[eData.size() - 1]->position;
				//座標を合わせる
				eData[num]->eBox.minPosition = XMVectorSet(
					eData[num]->position.x - eData[num]->r, eData[num]->position.y - eData[num]->r,
					eData[num]->position.z - eData[num]->r, 1);
				eData[num]->eBox.maxPosition = XMVectorSet(
					eData[num]->position.x + eData[num]->r, eData[num]->position.y + eData[num]->r,
					eData[num]->position.z + eData[num]->r, 1);
				eData[num]->direction = Left;
				break;
			default:
				break;
			}
		}
	}
}

void Enemy::Update(Player *player)
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->HP > 0)
		{
			switch (eData[i]->Status)
			{
			case NORMAL:
				SearchPlayer(i, player);
				break;
			case MOVE:
				eData[i]->direction = Direction(i, player);
				//移動
				Move(i, player);
				break;
			case ATTACK:
				eData[i]->direction = Direction(i, player);
				Attack(i, player);
				break;
			case ENEMIES:
				break;
			}
		}

		//座標を合わせる
		eData[i]->eBox.minPosition = XMVectorSet(eData[i]->position.x - eData[i]->r, eData[i]->position.y - eData[i]->r, eData[i]->position.z - eData[i]->r, 1);
		eData[i]->eBox.maxPosition = XMVectorSet(eData[i]->position.x + eData[i]->r, eData[i]->position.y + eData[i]->r, eData[i]->position.z + eData[i]->r, 1);
		eData[i]->eSphere.center = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z, 1);

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
		if (eData[i]->HP > 0)
		{

			switch (eData[i]->Status)
			{
			case NORMAL:
				Object::Instance()->Draw(eData[i]->enemyObject, eData[i]->position, eData[i]->scale, DirectionAngle(eData[i]->direction), eData[i]->color);
				switch (eData[i]->direction)
				{
				case Up:
					Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackEnemies.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					break;
				case Down:
					Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackEnemies.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					break;
				case Left:
					Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x - attackEnemies.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					break;
				case Right:
					Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x + attackEnemies.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					break;
				}
				break;
			case MOVE:
				Object::Instance()->Draw(eData[i]->enemyObject, eData[i]->position, eData[i]->scale, DirectionAngle(eData[i]->direction), eData[i]->color);
				break;
			case ATTACK:
				if (eData[i]->StatusTime >= attackMotionDamege)
				{//武器振り上げ
					Object::Instance()->Draw(attackOBJ[0], eData[i]->position, eData[i]->scale, DirectionAngle(eData[i]->attackDirection), eData[i]->color);
				}
				else
				{//武器振り下ろし
					Object::Instance()->Draw(attackOBJ[1], eData[i]->position, eData[i]->scale, DirectionAngle(eData[i]->attackDirection), eData[i]->color);
				}
				EffectDraw(i);
				switch (eData[i]->attackDirection)
				{
				case Up:
					if (eData[i]->StatusTime == attackMotionDamege)
					{
						Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackField.y / 2),
							eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					}
					break;
				case Down:
					if (eData[i]->StatusTime == attackMotionDamege)
					{
						Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackField.y / 2),
							eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					}
					break;
				case Left:
					if (eData[i]->StatusTime == attackMotionDamege)
					{
						Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x - attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
							eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					}
					break;
				case Right:
					if (eData[i]->StatusTime == attackMotionDamege)
					{
						Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x + attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
							eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
					}
					break;
				}
				break;
			}
			//HPゲージ
			float parsent = eData[i]->HP / eData[i]->HPMax;
			if (parsent != 1.0f)
			{
				Object::Instance()->Draw(hpGaugeOBJ, Vec3(eData[i]->position.x - (1.0f - parsent) * 6.0, eData[i]->position.y + 5.0f, eData[i]->position.z + 5.0f),
					Vec3(parsent, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGaugeGraph);
				//HP枠
				Object::Instance()->Draw(hpOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 5.1f, eData[i]->position.z + 5.0f),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGraph);
			}
		}
		/*	if (BloodFlag[i] == true) {
				Object::Instance()->Draw(Blood, Vec3(BloodPosition[i].x, BloodPosition[i].y - 2.0f, BloodPosition[i].z),
					Vec3(1, 1, 1), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), BloodGraph);
			}*/
	}


}

void Enemy::BloodDraw()
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (BloodFlag[i] == true) {
			Object::Instance()->Draw(Blood, Vec3(BloodPosition[i].x, BloodPosition[i].y - 4.9f, BloodPosition[i].z),
				Vec3(1, 1, 1), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), BloodGraph);
		}
	}
}


void Enemy::WasAttack(int i)
{
	//eData[i]->wasAttackFlag = true;
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

	//プレイヤーとエネミーの位置の差
	Vec3 memoryPosition = player->GetPosition() - eData[i]->position;
	//長さを求める
	float length = memoryPosition.length();

	if (length < player2EnemyLength)
	{
		eData[i]->Status = ATTACK;
		eData[i]->attackDirection = eData[i]->direction;
		eData[i]->StatusTime = attackMotionTime;
	}
	else
	{
		//プレイヤーの向き
		Vec3 direction = memoryPosition.normalize();
		eData[i]->position += direction * eData[i]->speed;
	}
}

void Enemy::SearchPlayer(int i, Player *player)
{
	Box enemiesBox = SearchField(i);
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーと敵の距離
		float Length = Vec3(player->GetPosition() - eData[i]->position).length();

		if (Length > player2EnemyLength)
		{//プレイヤーが攻撃距離にいなかったら移動
			eData[i]->Status = MOVE;
			eData[i]->StatusTime = moveTime;
		}
		else
		{//プレイヤーが攻撃距離にいたら
			eData[i]->Status = ATTACK;
			eData[i]->attackDirection = eData[i]->direction;
			eData[i]->StatusTime = attackMotionTime;
		}
	}
}

void Enemy::Attack(int i, Player* player)
{
	Box attackBox = AttackField(i);
	//攻撃モーション中のダメージを与えるタイミング
	if (eData[i]->StatusTime == attackMotionDamege)
	{


		//攻撃範囲内にいたらプレイヤーにダメージ
		if (Collision::CheckBox2Box(attackBox, player->GetBox()))
		{
			//エフェクトのフラグTRUE
			AttackEffect = true;

			player->Damage();

		}
	}
	//エフェクト関係
	if (AttackEffect == true) {

		if (effectCount < 8) {
			effectCount++;
		}
		else if (effectCount == 8) {
			effectCount = 0;
			AttackEffect = false;
		}
	}
	//時間が終わったら索敵にもどる
	eData[i]->StatusTime--;
	if (eData[i]->StatusTime <= 0)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::Delete()
{
	//敵が消える
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		BloodPosition[i] = eData[i]->position;
		if (eData[i]->HP <= 0)
		{
			BloodFlag[i] = true;
		}

	}
}

Box Enemy::SearchField(int i)
{
	Box enemiesBox;
	switch (eData[i]->direction)
	{
	case Up:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z + attackEnemies.y, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		break;
	case Left:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.y, eData[i]->position.y, eData[i]->position.z - attackEnemies.x / 2, 1);
		break;
	case Right:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.y, eData[i]->position.y, eData[i]->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackEnemies.x / 2, 1);
		break;
	case Down:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z - attackEnemies.y, 1);
		break;
	default:
		enemiesBox.maxPosition = {};
		enemiesBox.minPosition = {};
	}
	return enemiesBox;
}

Box Enemy::AttackField(int i)
{
	Box attackBox;
	switch (eData[i]->attackDirection)
	{
	case Up:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.x / 2, eData[i]->position.y, eData[i]->position.z + attackField.y, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		break;
	case Left:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.y, eData[i]->position.y, eData[i]->position.z - attackField.x / 2, 1);
		break;
	case Right:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.y, eData[i]->position.y, eData[i]->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackField.x / 2, 1);
		break;
	case Down:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.x / 2, eData[i]->position.y, eData[i]->position.z - attackField.y, 1);
		break;
	default:
		attackBox.maxPosition = {};
		attackBox.minPosition = {};
	}
	return attackBox;
}

int Enemy::Direction(int i, Player *player)
{
	int direction = Right;

	float X = eData[i]->position.x - player->GetPosition().x;
	if (X < 0)
	{
		X *= -1;
	}
	float Z = eData[i]->position.z - player->GetPosition().z;
	if (Z < 0)
	{
		Z *= -1;
	}

	if (X > Z)
	{
		if (eData[i]->position.x < player->GetPosition().x)
		{
			return Right;
		}
		else
		{
			return Left;
		}
	}
	else
	{
		if (eData[i]->position.z < player->GetPosition().z)
		{
			return Up;
		}
		else
		{
			return Down;
		}
	}
}

Vec3 Enemy::DirectionAngle(int direction)
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
void Enemy::EffectDraw(int i)
{
	if (AttackEffect == true)
	{
		switch (eData[i]->attackDirection)
		{

		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + eData[i]->r + AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - eData[i]->r - AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x - eData[i]->r - AttackEffectSize, eData[i]->position.y, eData[i]->position.z), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x + eData[i]->r + AttackEffectSize, eData[i]->position.y, eData[i]->position.z), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x + AttackEffectSize, eData[i]->position.y, eData[i]->position.z + AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x - AttackEffectSize, eData[i]->position.y, eData[i]->position.z + AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x + AttackEffectSize, eData[i]->position.y, eData[i]->position.z - AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(eData[i]->position.x - AttackEffectSize, eData[i]->position.y, eData[i]->position.z - AttackEffectSize), AttackScale, AttackAngle, eData[i]->color, AttackEffectGraph[effectCount]);

			break;
		}
	}
	//AttackEffect = true;
}

