#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Enemy::Enemy()
{}

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

	BloodFrount = Shape::CreateRect(10, 10);
	BloodBack = Shape::CreateRect(10, 10);
	Blood2Graph[0] = Object::Instance()->LoadTexture(L"Resources/Eblood/BloodFront1.png");
	Blood2Graph[1] = Object::Instance()->LoadTexture(L"Resources/Eblood/BloodFront2.png");
	Blood2GraphBack[0] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback0.png");
	Blood2GraphBack[1] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback1.png");
	Blood2GraphBack[2] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback2.png");


	explosionOBJ = Shape::CreateRect(10, 10);
	explosionGraph = Object::Instance()->LoadTexture(L"Resources/Point.png");
	//鬼１
	oniType.Init();
	//弓
	oniBow.Init();

	for (int i = 0; i < eNumMax; i++) {

		BloodTime[i] = bloodTimeMax;
		delayCount[i] = 0;
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
		explosionFlag[i] = false;
	}
	for (int i = 0; i < eNumMax; i++) {

		BloodTime[i] = bloodTimeMax;
	}
	char* Filepath = (char*)"";
	switch (stageNum)
	{
	case 1:
		Filepath = (char*)"Resources/map/stage01spawnMap.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/stage02spawnMap.csv";
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
			num = 0;
			switch (spawnMap[j][i])
			{
			case ONILEFT:
			case ONIUP:
			case ONIDOWN:
			case ONIRIGHT:
				eData.push_back(new EnemyData);
				num = eData.size() - 1;
				eData[num]->position = { basePosition.x + i * mapSize, 0, basePosition.y + j * (-mapSize) };
				eData[num]->oldPosition = eData[eData.size() - 1]->position;
				//座標を合わせる
				eData[num]->eBox.minPosition = XMVectorSet(
					eData[num]->position.x - eData[num]->r, eData[num]->position.y - eData[num]->r, eData[num]->position.z - eData[num]->r, 1);
				eData[num]->eBox.maxPosition = XMVectorSet(
					eData[num]->position.x + eData[num]->r, eData[num]->position.y + eData[num]->r, eData[num]->position.z + eData[num]->r, 1);
				eData[num]->type = Oni;
				break;
			case ONIBOWLEFT:
			case ONIBOWRIGHT:
			case ONIBOWUP:
			case ONIBOWDOWN:
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
				eData[num]->type = OniBow;
				break;
			default:
				break;
			}
			if (num > 0)
			{//敵の向きを設定１の桁
				switch (spawnMap[j][i] % 10)
				{
				case Up:
					eData[num]->direction = Up;
					break;
				case Down:
					eData[num]->direction = Down;
					break;
				case Left:
					eData[num]->direction = Left;
					break;
				case Right:
					eData[num]->direction = Right;
					break;
				}
			}
		}
	}
}

void Enemy::Update(Player* player)
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
			NockBack(i);
		}
		//矢の更新
		oniBow.BowUpdate(eData[i], player);

		//座標を合わせる
		eData[i]->eBox.minPosition = XMVectorSet(eData[i]->position.x - eData[i]->r, eData[i]->position.y - eData[i]->r, eData[i]->position.z - eData[i]->r, 1);
		eData[i]->eBox.maxPosition = XMVectorSet(eData[i]->position.x + eData[i]->r, eData[i]->position.y + eData[i]->r, eData[i]->position.z + eData[i]->r, 1);
		eData[i]->eSphere.center = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z, 1);

		if (eData[i]->damegeTime > 0)
		{
			eData[i]->damegeTime--;
		}
		if (eData[i]->explosionCount <= 0)
		{
			eData[i]->explosionFlag = true;

		}
		if (eData[i]->explosionCount == 1)
		{
			explosionFlag[i] = true;
		}
		else if (eData[i]->explosionCount == 2)
		{
			explosionGraphCnt[i] = 0;
		}
		if (eData[i]->explosionDelay)
		{
			delayCount[i]++;
			if (delayCount[i] >= 20)
			{
				eData[i]->explosionCount = 2;
				eData[i]->explosionFlag = false;
				delayCount[i] = 0;
				eData[i]->explosionDelay = false;
			}
		}

		if (explosionFlag[i])
		{
			explosionGraphCnt[i]++;
		}
		if (explosionGraphCnt[i] > 30)
		{
			explosionFlag[i] = false;
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
			//敵　破　
			if (explosionFlag[i] == true) {
				Object::Instance()->Draw(explosionOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f, eData[i]->position.z),
					Vec3(0.5f, 0.5f, 0.5f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), explosionGraph);
			}
		}
		if (eData[i]->HP <= 0)
		{
			if (BloodTime[i] > 20) {
			
				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[0]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[0]);
			}
			else if (BloodTime[i] > 10) {
				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[1]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[1]);
			}
			else if (BloodTime[i] > 0) {
				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[2]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[1]);
			}
		}
	}


}

void Enemy::BloodDraw()
{
	float size = 0.0f;
	for (size_t i = 0; i < eData.size(); i++)
	{//血痕の描画
		size += 0.01;
		if (BloodFlag[i] == true) {
			Object::Instance()->Draw(Blood, Vec3(BloodPosition[i].x, BloodPosition[i].y - 0.9f + size, BloodPosition[i].z),
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

void Enemy::DamegeNormal(int i, int pAttackDirection)
{
	eData[i]->HP -= 2;
	eData[i]->damegeTime = 10;
	eData[i]->nockbackTime = nockBackTimeMax;
	eData[i]->nockDirection = pAttackDirection;
}

void Enemy::DamegeThrowSword(int i)
{
	eData[i]->HP -= 2;
	eData[i]->explosionCount--;
	eData[i]->damegeTime = 10;
}

void Enemy::DamegeSword(int i)
{
	eData[i]->HP -= 2;
	eData[i]->explosionCount--;
	eData[i]->damegeTime = 10;
}


void Enemy::Delete()
{
	//敵が消える

	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		BloodPosition[i] = eData[i]->position;
		//BloodEffectが消えたら血痕が表示される
		if (eData[i]->HP <= 0)
		{
			//0になるまでタイムのマイナス
			if (BloodTime[i] > 0) {
				BloodTime[i]--;
			}

		}
		//0になったら血痕の表示
		if (BloodTime[i] <= 0) {
			BloodFlag[i] = true;
		}

	}

}



int Enemy::Direction(int i, Player* player)
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

bool Enemy::SetDamegeFlag(int i, bool DamegeFlag)
{
	return eData[i]->DamegeFlag = DamegeFlag;
}

bool Enemy::SetExplosionFlag(int i)
{
	return eData[i]->explosionFlag = false;
}

bool Enemy::SetExplosionCount(int i)
{
	return eData[i]->explosionDelay = true;
}

void Enemy::NockBack(int i)
{
	//ノックバック用
	if (eData[i]->nockbackTime > 0)
	{
		eData[i]->Status = NOCKBACK;
		eData[i]->nockbackTime--;
		if (eData[i]->nockbackTime == 0)
		{
			eData[i]->Status = MOVE;
		}
		switch (eData[i]->nockDirection)
		{
		case Up:
			eData[i]->position.z += nockPower;
			break;
		case Down:
			eData[i]->position.z -= nockPower;
			break;
		case Left:
			eData[i]->position.x -= nockPower;
			break;
		case Right:
			eData[i]->position.x += nockPower;
			break;
		case UpRight:
			eData[i]->position.z += nockPower;
			eData[i]->position.x += nockPower;
			break;
		case UpLeft:
			eData[i]->position.z += nockPower;
			eData[i]->position.x -= nockPower;
			break;
		case DownRight:
			eData[i]->position.z -= nockPower;
			eData[i]->position.x += nockPower;
			break;
		case DownLeft:
			eData[i]->position.z -= nockPower;
			eData[i]->position.x -= nockPower;
			break;
		}
	}
}

void Enemy::UpdateOni(int i, Player* player)
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

void Enemy::UpdateBow(int i, Player* player)
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


