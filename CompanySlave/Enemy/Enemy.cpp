#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Enemy::Enemy()
{}

Enemy::~Enemy()
{
	//ðú
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
	}
	//XAudio2ðú
	audio->xAudio2.Reset();
	//¹f[^ðú
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	Audio::SoundUnload(&sound3);
	Audio::SoundUnload(&sound4);
	Audio::SoundUnload(&sound5);

	safe_delete(audio);
}

void Enemy::Init()
{
	audio = Audio::Create();
	sound1 = Audio::SoundLoadWave("Resources/Music/SE/club.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/SE/Arrow.wav");
	sound3 = Audio::SoundLoadWave("Resources/Music/SE/wolf.wav");
	sound4 = Audio::SoundLoadWave("Resources/Music/SE/boar.wav");
	sound5 = Audio::SoundLoadWave("Resources/Music/SE/boss.wav");


	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//GHP
	hpGaugeOBJ = Shape::CreateRect(12.0f, 1.8f);
	hpSubOBJ = Shape::CreateRect(11.8f, 1.7f);
	hpOBJ = Shape::CreateRect(15.0f, 2.0f);
	hpGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/EnemyGauge.png");
	hpGaugeGraph = Object::Instance()->LoadTexture(L"Resources/EnemyUI/HP.png");
	hpSub = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");
	//
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
	bossSubHPSPrite = Sprite::Instance()->SpriteCreate(L"Resources/color/yellow.png");

	explosionOBJ = Shape::CreateRect(10, 10);
	explosionGraph = Object::Instance()->LoadTexture(L"Resources/Point.png");
	//SP
	oniType.Init();
	//|
	oniBow.Init();
	//T
	wolf.Init();
	//CmVV
	boar.Init();
	//{XS
	bigOniBoss.Init();
	//TÌQê
	wolfFlock.Init();
	//CmVVQÌ{X
	twinBoar.Init();
}

void Enemy::StageInit(int stageNum)
{
	//ðú
	audioBoss = 5;
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
		//Ìú»
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
	case 8:
		Filepath = (char*)"Resources/map/Enemy_Tile8.csv";
		break;
	case 9:
		Filepath = (char*)"Resources/map/Enemy_Tile9.csv";
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
				//GÌíÞðÝè
				switch (spawnMap[j][i])
				{//ßÚS
				case ONILEFT:
				case ONIUP:
				case ONIDOWN:
				case ONIRIGHT:
					eData[num]->type = Oni;
					break;
					//£S
				case ONIBOWLEFT:
				case ONIBOWRIGHT:
				case ONIBOWUP:
				case ONIBOWDOWN:
					eData[num]->type = OniBow;
					break;
					//T
				case WOLFLEFT:
				case WOLFRIGHT:
				case WOLFUP:
				case WOLFDOWN:
					*eData[num] = wolf.WolfData();
					eData[num]->type = WolfType;
					break;
					//CmVV
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
				case TWINBOAR:
					twinBoar.LoopInit();
					*eData[num] = twinBoar.GetBossData();
					eData[num]->type = BossTwinBoar;
					break;
				default:
					break;
				}
				eData[num]->position = { basePosition.x + i * mapSize,eData[num]->position.y, basePosition.y + j * (-mapSize) };
				eData[num]->oldPosition = eData[eData.size() - 1]->position;
				//ÀWðí¹é
				eData[num]->eBox.minPosition = XMVectorSet(
					eData[num]->position.x - eData[num]->r, eData[num]->position.y - eData[num]->r, eData[num]->position.z - eData[num]->r, 1);
				eData[num]->eBox.maxPosition = XMVectorSet(
					eData[num]->position.x + eData[num]->r, eData[num]->position.y + eData[num]->r, eData[num]->position.z + eData[num]->r, 1);
				eData[num]->eSphere.radius = 5.0f;
				eData[num]->HP = eData[num]->HPMax;
				eData[num]->subHP = eData[num]->HPMax;
				//GÌü«ðÝè
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
		for (int j = 0; j < 7; j++)
		{
			returnDamageFlag[j][i] = false;
		}
	}

}

void Enemy::Update(Player* player)
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		eData[i]->oldPosition = eData[i]->position;
		switch (eData[i]->type)
		{
		case Oni:
			if (eData[i]->HP > 0)
			{
				UpdateOni(i, player);
			}
			else
			{
				eData[i]->attakFlag = false;
			}
			break;
		case OniBow:
			if (eData[i]->HP > 0)
			{
				UpdateBow(i, player);
			}
			break;
		case WolfType:
			if (eData[i]->HP > 0)
			{
				UpdateWolf(i, player);
			}
			break;
		case BoarType:
			if (eData[i]->HP > 0)
			{
				UpdateBoar(i, player);
			}
			break;
		case BossBigOni:
			if (eData[i]->HP > 0)
			{
				UpdateBigOniBoss(i, player);
			}
			break;
		case BossWolfFlock:
			if (eData[i]->HP > 0)
			{
				UpdateWolfFlock(i, player);
			}
			break;
		case BossTwinBoar:
			twinTotalHP = 0;
			for (size_t n = 0; n < eData.size(); n++)
			{//2ÌÌCmVVÌvHPðvZ
				twinTotalHP += eData[n]->HP;
			}
			if (twinTotalHP > 0)
			{
				UpdateTwinBoar(i, player);
			}
			else
			{//¼ûHPª³©Á½ç
				for (size_t n = 0; n < eData.size(); n++)
				{
					eData[n]->HP = 0;
				}
			}
			break;
		}
		if (eData[i]->HP > 0)
		{
			NockBack(i);
			//îÌXV
			oniBow.BowUpdate(eData[i], player);
		}
		FallDown(i);
		//ÀWðí¹é
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
				
				if (eData[i]->type == BossBigOni)
				{
					eData[i]->explosionCount = 8;
				}
				else if (eData[i]->type == BossTwinBoar)
				{
					eData[i]->explosionCount = 5;
				}
				else
				{
					eData[i]->explosionCount = 3;
				}
				eData[i]->explosionFlag = false;
				delayCount[i] = 0;
				eData[i]->explosionDelay = false;
			}
		}

		if (explosionFlag[i])
		{
			explosionGraphCnt[i]++;
		}
		if (eData[i]->explosionCount == 0)
		{
			explosionFlag[i] = false;
		}
		//¹
		if (eData[i]->attakFlag == true) {
			if (eData[i]->HP > 0 && eData[i]->type == Oni && eData[i]->effectCount == 0) {
				audio->SoundSEPlayWave(sound1);//±ñ_
			}
			if (eData[i]->HP > 0 && eData[i]->type == BossBigOni && eData[i]->effectCount == 0) {
				audio->SoundSEPlayWave(sound1);//±ñ_
			}
		}
		if (eData[i]->attakWFlag == true) {
			if (eData[i]->HP > 0 && eData[i]->type == WolfType) {
				audio->SoundSEPlayWave(sound3);//T
			}
			if (eData[i]->HP > 0 && eData[i]->type == BossWolfFlock) {
				audio->SoundSEPlayWave(sound3);//T
			}
		}
		if (eData[i]->attakBFlag == true) {
			if (eData[i]->HP > 0 && eData[i]->type == BoarType) {
				audio->SoundSEPlayWave(sound4);//
			}
			if (eData[i]->HP > 0 && eData[i]->type == BossTwinBoar) {
				audio->SoundSEPlayWave(sound4);//
			}

		}
		if (eData[i]->bowAFlag == true) {
			if (eData[i]->HP > 0 && eData[i]->type == OniBow) {
				audio->SoundSEPlayWave(sound2);//|
			}

		}
		//{Xªñ¾Æ«
		if (eData[i]->type == BossBigOni ||
			eData[i]->type == BossWolfFlock) {
			if (eData[i]->HP <= 0) {
				audioBoss--;
			}
			if (audioBoss == 0) {
				audio->SoundSEPlayWave(sound5);
			}
		}
		twinTotalHP = 0;
		for (size_t n = 0; n < eData.size(); n++)
		{//2ÌÌCmVVÌvHPðvZ
			twinTotalHP += eData[n]->HP;
		}
		//CmVV{X¾¯QÌÈÌÅ
		if (eData[0]->type == BossTwinBoar && twinTotalHP <= 0)
		{
			audioBoss--;
			if (audioBoss == 0) {
				audio->SoundSEPlayWave(sound5);
			}
		}
	}
	//í
	Delete();
	audio->SetVolume(volume);
}

void Enemy::Draw()
{
	for (size_t i = 0; i < eData.size(); i++)
	{

		switch (eData[i]->type)
		{
		case Oni:
			if (eData[i]->HP > 0)
			{
				oniType.Draw(eData[i]);
			}
			else
			{
				oniType.FallDownDraw(eData[i]);
			}
			break;
		case OniBow:
			if (eData[i]->HP > 0)
			{
				oniBow.Draw(eData[i]);
			}
			else
			{
				oniBow.FallDownDraw(eData[i]);
			}
			break;
		case WolfType:
			if (eData[i]->HP > 0)
			{
				wolf.Draw(eData[i]);
			}
			else
			{
				wolf.FallDownDraw(eData[i]);
			}
			break;
		case BoarType:
			if (eData[i]->HP > 0)
			{
				boar.Draw(eData[i]);
			}
			else
			{
				boar.FallDownDraw(eData[i]);
			}
			break;
		case BossBigOni:
			if (eData[i]->HP > 0)
			{
				bigOniBoss.Draw(eData[i]);
			}
			else
			{
				bigOniBoss.FallDownDraw(eData[i]);
			}
			break;
		case BossWolfFlock:
			if (eData[i]->HP > 0)
			{
				wolfFlock.Draw(eData[i]);
			}
			break;
		case BossTwinBoar:
			if (twinTotalHP > 0)
			{
				twinBoar.Draw(eData[i], i);
			}
			else
			{
				twinBoar.FallDownDraw(eData[i]);
			}
			break;
		}
		if (eData[i]->HP > 0 && eData[i]->bossFlag == false)
		{
			if (eData[i]->subHP > eData[i]->HP)
			{
				eData[i]->subHP -= 0.1f;
			}
			//HPQ[W
			float parsent = eData[i]->HP / eData[i]->HPMax;
			float parsent2 = eData[i]->subHP / eData[i]->HPMax;
			if (parsent != 1.0f)
			{
				Object::Instance()->Draw(hpSubOBJ, Vec3(eData[i]->position.x + 0.01 - (1.0f - parsent2) * 6.0, eData[i]->position.y + 4.9f + eData[i]->r, eData[i]->position.z + 5.0f),
					Vec3(parsent2, 0.9f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpSub);
				Object::Instance()->Draw(hpGaugeOBJ, Vec3(eData[i]->position.x - (1.0f - parsent) * 6.0, eData[i]->position.y + 5.0f + eData[i]->r, eData[i]->position.z + 5.0f),
					Vec3(parsent, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGaugeGraph);
				//HPg
				Object::Instance()->Draw(hpOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 5.1f + eData[i]->r, eData[i]->position.z + 5.0f),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), hpGraph);
			}
			//G@j@
			if (eData[i]->explosionCount == 1) {
				Object::Instance()->Draw(explosionOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 2.0f + eData[i]->r, eData[i]->position.z),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(70.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), explosionGraph);
			}
		}
		if (eData[i]->bossFlag == true && eData[i]->HP > 0)
		{
			//G@j@
			if (eData[i]->explosionCount == 1 && eData[i]->type == BossBigOni) {
				Object::Instance()->Draw(explosionOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 20.0f + eData[i]->r, eData[i]->position.z),
					Vec3(2.0f, 2.0f, 2.0f), Vec3(70.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), explosionGraph);
			}
			else if (eData[i]->explosionCount == 1 && eData[i]->type == BossTwinBoar)
			{
				Object::Instance()->Draw(explosionOBJ, Vec3(eData[i]->position.x, eData[i]->position.y + 9.0f + eData[i]->r, eData[i]->position.z),
					Vec3(2.0f, 2.0f, 2.0f), Vec3(70.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), explosionGraph);
			}
		}
	}
}

void Enemy::DrawUI()
{
	int boarNum = 0;
	float boarTotalHP = 0;
	float boarTotalHPMax = 0;
	float boarTotalSubHP = 0;
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->HP > 0 && eData[i]->bossFlag == true && eData[i]->Status != NORMAL && eData[i]->type != BossTwinBoar)
		{
			if (eData[i]->HP < eData[i]->subHP)
			{
				eData[i]->subHP -= 0.1f;
			}
			float ratio = eData[i]->HP / eData[i]->HPMax;
			float ratio2 = eData[i]->subHP / eData[i]->HPMax;
			Sprite::Instance()->Draw(bossSubHPSPrite, Vec2(150.0f, 650.0f), 1000.0f * ratio2, 50.0f);
			Sprite::Instance()->Draw(bossHPSprite, Vec2(150.0f, 650.0f), 1000.0f * ratio, 50.0f);
			Sprite::Instance()->Draw(bossSprite, Vec2(100.0f, 650.0f), 1100.0f, 50.0f);
		}
		//2ÌÌCmVV{Xp
		if (eData[i]->type == BossTwinBoar && eData[i]->bossFlag == true && eData[i]->Status != NORMAL)
		{
			boarNum++;
			boarTotalHP += eData[i]->HP;
			boarTotalHPMax += eData[i]->HPMax;
			boarTotalSubHP += eData[i]->subHP;
			if (boarTotalHP < boarTotalSubHP)
			{
				eData[0]->subHP -= 0.1f;
			}
			if (boarTotalHP > 0 && boarNum == 2)
			{
				float ratio = boarTotalHP / boarTotalHPMax;
				float ratio2 = boarTotalSubHP / boarTotalHPMax;
				Sprite::Instance()->Draw(bossSubHPSPrite, Vec2(150.0f, 650.0f), 1000.0f * ratio2, 50.0f);
				Sprite::Instance()->Draw(bossHPSprite, Vec2(150.0f, 650.0f), 1000.0f * ratio, 50.0f);
				Sprite::Instance()->Draw(bossSprite, Vec2(100.0f, 650.0f), 1100.0f, 50.0f);
			}
		}
	}
}

void Enemy::FirstDraw()
{
	float size = 0.0f;
	for (size_t i = 0; i < eData.size(); i++)
	{//­Ì`æ
		size += 0.01;
		if (BloodFlag[i] == true && eData[i]->type != BossWolfFlock) {
			Object::Instance()->Draw(Blood, Vec3(BloodPosition[i].x, 0.0f - 0.9f + size, BloodPosition[i].z),
				Vec3(1, 1, 1), Vec3(90.0f, 0.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f), BloodGraph);
		}
		switch (eData[i]->type)
		{
		case Oni:
			if (eData[i]->HP > 0)
			{
				if (eData[i]->AttackEffect == true)
				{
					oniType.PreDraw(eData[i], size);
				}
			}
			break;
		case BossBigOni:
			if (eData[i]->HP > 0)
			{
				bigOniBoss.PreDraw(eData[i]);
			}
			break;
		case BossTwinBoar:
			if (twinTotalHP > 0)
			{
				twinBoar.PreDraw(eData[i], i);
			}
			break;
		default:
			break;
		}
	}
}

void Enemy::SetPosition(int i, Vec3 position)
{
	eData[i]->position = position;
}

void Enemy::DamegeNormal(int i, int pAttackDirection)
{
	eData[i]->HP -= 2.8f;//2
	//if (eData[i]->explosionCount == 1) { eData[i]->explosionCount--; }
	eData[i]->damegeTime = 10;
	if (eData[i]->nockPossibleFlag == true)
	{//mbNobNªÂ\Èç
		eData[i]->memoryStatus = eData[i]->Status;
		eData[i]->nockbackTime = EnemySupport::nockBackTimeMax;
		eData[i]->nockDirection = pAttackDirection;
		eData[i]->AttackEffect = false;
	}
	if (eData[i]->Status == NORMAL)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::DamegeThrowSword(int i)
{
	eData[i]->HP -= 1.5f;//2
//	eData[i]->explosionCount--;
	eData[i]->damegeTime = 10;
	if (eData[i]->Status == NORMAL)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::DamegeSword(int j, int i)
{
	if (returnDamageFlag[j][i] == false)
	{
		eData[i]->HP -= 2;
		eData[i]->damegeTime = 10;
		if (eData[i]->Status == NORMAL)
		{
			eData[i]->Status = MOVE;
		}
	}
	returnDamageFlag[j][i] = true;
}

void Enemy::DrawBlood()
{
	float size = 0.0f;
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->HP <= 0 || (twinTotalHP <= 0 && eData[i]->type == BossTwinBoar))
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
	//GªÁ¦é
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		BloodPosition[i] = eData[i]->position;
		//BloodEffectªÁ¦½ç­ª\¦³êé
		if ((eData[i]->HP <= 0 && eData[i]->type != BossTwinBoar) ||
			(twinTotalHP <= 0 && eData[i]->type == BossTwinBoar && eData[i]->Status != NORMAL))
		{
			//0ÉÈéÜÅ^CÌ}CiX
			if (BloodTime[i] > 0) {
				BloodTime[i]--;
			}

		}
		//0ÉÈÁ½ç­Ì\¦
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
	//mbNobNp
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
	//ÀWðí¹é
	eData[eNum]->eBox.minPosition = XMVectorSet(
		eData[eNum]->position.x - eData[eNum]->r, eData[eNum]->position.y - eData[eNum]->r, eData[eNum]->position.z - eData[eNum]->r, 1);
	eData[eNum]->eBox.maxPosition = XMVectorSet(
		eData[eNum]->position.x + eData[eNum]->r, eData[eNum]->position.y + eData[eNum]->r, eData[eNum]->position.z + eData[eNum]->r, 1);
	eData[eNum]->eSphere.radius = 5.0f;
}

void Enemy::FallDown(int eNum)
{
	if (eData[eNum]->HP <= 0)
	{
		float time = eData[eNum]->fallDownTime / fallDownTimeMax;
		if (time <= 1.0f)
		{
			eData[eNum]->fallDownTime += 1.0f;
		}
		//ãëÉ|êé
		switch (eData[eNum]->type)
		{
		case Oni:
		case OniBow:
		case BossBigOni:
			if (eData[eNum]->angle.z > 90.0f)
			{
				eData[eNum]->angle.z = 90.0f;
				//¾ñÅ¢­
				if (eData[eNum]->position.y > -50.0f)
				{
					eData[eNum]->position.y -= 0.08f;
				}
			}
			else
			{
				eData[eNum]->angle = Easing::easeIn(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 90.0f), time);
			}
			break;
		case WolfType:
		case BoarType:
			if (eData[eNum]->angle.x > 90.0f)
			{
				eData[eNum]->angle.x = 90.0f;
				//¾ñÅ¢­
				if (eData[eNum]->position.y > -50.0f)
				{
					eData[eNum]->position.y -= 0.08f;
				}
			}
			else
			{
				eData[eNum]->angle = Easing::easeIn(Vec3(0.0f, 0.0f, 0.0f), Vec3(90.0f, 0.0f, 0.0f), time);
			}
			break;
		case BossWolfFlock:
			break;
		case BossTwinBoar:
			twinTotalHP = 0;
			for (size_t n = 0; n < eData.size(); n++)
			{//2ÌÌCmVVÌvHPðvZ
				twinTotalHP += eData[n]->HP;
			}
			if (twinTotalHP <= 0)
			{
				if (eData[eNum]->angle.x > 90.0f)
				{
					eData[eNum]->angle.x = 90.0f;
					//¾ñÅ¢­
					if (eData[eNum]->position.y > -50.0f)
					{
						eData[eNum]->position.y -= 0.08f;
					}
				}
				else
				{
					eData[eNum]->angle = Easing::easeIn(Vec3(0.0f, 0.0f, 0.0f), Vec3(90.0f, 0.0f, 0.0f), time);
				}
			}
			break;
		}
	}
}

bool Enemy::SetReturnDamageFlag(int j, int i)
{
	return returnDamageFlag[j][i] = false;
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
		//Ú®
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
		//Ú®
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
		//Ú®
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
		//qª¢«Å¾¯
		if (bigOniBoss.GetFirstSummonFlag())
		{
			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			//ÀWðí¹é
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 22 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(20 * mapSize, eData[num]->position.y, 35 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(40 * mapSize, eData[num]->position.y, 22 * (-mapSize)), eData[num]->r, num);

			eData.push_back(new EnemyData);
			num = eData.size() - 1;
			eData[num]->type = Oni;
			eData[num]->Status = MOVE;
			SetFirstPosition(Vec3(40 * mapSize, eData[num]->position.y, 35 * (-mapSize)), eData[num]->r, num);

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
		{//HPª éGð¦é
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
		//_[WðHç¤
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
		//Ú®
		boar.Move(eData[i], player);
		break;
	case ATTACK:
		eData[i]->direction = Direction(i, player);
		boar.Attack(eData[i], player);
		break;
	}
}

void Enemy::UpdateTwinBoar(int i, Player* player)
{
	switch (eData[i]->Status)
	{
	case NORMAL:
		if (twinBoar.GetSerachEnemyFlag() == true)
		{//Ðûª©Â©Á½çà¤Ðûàí¬óÔÉÚs·é
			eData[i]->Status = twinBoar.SearchStatus();
		}
		twinBoar.SearchPlayer(eData[i], player);
		break;
	case BOSSATTACK:
	{
		eData[i]->direction = Direction(i, player);
		twinBoar.AttackShortRush(eData[i], player, i);
		break;
	}
	case BOSSATTACK2:
		eData[i]->direction = Direction(i, player);
		twinBoar.AttackDoubleRush(eData[i], player);
		break;
	case BOSSATTACK3:
		eData[i]->direction = Direction(i, player);
		twinBoar.AttackRush(eData[i], player, i);
		break;
	}
}