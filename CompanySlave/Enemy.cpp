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
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//敵HP
	hpGaugeOBJ = Shape::CreateRect(12.0f, 1.8f);
	hpOBJ = Shape::CreateRect(15.0f, 2.0f);
	hpGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/EnemyGauge.png");
	hpGaugeGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/HP.png");
	//血
	Blood = Shape::CreateRect(10, 10);
	BloodGraph = Object::Instance()->LoadTexture(L"Resources/Blood/Blood.png");


	//鬼１
	oniType.Init();
	//弓
	oniBow.Init();

	for (int i = 0; i < eNumMax; i++) {
		particleFlag[i] = false;
		particleTime[i] = 60;
	}
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
	for (int i = 0; i < eNumMax; i++) {
		particleFlag[i] = false;
		particleTime[i] = 60;
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
				eData[num]->type = Oni;
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
			switch (eData[i]->type)
			{
			case Oni:
				UpdateOni(i, player);
				break;
			case OniBow:
				UpdateBow(i, player);
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
			switch (eData[i]->type)
			{
			case Oni:
				oniType.Draw(eData[i]);
				break;
			case OniBow:
				oniBow.Draw(eData[i]);
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

void Enemy::DamegeSword(int i)
{
	eData[i]->HP -= 0.3;
	eData[i]->damegeTime = 10;
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
			particleFlag[i] = true;

		}
		if (particleFlag[i] == true) {
			if (particleTime[i] > 0) {
				particleTime[i]--;
			}
		}
		if (particleTime[i] == 0) {
			particleFlag[i] = false;
		}

	}
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

void Enemy::UpdateOni(int i, Player *player)
{
	switch (eData[i]->Status)
	{
	case NORMAL:
		oniType.SearchPlayer(eData[i], player);
		break;
	case MOVE:
		eData[i]->direction = Direction(i, player);
		//移動
		oniType.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		oniType.Attack(eData[i], player);
		break;
	}
}

void Enemy::UpdateBow(int i, Player *player)
{
	switch (eData[i]->Status)
	{
	case NORMAL:
		oniBow.SearchPlayer(eData[i], player);
		break;
	case MOVE:
		eData[i]->direction = Direction(i, player);
		//移動
		oniBow.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		oniBow.Attack(eData[i], player);
		break;
	}
}


