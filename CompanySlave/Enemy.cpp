#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Enemy::Enemy()
{}

Enemy::~Enemy()
{
	//ÉÅÉÇÉäâï˙
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
	}
	//XAudio2âï˙
	audio->xAudio2.Reset();
	//âπÉfÅ[É^âï˙
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	Audio::SoundUnload(&sound3);
	Audio::SoundUnload(&sound4);
	safe_delete(audio);
}

void Enemy::Init()
{
	audio = Audio::Create();
	sound1 = Audio::SoundLoadWave("Resources/Music/SE/club.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/SE/Arrow.wav");
	sound3 = Audio::SoundLoadWave("Resources/Music/SE/wolf.wav");
	sound4 = Audio::SoundLoadWave("Resources/Music/SE/boar.wav");


	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//ìGHP
	hpGaugeOBJ = Shape::CreateRect(12.0f, 1.8f);
	hpOBJ = Shape::CreateRect(15.0f, 2.0f);
	hpGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/EnemyGauge.png");
	hpGaugeGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/HP.png");
	//åå
	Blood = Shape::CreateRect(10, 10);
	BloodGraph = Object::Instance()->LoadTexture(L"Resources/Blood/Blood.png");

	BloodFrount = Shape::CreateRect(10, 10);
	BloodBack = Shape::CreateRect(10, 10);
	Blood2Graph[0] = Object::Instance()->LoadTexture(L"Resources/Eblood/BloodFront1.png");
	Blood2Graph[1] = Object::Instance()->LoadTexture(L"Resources/Eblood/BloodFront2.png");
	Blood2GraphBack[0] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback0.png");
	Blood2GraphBack[1] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback1.png");
	Blood2GraphBack[2] = Object::Instance()->LoadTexture(L"Resources/Eblood/Bloodback2.png");

	bossSprite = Sprite::Instance()->SpriteCreate(L"Resources/EnemyUI/Boss_HP.png");
	bossHPSprite = Sprite::Instance()->SpriteCreate(L"Resources/EnemyUI/HP.png");

	explosionOBJ = Shape::CreateRect(10, 10);
	explosionGraph = Object::Instance()->LoadTexture(L"Resources/Point.png");
	//ãSÇP
	oniType.Init();
	//ã|
	oniBow.Init();
	//òT
	wolf.Init();
	//ÉCÉmÉVÉV
	boar.Init();
	//É{ÉXãS
	bigOniBoss.Init();
	//òTÇÃåQÇÍ
	wolfFlock.Init();
}

void Enemy::StageInit(int stageNum)
{
	//ÉÅÉÇÉäâï˙
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
		//ååÇÃèâä˙âª
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
		Filepath = (char*)"Resources/map/Enemy_Tile1.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/Enemy_Tile2.csv";
		break;
	case 3:
		Filepath = (char*)"Resources/map/Enemy_Tile3.csv";
		break;
	case 4:
		Filepath = (char*)"Resources/map/Enemy_Tile4.csv";
		break;
	case 5:
		Filepath = (char*)"Resources/map/Enemy_Tile5.csv";
		break;
	case 6:
		Filepath = (char*)"Resources/map/Enemy_Tile6.csv";
		break;
	case 7:
		Filepath = (char*)"Resources/map/Enemy_Tile7.csv";
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
			if (spawnMap[j][i] > 1)
			{
				eData.push_back(new EnemyData);
				num = eData.size() - 1;
				//ìGÇÃéÌóﬁÇê›íË
				switch (spawnMap[j][i])
				{//ãﬂê⁄ãS
				case ONILEFT:
				case ONIUP:
				case ONIDOWN:
				case ONIRIGHT:
					eData[num]->type = Oni;
					break;
					//âìãóó£ãS
				case ONIBOWLEFT:
				case ONIBOWRIGHT:
				case ONIBOWUP:
				case ONIBOWDOWN:
					eData[num]->type = OniBow;
					break;
					//òT
				case WOLFLEFT:
				case WOLFRIGHT:
				case WOLFUP:
				case WOLFDOWN:
					*eData[num] = wolf.WolfData();
					eData[num]->type = WolfType;
					break;
					//ÉCÉmÉVÉV
				case BOARLEFT:
				case BOARRIGHT:
				case BOARUP:
				case BOARDOWN:
					*eData[num] = boar.BoarData();
					eData[num]->type = BoarType;
					break;
				case BossOni:
					*eData[num] = bigOniBoss.GetBossData();
					eData[num]->type = BossBigOni;
					break;
				case WOLFFLOCK:
					*eData[num] = wolfFlock.GetBossData();
					eData[num]->type = BossWolfFlock;
					wolfFlock.LoopInit();
					break;
				default:
					break;
				}
				eData[num]->position = { basePosition.x + i * mapSize,eData[num]->position.y, basePosition.y + j * (-mapSize) };
				eData[num]->oldPosition = eData[eData.size() - 1]->position;
				//ç¿ïWÇçáÇÌÇπÇÈ
				eData[num]->eBox.minPosition = XMVectorSet(
					eData[num]->position.x - eData[num]->r, eData[num]->position.y - eData[num]->r, eData[num]->position.z - eData[num]->r, 1);
				eData[num]->eBox.maxPosition = XMVectorSet(
					eData[num]->position.x + eData[num]->r, eData[num]->position.y + eData[num]->r, eData[num]->position.z + eData[num]->r, 1);
				eData[num]->eSphere.radius = 5.0f;
				//ìGÇÃå¸Ç´Çê›íË
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

	for (int i = 0; i < eNumMax; i++) {

		BloodTime[i] = bloodTimeMax;
		delayCount[i] = 0;
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
			case WolfType:
				UpdateWolf(i, player);
				break;
			case BoarType:
				UpdateBoar(i, player);
				break;
			case BossBigOni:
				UpdateBigOniBoss(i, player);
				break;
			case BossWolfFlock:
				UpdateWolfFlock(i, player);
				break;
			}
			NockBack(i);
		}
		//ñÓÇÃçXêV
		oniBow.BowUpdate(eData[i], player);

		//ç¿ïWÇçáÇÌÇπÇÈ
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
		//âπ
		if (eData[i]->attakFlag == true) {
			audio->SoundSEPlayWave(sound1);//Ç±ÇÒñ_
		}
		if (eData[i]->attakWFlag == true) {
			audio->SoundSEPlayWave(sound3);//òT
		}
		if (eData[i]->attakBFlag == true) {
			audio->SoundSEPlayWave(sound4);//íñ
		}
		if (eData[i]->bowAFlag ==true) {
			audio->SoundSEPlayWave(sound2);//ã|
		}
	}
	//çÌèú
	Delete();
	audio->SetVolume(volume);
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
			case WolfType:
				wolf.Draw(eData[i]);
				break;
			case BoarType:
				boar.Draw(eData[i]);
				break;
			case BossBigOni:
				bigOniBoss.Draw(eData[i]);
				break;
			case BossWolfFlock:
				wolfFlock.Draw(eData[i]);
				break;
			}

			//HPÉQÅ[ÉW
			float parsent = eData[i]->HP / eData[i]->HPMax;
			if (parsent != 1.0f && eData[i]->bossFlag == false)
			{
				Object::Instance()->Draw(hpGaugeOBJ, Vec3(eData[i]->position.x - (1.0f - parsent) * 6.0, eData[i]->position.y + 5.0f + eData[i]->r, eData[i]->position.z + 5.0f),
					Vec3(parsent, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGaugeGraph);
				//HPòg
				Object::Instance()->Draw(hpOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 5.1f + eData[i]->r, eData[i]->position.z + 5.0f),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGraph);
			}
			//ìGÅ@îjÅ@
			if (explosionFlag[i] == true) {
				Object::Instance()->Draw(explosionOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f + eData[i]->r, eData[i]->position.z),
					Vec3(0.5f, 0.5f, 0.5f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), explosionGraph);
			}
		}
	}
}

void Enemy::DrawUI()
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->HP > 0 && eData[i]->bossFlag == true && eData[i]->Status != NORMAL)
		{
			float ratio = eData[i]->HP / eData[i]->HPMax;
			Sprite::Instance()->Draw(bossHPSprite, Vec2(150.0f, 650.0f), 1000.0f * ratio, 50.0f);
			Sprite::Instance()->Draw(bossSprite, Vec2(100.0f, 650.0f), 1100.0f, 50.0f);
		}
	}
}

void Enemy::BloodDraw()
{
	float size = 0.0f;
	for (size_t i = 0; i < eData.size(); i++)
	{//ååç≠ÇÃï`âÊ
		size += 0.01;
		if (BloodFlag[i] == true && eData[i]->type != BossWolfFlock) {
			Object::Instance()->Draw(Blood, Vec3(BloodPosition[i].x, 0.0f - 0.9f + size, BloodPosition[i].z),
				Vec3(1, 1, 1), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), BloodGraph);
		}
	}
}

void Enemy::SetPosition(int i, Vec3 position)
{
	eData[i]->position = position;
}

void Enemy::DamegeNormal(int i, int pAttackDirection)
{
	eData[i]->HP -= 2;
	eData[i]->damegeTime = 10;
	if (eData[i]->nockPossibleFlag == true)
	{//ÉmÉbÉNÉoÉbÉNÇ™â¬î\Ç»ÇÁ
		eData[i]->memoryStatus = eData[i]->Status;
		eData[i]->nockbackTime = EnemySupport::nockBackTimeMax;
		eData[i]->nockDirection = pAttackDirection;
	}
	if (eData[i]->Status == NORMAL)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::DamegeThrowSword(int i)
{
	eData[i]->HP -= 2;
	eData[i]->explosionCount--;
	eData[i]->damegeTime = 10;
	if (eData[i]->Status == NORMAL)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::DamegeSword(int i)
{
	eData[i]->HP -= 2;
	eData[i]->explosionCount--;
	eData[i]->damegeTime = 10;
	if (eData[i]->Status == NORMAL)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::DrawBlood()
{
	float size = 0.0f;
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->HP <= 0)
		{
			size += 0.01;
			if (BloodTime[i] > 20) {

				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[0]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[0]);
			}
			else if (BloodTime[i] > 10) {
				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[1]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[1]);
			}
			else if (BloodTime[i] > 0) {
				Object::Instance()->Draw(BloodBack, Vec3(eData[i]->position.x, eData[i]->position.y + 5.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2GraphBack[2]);
				Object::Instance()->Draw(BloodFrount, Vec3(eData[i]->position.x, eData[i]->position.y + 6.0f + size, eData[i]->position.z),
					Vec3(1, 1, 1), Vec3(60.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), Blood2Graph[1]);
			}
		}
	}
}


void Enemy::Delete()
{
	//ìGÇ™è¡Ç¶ÇÈ
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		BloodPosition[i] = eData[i]->position;
		//BloodEffectÇ™è¡Ç¶ÇΩÇÁååç≠Ç™ï\é¶Ç≥ÇÍÇÈ
		if (eData[i]->HP <= 0)
		{
			//0Ç…Ç»ÇÈÇ‹Ç≈É^ÉCÉÄÇÃÉ}ÉCÉiÉX
			if (BloodTime[i] > 0) {
				BloodTime[i]--;
			}

		}
		//0Ç…Ç»Ç¡ÇΩÇÁååç≠ÇÃï\é¶
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
	//ÉmÉbÉNÉoÉbÉNóp
	if (eData[i]->nockbackTime > 0)
	{
		eData[i]->Status = NOCKBACK;
		eData[i]->nockbackTime--;
		if (eData[i]->nockbackTime == 0)
		{
			if (eData[i]->type == BossBigOni)
			{
				eData[i]->Status = eData[i]->memoryStatus;
			}
			else
			{
				eData[i]->Status = MOVE;
			}
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

void Enemy::SetFirstPosition(Vec3 pos, float r, int eNum)
{
	eData[eNum]->position = pos;
	eData[eNum]->oldPosition = eData[eData.size() - 1]->position;
	//ç¿ïWÇçáÇÌÇπÇÈ
	eData[eNum]->eBox.minPosition = XMVectorSet(
		eData[eNum]->position.x - eData[eNum]->r, eData[eNum]->position.y - eData[eNum]->r, eData[eNum]->position.z - eData[eNum]->r, 1);
	eData[eNum]->eBox.maxPosition = XMVectorSet(
		eData[eNum]->position.x + eData[eNum]->r, eData[eNum]->position.y + eData[eNum]->r, eData[eNum]->position.z + eData[eNum]->r, 1);
	eData[eNum]->eSphere.radius = 5.0f;
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
		//à⁄ìÆ
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
		//à⁄ìÆ
		oniBow.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		oniBow.Attack(eData[i], player);
		break;
	}

}

void Enemy::UpdateWolf(int i, Player* player)
{
	switch (eData[i]->Status)
	{
	case NORMAL:
		wolf.SearchPlayer(eData[i], player);
		break;
	case MOVE:
		eData[i]->direction = Direction(i, player);
		//à⁄ìÆ
		wolf.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		wolf.Attack(eData[i], player);
		break;
	}
}

void Enemy::UpdateBigOniBoss(int i, Player* player)
{
	int num = 0;
	switch (eData[i]->Status)
	{
	case NORMAL:
		bigOniBoss.SearchPlayer(eData[i], player);
		break;
	case BOSSATTACK:
		eData[i]->direction = Direction(i, player);
		bigOniBoss.AttackSmall(eData[i], player);
		break;
	case BOSSATTACK2:
		eData[i]->direction = Direction(i, player);
		bigOniBoss.AttackBig(eData[i], player);
		break;
	case SUMMON:
		//éqï™è¢ä´ç≈èâÇæÇØ
		if (bigOniBoss.GetFirstSummonFlag())
		{
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			//ç¿ïWÇçáÇÌÇπÇÈ
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 22 * (-mapSize)), eData[num]->r, num);
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 35 * (-mapSize)), eData[num]->r, num);
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = OniBow;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(60 * mapSize, eData[num]->position.y, 22 * (-mapSize)), eData[num]->r, num);
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = OniBow;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(60 * mapSize, eData[num]->position.y, 35 * (-mapSize)), eData[num]->r, num);
		}
		eData[i]->direction = Direction(i, player);

		int eNum = 0;
		for (int e = 0; e < eData.size(); e++)
		{//HPÇ™Ç†ÇÈìGÇêîÇ¶ÇÈ
			if (eData[e]->HP > 0)
			{
				eNum++;
			}
		}

		bigOniBoss.AttackSummon(eData[i], player, eNum);
		break;
	}
}

void Enemy::UpdateWolfFlock(int i, Player* player)
{
	int num = 0;
	switch (eData[i]->Status)
	{
	case NORMAL:
		wolfFlock.SearchPlayer(eData[i], player);
		break;
	case SUMMON:
		wolfFlock.AttackSummon(eData[i]);
		if (wolfFlock.GetFlockFlag() == true)
		{
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			*eData[num] = wolf.WolfData();
			eData[num]->type = WolfType;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 9 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			*eData[num] = wolf.WolfData();
			eData[num]->type = WolfType;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 31 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			*eData[num] = wolf.WolfData();
			eData[num]->type = WolfType;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(80 * mapSize, eData[num]->position.y, 9 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			*eData[num] = wolf.WolfData();
			eData[num]->type = WolfType;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(80 * mapSize, eData[num]->position.y, 31 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			*eData[num] = wolf.WolfData();
			eData[num]->type = WolfType;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(50 * mapSize, eData[num]->position.y, 6 * (-mapSize)), eData[num]->r, num);
		}
		int damegeNum = 0;
		for (int j = 0; j < eData.size(); j++)
		{
			if (eData[j]->HP <= 0)
			{
				damegeNum++;
			}
		}
		//É_ÉÅÅ[ÉWÇêHÇÁÇ§
		eData[i]->HP -= damegeNum;
		if (eData[i]->HP <= 0)
		{
			eData[i]->HP = 0;
		}
		break;
	}
}


void Enemy::UpdateBoar(int i, Player* player)
{
	switch (eData[i]->Status)
	{
	case NORMAL:
		boar.SearchPlayer(eData[i], player);
		break;
	case MOVE:
		eData[i]->direction = Direction(i, player);
		//à⁄ìÆ
		boar.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		boar.Attack(eData[i], player);
		break;
	}
}

