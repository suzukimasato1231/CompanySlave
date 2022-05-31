#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
#include<time.h>
#include<random>

Player::Player()
{}

Player::~Player()
{
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	Audio::SoundUnload(&sound3);
	Audio::SoundUnload(&sound4);
	Audio::SoundUnload(&sound5);
	Audio::SoundUnload(&sound6);
	Audio::SoundUnload(&sound7);
	Audio::SoundUnload(&sound8);
	Audio::SoundUnload(&sound9);
	Audio::SoundUnload(&sound10);
	Audio::SoundUnload(&sound11);
	Audio::SoundUnload(&sound12);
	safe_delete(audio);
}

void Player::Init()
{
	//Audioクラス作成
	audio = Audio::Create();
	sound1 = Audio::SoundLoadWave("Resources/Music/SE/katana.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/SE/hiken.wav");
	sound3 = Audio::SoundLoadWave("Resources/Music/SE/return.wav");
	sound4 = Audio::SoundLoadWave("Resources/Music/SE/speed.wav");
	sound5 = Audio::SoundLoadWave("Resources/Music/SE/Error.wav");
	sound6 = Audio::SoundLoadWave("Resources/Music/SE/krkr.wav");
	sound7 = Audio::SoundLoadWave("Resources/Music/SE/HP.wav");
	sound8 = Audio::SoundLoadWave("Resources/Music/SE/hk.wav");
	sound9 = Audio::SoundLoadWave("Resources/Music/SE/Damege.wav");
	sound10 = Audio::SoundLoadWave("Resources/Music/SE/collect.wav");
	sound11 = Audio::SoundLoadWave("Resources/Music/SE/drop.wav");
	sound12 = Audio::SoundLoadWave("Resources/Music/SE/Hikenkatana.wav");

	playerSwordWalkObject[0] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[1] = Object::Instance()->CreateOBJ("playerKari2-2", "playerOBJ/");
	playerSwordWalkObject[2] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[3] = Object::Instance()->CreateOBJ("playerKari2-3", "playerOBJ/");
	playerDamageObject = Object::Instance()->CreateOBJ("playerDead", "playerOBJ/");
	playerLifeEffectObject = Object::Instance()->CreateOBJ("Healing", "playerOBJ/");

	playerAttackObject[0] = Object::Instance()->CreateOBJ("playerAttack1-1", "playerOBJ/");
	playerAttackObject[1] = Object::Instance()->CreateOBJ("playerAttack1-3", "playerOBJ/");
	playerAttackObject[2] = Object::Instance()->CreateOBJ("playerAttack1-4", "playerOBJ/");
	playerAttackObject[3] = Object::Instance()->CreateOBJ("playerAttack2-1", "playerOBJ/");
	playerAttackObject[4] = Object::Instance()->CreateOBJ("playerAttack2-2", "playerOBJ/");
	playerAttackObject[5] = Object::Instance()->CreateOBJ("playerAttack2-3", "playerOBJ/");
	playerAttackObject[6] = Object::Instance()->CreateOBJ("playerAttack3-1", "playerOBJ/");
	playerAttackObject[7] = Object::Instance()->CreateOBJ("playerAttack3-2", "playerOBJ/");
	playerAttackObject[8] = Object::Instance()->CreateOBJ("playerAttack3-3", "playerOBJ/");

	playerLifeObject[0] = Object::Instance()->CreateOBJ("playerKari4-1", "playerOBJ/");
	playerLifeObject[1] = Object::Instance()->CreateOBJ("playerKari4-2", "playerOBJ/");

	swordObject = Object::Instance()->CreateOBJ("sword");
	swordObject2 = Object::Instance()->CreateOBJ("sworda");
	swordObject3 = Object::Instance()->CreateOBJ("swordb");
	swordObjectEx2 = Object::Instance()->CreateOBJ("swordExplosion");
	swordObjectEx3 = Object::Instance()->CreateOBJ("swordExplosion2");
	fotonObject1 = Object::Instance()->CreateOBJ("foton");
	fotonObject2 = Object::Instance()->CreateOBJ("foton2");
	tornadoObject = Object::Instance()->CreateOBJ("tornado");
	swordEffectObject = Object::Instance()->CreateOBJ("Effect");
	returnLineObject = Object::Instance()->CreateOBJ("Box");
	cursorGraph = Object::Instance()->LoadTexture(L"Resources/Effect/Line.png");
	cursorObject = Shape::CreateRect(5, 0.5);

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;

	//プレイヤーHP
	HPGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/HPGauge.png");
	HPGaugeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	HPGaugeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/yellow.png");
	//ソードゲージUI
	swordGraph[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge15.png");
	swordGraph[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge14.png");
	swordGraph[2] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge13.png");
	swordGraph[3] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge12.png");
	swordGraph[4] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge11.png");
	swordGraph[5] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge10.png");
	swordGraph[6] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge9.png");
	swordGraph[7] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge8.png");
	swordGraph[8] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge7.png");
	swordGraph[9] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge6.png");
	swordGraph[10] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge5.png");
	swordGraph[11] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge4.png");
	swordGraph[12] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge3.png");
	swordGraph[13] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge2.png");
	swordGraph[14] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge1.png");

	timeUpEffectGraph[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect1.png");
	timeUpEffectGraph[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect2.png");
	timeUpEffectGraph[2] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect3.png");
	timeUpEffectGraph[3] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect4.png");
	timeUpEffectGraph[4] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect5.png");
	timeUpEffectGraph[5] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect6.png");
	timeUpEffectGraph[6] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect7.png");
	timeUpEffectGraph[7] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect8.png");
	timeUpEffectGraph[8] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/TimeUpEffect9.png");

	swordGargeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	swordGargeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/blue.png");
#if _DEBUG
	attackField = Shape::CreateRect(10.0, 20.0f);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	normalFieldOBJ = Shape::CreateRect(normalLength, normalLengthSub);
	normalFieldOBJ2 = Shape::CreateRect(normalLength, normalLength);
#endif
	AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	//１攻撃目のエフェクト
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect9.png");

	//2攻撃目のエフェクト
	AttackEffectGraph[9] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect1.png");
	AttackEffectGraph[10] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect2.png");
	AttackEffectGraph[11] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect3.png");
	AttackEffectGraph[12] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect4.png");
	AttackEffectGraph[13] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect5.png");
	AttackEffectGraph[14] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect6.png");
	AttackEffectGraph[15] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect7.png");
	AttackEffectGraph[16] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect8.png");
	AttackEffectGraph[17] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect9.png");

	//2攻撃目のエフェクト
	AttackEffectGraph[18] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect1.png");
	AttackEffectGraph[19] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect2.png");
	AttackEffectGraph[20] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect3.png");
	AttackEffectGraph[21] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect4.png");
	AttackEffectGraph[22] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect5.png");
	AttackEffectGraph[23] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect6.png");
	AttackEffectGraph[24] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect7.png");
	AttackEffectGraph[25] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect8.png");
	AttackEffectGraph[26] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect9.png");

	swordUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/0White.png");
	swordUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/1White.png");
	swordUI[2] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/2White.png");
	swordUI[3] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/3White.png");
	swordUI[4] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/4White.png");
	swordUI[5] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/5White.png");
	swordUI[6] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/6White.png");
	swordUI[7] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/7White.png");
	swordUI[8] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/8White.png");
	swordUI[9] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/9White.png");

	swordPre = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/sword.png");

	skillUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill1.png");
	skillUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill2.png");

	swordNot = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordNot.png");
	lifeNot = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordNot.png");

	swordRotationGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordRotion.png");

	portionSprite[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");
	portionSprite[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");

	BlackGraph = Sprite::Instance()->SpriteCreate(L"Resources/BlackOut.png");

	fiveLife = Object::Instance()->CreateOBJ("sphere", "", true);
	gaugeRight = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGaugeRight.png");

	portionRemOBJ = Shape::CreateRect(10.0f, 13.6f);
	portionRemGraph[0] = Object::Instance()->LoadTexture(L"Resources/Button/Y1.png");
	portionRemGraph[1] = Object::Instance()->LoadTexture(L"Resources/Button/Y2.png");
}

void Player::LoopInit()
{
	HP = HPMAX;
	HPSub = HPMAX;
	//ポーション
	portion = portionMax;
	portionRemFlag = false;
	portionRemOneFlag = false;
}

void Player::StageInit(int stageNum)
{
	//仮
	char* Filepath = (char*)"";
	switch (stageNum)
	{
	case 1:
		//ポーション
		portion = portionMax;
		HP = HPMAX;
		HPSub = HP;
		Filepath = (char*)"Resources/map/Enemy_Tile1.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/Enemy_Tile2.csv";
		break;
	case 3:
		Filepath = (char*)"Resources/map/Enemy_Tile3.csv";
		break;
	case 4:
		//ポーション
		portion = portionMax;
		HP = HPMAX;
		HPSub = HP;
		Filepath = (char*)"Resources/map/Enemy_Tile4.csv";
		break;
	case 5:
		Filepath = (char*)"Resources/map/Enemy_Tile5.csv";
		break;
	case 6:
		Filepath = (char*)"Resources/map/Enemy_Tile6.csv";
		break;
	case 7:
		//ポーション
		portion = portionMax;
		HP = HPMAX;
		HPSub = HP;
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
	int spawnMap[MAP_HEIGHT][MAP_WIDTH] = {};
	const float mapSize = 10.0f;
	LoadCSV(spawnMap, Filepath);

	Vec3 memoryPos = {};
	for (size_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (size_t i = 0; i < MAP_WIDTH; i++)
		{
			if (spawnMap[j][i] == 1)
			{
				memoryPos = { i * mapSize, 5, j * (-mapSize) };
				break;
			}
		}
	}
	position = memoryPos;				//座標
	oldPosition = position;				//1つ前の座標
	//座標を合わせる
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
	direction = Right;					//プレイヤーの向き

	//ステージ変わった時の初期化
	walkCount = 0;						//描画用カウント
	walkNo = 0;							//描画するNO
	attackCount = 0;					//描画用カウント
	attackNo = 0;						//描画するNO
	attackMode = false;
	damageTime = 0;						//ダメージ食らったかの見た目用
	moveFlag = false;					//動いているかどうか
	//回避
	avoidanceFlag = false;				//回避中か
	avoidanceTime = 0;					//今回避時間
	avoiCoolTime = 0;					//今回避クールタイム
	//通常攻撃
	normalAttackFlag.fill(false);//通常攻撃可能か

	enemyDamegeTime.fill(60.0f);

	normalAttackTime = 0;							//攻撃と攻撃の間
	normalDirection = 0;							//攻撃の向き
	normalAttackCount = 0;							//通常攻撃の何回目か
	normalGraceTime = 0;							//連続切りまでの猶予

   //剣攻撃
	for (int i = 0; i < 7; i++)
	{
		swordPosition[i] = position;	//座標
		swordAngle[i] = { 0.0f,0.0f,0.0f };		//角度
		swordAngleVec[i] = 0;//飛ばす方向
		isSwordAttack[i] = false;//アタックフラグ
		stingCnt[i] = 0;//刺さるまでの時間
		haveSword[i] = true;//持ってる剣
	}
	shotNo = 0;//どの剣か
	returnFlag = false;//剣が戻る時のフラグ
	nowTime = 0;//剣が戻る時のラープ
	timeRate = 0;//剣が戻る時のラープ
	srand(time(NULL));
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < eNumMax; j++)
		{
			isEnemySting[i][j] = false;
		}
	}
	swordCoolTimeFlag = false;
	swordCoolTime = swordCoolTimeMax;

	eslowTime = 120;

	//ポーション
	portionFlag = false;
	portionTime = 0;

	swordSoundFlag = false;
	swordSoundCount = 0;

	invincivleTime = 0;
	BlackFlag = false;
	collectPos = {};
}

void Player::Update(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	//右スティックに入力があるかどうか
	if (!Input::Instance()->ConRightInput()) {
		homingFlag = true;
	}
	else {
		homingFlag = false;
	}

	Angle();

	ShakeUpdate();

	SwordAngle();
	//無敵時間の更新
	InvincivleUpdate();

	//移動
	Move();
	//プレイヤーの向きを決める
	PDirection();
	//通常攻撃
	NormalAttack(enemy);
	//剣攻撃
	SwordAttack(enemy);
	//回復
	LifePortion();
	//回避
	Avoidance();
	//5秒回復演出更新
	FiveLife();
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > 990)
	{
		position.x = 990;
	}
	if (position.z > 0)
	{
		position.z = 0;
	}
	if (position.z < -390)
	{
		position.z = -390;
	}
	//座標を合わせる
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);


	audio->SetVolume(volume);
}

void Player::PreDraw()
{
	EffectDraw();
}

void Player::Draw(Enemy* enemy)
{
	//プレイヤー
	if (damageTime % 2 == 0)
	{
		if (invincivleTime >= 30) { Object::Instance()->Draw(playerDamageObject, position, scale, angle, color); }
		else if (portionFlag == true)
		{
			Object::Instance()->Draw(playerLifeObject[portionNo], position, scale, angle, color);
			Object::Instance()->Draw(playerLifeEffectObject, { position.x ,position.y + lifeEffectPos ,position.z }, { 0.75f,0.75f ,0.75f }, { 0,0,0 }, color);
			Object::Instance()->Draw(playerLifeEffectObject, { position.x ,position.y + lifeEffectPos - 3 ,position.z }, { 0.75f,0.75f ,0.75f }, { 0,90,0 }, color);
		}
		else if (attackMode == false) { Object::Instance()->Draw(playerSwordWalkObject[walkNo], position, scale, angle, color); }
		else if (attackMode == true)
		{
			Object::Instance()->Draw(playerAttackObject[attackNo], position, scale, angle, color);
#if _DEBUG
			if (direction == Up) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == Down) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == Left) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - normalLength / 2, position.y, position.z), scale, Vec3(90.0f, 0.0f, 0.0f), color, redColor);
			}
			else if (direction == Right) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + normalLength / 2, position.y, position.z), scale, Vec3(90.0f, 0.0f, 0.0f), color, redColor);
			}
			else if (direction == UpLeft) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x - normalLength / 2, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == UpRight) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x + normalLength / 2, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == DownLeft)
			{
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x - normalLength / 2, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == DownRight) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x + normalLength / 2, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
#endif
		}
	}
	if (homingFlag == false)
	{//剣の飛ぶ方向の描画
		Object::Instance()->Draw(cursorObject, { position.x,0,position.z, }, { 10,2,1 }, { 90,Rangle.y,0 }, color, cursorGraph);
	}

	for (int i = 0; i < 7; i++)
	{
		if (returnFlag == true && haveSword[i] == false)
		{
			Object::Instance()->Draw(swordEffectObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 0.5f,0.5f ,2.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i] + 90, swordAngle[i].z }, color);
		}
		else if (ExplosionblinkingFlag[i])
		{
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{

				if (BossFlag[i] == true && isEnemySting[i][j])
				{
					Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y + 13,swordPosition[i].z }, { 2.25f,2.25f ,4.5f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					if (blinkingCount >= 15 && blinkingCount <= 30 && enemy->GetExplosionCount(j) == 1 || blinkingCount >= 45 && blinkingCount <= 60 && enemy->GetExplosionCount(j) == 1)
					{
						Object::Instance()->Draw(swordObjectEx2, { swordPosition[i].x,swordPosition[i].y + 13,swordPosition[i].z }, { 2.55f,2.55f ,5.25 }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					}
					else if (blinkingCount >= 30 && blinkingCount <= 45 && enemy->GetExplosionCount(j) == 1)
					{
						Object::Instance()->Draw(swordObjectEx3, { swordPosition[i].x,swordPosition[i].y + 13,swordPosition[i].z }, { 2.55f,2.55f ,5.25 }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					}
				}
				else if (BossFlag[i] != true && isEnemySting[i][j])
				{
					Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					if (blinkingCount >= 15 && blinkingCount <= 30 && enemy->GetExplosionCount(j) == 1 || blinkingCount >= 45 && blinkingCount <= 60 && enemy->GetExplosionCount(j) == 1)
					{
						Object::Instance()->Draw(swordObjectEx2, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.7f,1.7f ,3.5f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					}
					else if (blinkingCount >= 30 && blinkingCount <= 45 && enemy->GetExplosionCount(j) == 1)
					{
						Object::Instance()->Draw(swordObjectEx3, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.7f,1.7f ,3.5f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
					}
				}
			}
		}
		else if (isSwordAttack[i] == false && blinkingFlag[i])
		{
			if (blinkingCount >= 0 && blinkingCount <= 15)
			{
				Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
			}
			else if (blinkingCount >= 15 && blinkingCount <= 30 || blinkingCount >= 45 && blinkingCount <= 60)
			{
				Object::Instance()->Draw(swordObject2, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
			}
			else if (blinkingCount >= 30 && blinkingCount <= 45)
			{
				Object::Instance()->Draw(swordObject3, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
			}
		}
		else if (returnFlag == true && haveSword[i])
		{

		}
		else if (explosionCount[i] != 0)
		{
			Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
		}
		else if (haveSword[i] == true)
		{
			Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
		}
		else if (isSwordAttack[i] == false)
		{
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (BossFlag[i] == true && enemy->GetExplosionCount(j) != 1)
				{
					Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y + 13,swordPosition[i].z }, { 2.25f,2.25f ,4.5f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
				}
				else if (BossFlag[i] != true && enemy->GetExplosionCount(j) != 1)
				{
					Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
				}
			}
		}
		else
		{
			Object::Instance()->Draw(swordEffectObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 0.5f,0.5f ,2.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i] + 90, swordAngle[i].z }, color);
		}
		//予測線
		if (haveSword[i] || swordCoolTimeFlag == true)
		{

		}
		else if (Input::Instance()->ControllerPush(ButtonLB) && !haveSword[i])
		{
			Object::Instance()->Draw(returnLineObject, { position.x ,position.y ,position.z }, { playerSwordDistance[i] / 2 ,0.1f ,0.1f }, { 0,lineAngle[i] ,0 }, color);
		}
	}
	if (fotonFlag)
	{
		Object::Instance()->Draw(fotonObject2, { havePosition.x,havePosition.y,havePosition.z + 2 }, { 0.5f - fotonLowerScale,5 ,0.5f - fotonLowerScale }, { 0,180 ,0 }, color);
		Object::Instance()->Draw(fotonObject1, { havePosition.x,havePosition.y,havePosition.z + 2 }, { 1 - fotonLowerScale,2 ,1 - fotonLowerScale }, { 0,180 ,0 }, color);
	}
	if (returnFlag)
	{
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle + 120,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle + 240,0 }, color);

		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 285,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 45,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 165,0 }, color);
	}
	if (BlackFlag == true)
	{
		Sprite::Instance()->Draw(BlackGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
	}
	//EffectDraw();
	//５秒回復の球
	for (int i = 0; i < fiveLifeFlag2.size(); i++)
	{
		if (fiveLifeFlag2[i] && startFiveTime[i] <= 0)
		{
			Object::Instance()->Draw(fiveLife, fiveLifePos[i], Vec3(0.5f, 0.5f, 0.5f), Vec3(0.0f, 45.0f, 180.0f),
				Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//移動
void Player::Move()
{
	oldPosition = position;
	//歩きアニメーション用
	if (walkCount >= 5)
	{
		walkCount = 0;
		walkNo++;
	}
	if (walkNo >= 4)
	{
		walkNo = 0;
	}
	//移動
	if ((Input::Instance()->KeybordInputArrow() || Input::Instance()->ConLeftInput())
		&& portionFlag == false && invincivleTime <= 30)
	{
		if ((normalAttackTime <= 1 && AttackToFlag() == true) || (attackMode == false && AttackToFlag() == false))
		{
			//向き変更
			if (Input::Instance()->KeybordPush(DIK_RIGHT)) { angle.y = 0; }
			else if (Input::Instance()->KeybordPush(DIK_LEFT)) { angle.y = 180; }
			else if (Input::Instance()->KeybordPush(DIK_UP)) { angle.y = 270; }
			else if (Input::Instance()->KeybordPush(DIK_DOWN)) { angle.y = 90; }
			if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
				angle.y = 315;
			}
			else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
				angle.y = 225;
			}
			else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN))
			{
				angle.y = 45;
			}
			else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN))
			{
				angle.y = 135;
			}

			if (Input::Instance()->ConLeftInput() && portionFlag == false)
			{
				angle.y = XMConvertToDegrees(atan2(sinRad, cosRad)) - 90;
			}
			walkCount++;//アニメーションのタイマー
			moveFlag = true;
		}
	}
	else {
		walkNo = 0;
		moveFlag = false;
	}
	if (avoidanceTime <= 0 && normalAttackTime <= 0 && portionFlag == false && attackMode == false && invincivleTime <= 30)
	{
		//移動
		//キーボード
		if (Input::Instance()->KeybordPush(DIK_RIGHT))
		{
			position.x += speed.x * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_LEFT))
		{
			position.x -= speed.x * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_UP))
		{
			position.z += speed.z * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_DOWN))
		{
			position.z -= speed.z * slowValue;
		}
		//コントローラー
		if (Input::Instance()->ConLeftInput())
		{
			position.x += speed.x * sinRad * slowValue;
			position.z += speed.z * cosRad * slowValue;
		}
	}
}

void Player::NormalAttack(Enemy* enemy)
{
	//if (attackMode == true) { attackCount++; }//アニメーションのカウント
	if ((Input::Instance()->KeybordTrigger(DIK_D) || Input::Instance()->ControllerDown(ButtonX))
		&& avoidanceTime <= 0 && portionFlag == false && invincivleTime <= 30)
	{
		attackMode = true;

		//連続攻撃を数える
		if (normalAttackCount < 3)
		{
			normalAttackFlag[normalAttackCount] = true;
			normalAttackCount++;
		}
	}

	//攻撃している時間
	if (normalAttackTime > 0)
	{
		normalAttackTime--;
		if (normalAttackFlag[0] == false && normalAttackFlag[1] == false && normalAttackFlag[2] == false && normalAttackCount == 3)
		{//3攻撃目のスピード
			position.x += attackMoveHighSpeed * sinRad * slowValue;
			position.z += attackMoveHighSpeed * cosRad * slowValue;

			attackMoveHighSpeed -= 0.20f;
			if (attackMoveHighSpeed <= 0.1f)
			{
				attackMoveHighSpeed = 0.05f;
			}
			if (normalAttackTime <= 5)
			{
				normalAttackTime = 0;
			}
		}
		else
		{//1,2攻撃目のスピード
			position.x += attackMoveSpeed * sinRad * slowValue;
			position.z += attackMoveSpeed * cosRad * slowValue;
		}

		attackMoveSpeed -= 0.20f;
		if (attackMoveSpeed <= 0.1f)
		{
			attackMoveSpeed = 0.05f;
		}


		//攻撃モーションNo更新
		attackCount++;
		if (attackCount >= 31)
		{
			attackCount = 31;
		}
		if (attackCount % 10 == 0)
		{
			attackNo++;
			if (attackNo >= 9)
			{
				attackNo = 8;
			}
		}
		//攻撃エフェクト更新
		attackEffectCount++;
		if (attackEffectCount >= 26)
		{
			attackEffectCount = 26;
		}
		if (attackEffectCount % 3 == 0)
		{
			attackEfectNo++;
			if (attackEfectNo >= 27)
			{
				attackEfectNo = 27;
			}
		}
	}
	else
	{
		attackEfectFlag = false;
	}

	for (int j = 0; j < 3; j++)
	{//攻撃判定は一番最初
		if (normalAttackFlag[j] == true && normalAttackTime <= 0)
		{
			attackEfectFlag = true;
			audio->SoundSEPlayWave(sound1);
			attackNo = 3 * j;
			attackEfectNo = 9 * j;
			attackCount = 0;
			attackEffectCount = 0;
			normalAttackFlag[j] = false;
			NormalFieldDirection();
			bool swordCoolTimeLifeFlag = false;
			attackMoveSpeed = attackMoveSpeed2;
			attackMoveHighSpeed = attackMoveHighSpeed2;
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if ((enemy->GetHP(i) > 0 && enemy->GetType(i) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(i) == BossTwinBoar)) {
					if (Collision::CheckBox2Box(enemy->GetBox(i), normalAttackBox))
					{
						enemy->DamegeNormal(i, direction);
						enemy->SetDamegeFlag(i, true);
						swordCoolTimeLifeFlag = true;
					}
				}
			}
			//20％の確率で５秒剣投げクールタイム減少
			if (swordCoolTimeLifeFlag == true && swordCoolTimeFlag == true && fiveLifeFlag[0] == false && fiveLifeFlag[1] == false)
			{
				std::random_device rnd;
				std::mt19937 mt(rnd());
				std::uniform_int_distribution<>rand100(0, 99);//0~99の範囲
				int num = rand100(mt);
				if (num < 30)
				{//２０未満ならクールタイム５秒現象
					audio->SoundSEPlayWave(sound8);
					fiveLifeFlag[0] = true;
				}
			}

			normalAttackTime = normalAttackTimeMax;
			normalGraceTime = normalGraceTimeMax;
			break;
		}
	}
	//連続切りまでの猶予
	if (normalGraceTime >= 0 && normalAttackFlag[0] == false && normalAttackFlag[1] == false && normalAttackFlag[2] == false)
	{
		normalGraceTime--;
		//時間終わったら連続切りリセット
		if (normalGraceTime <= 0)
		{
			normalAttackCount = 0;
			attackMode = false;
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				enemy->SetDamegeFlag(i, false);
			}
		}
	}
	if (normalAttackCount == 0) {
		AttackScale = { 3.0f,3.0f,3.0f };
	}
	if (normalAttackCount == 1) {
		AttackScale = { 3.0f,3.0f,3.0f };
	}
	if (normalAttackCount == 2) {
		AttackScale = { 4.0f,4.0f,4.0f };
	}
	if (normalAttackCount == 3) {
		AttackScale = { 5.0f,5.0f,5.0f };
	}

	eslowFlag = true;
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			eslowFlag = false;
		}
	}
	if (eslowFlag == true) {
		if (eslowTime > 0) {
			eslowTime--;
			slowValue = 0.15;
		}
		else if (eslowTime <= 0) {
			slowValue = 1;
			eslowFlag = false;
		}
	}
}

void Player::SwordAttack(Enemy* enemy)
{
	//撃つ
	if (Input::Instance()->ControllerDown(ButtonRB) && haveSword[shotNo] && !returnFlag && portionFlag == false && invincivleTime <= 30)
	{
		audio->SoundSEPlayWave(sound2);
		swordPosition[shotNo] = position;
		swordAngle[shotNo].z = 0;
		swordAngle[shotNo] = Rangle;
		if (homingFlag && enemy->GetEnemySize() > 0)
		{
			swordAngle[shotNo] = Vec3{ 0,(XMConvertToDegrees(atan2(enemy->GetPosition(EnemyNeedNumber(enemy)).x - position.x, enemy->GetPosition(EnemyNeedNumber(enemy)).z - position.z)) + 270),0 };
		}
		isSwordAttack[shotNo] = true;
		haveSword[shotNo] = false;
		holdingFlag[shotNo] = false;
	}

	//剣撃つやつ入れ替え
	if (!haveSword[shotNo])
	{
		shotNo++;
	}
	if (shotNo >= 7)
	{
		shotNo = 0;
	}

	//剣戻ってくるやつ発動
	if (Input::Instance()->ControllerUp(ButtonLB) && portionFlag == false && invincivleTime <= 30)
	{
		if (IsSwordALLHave() == true && swordCoolTimeFlag == false)
		{//剣を全部持っていた場合
			swordCoolTimeFlag = true;
			fotonSaveFlag = false;
			start_time = time(NULL);//クールタイム計測開始
			swordCoolTimePlas = 0;
			slowValue = 0.1;
			slowFlag = true;
			BlackFlag = true;
			for (int i = 0; i < 7; i++)
			{
				reverseAngle[i] = 0;
				reverseValue[i] = 0;
				isSwordAttack[i] = false;
				swordStop[i] = false;
				stingCnt[i] = 0;
				holdingFlag[i] = true;
				swordAngle[i].z = 0;
				ExplosionblinkingFlag[i] = false;
			}
			returnFlag = true;
			tornadoScale = 0.5f;
			audio->SoundSEPlayWave(sound3);
		}
		else
		{//回収できない時の描写の時間設定
			swordNotTime = swordNotTimeMax;
			audio->SoundSEPlayWave(sound5);
		}

	}
	for (int i = 0; i < 7; i++)
	{
		for (int k = 0; k < 25; k++)
		{
			if (position.x + 6.0 < havePosition.x)
			{
				havePosition.x -= 0.1f;
			}
			else if (position.x - 6.0 > havePosition.x)
			{
				havePosition.x += 0.1f;
			}
		}
		for (int k = 0; k < 25; k++)
		{
			if (position.z + 6.0 < havePosition.z)
			{
				havePosition.z -= 0.1f;
			}
			else if (position.z - 6.0 > havePosition.z)
			{
				havePosition.z += 0.1f;
			}
		}

		//リス時の剣の位置変更
		float RespornX = havePosition.x - position.x;      //xの差を求める
		float RespornZ = havePosition.z - position.z;     //zの差を求める
		float RespornXZ = RespornX * RespornX + RespornZ * RespornZ;  //ルートの中を計算
		float Distance = sqrt(RespornXZ);            //ルート計算
		if (Distance > (40.0f))
		{
			havePosition.z = position.z;
			havePosition.x = position.x;
		}


		if (!haveSword[i])
		{
			//予測線の距離
			DistanceX[i] = swordPosition[i].x - position.x;      //xの差を求める
			DistanceZ[i] = swordPosition[i].z - position.z;     //zの差を求める
			DistanceXZ[i] = DistanceX[i] * DistanceX[i] + DistanceZ[i] * DistanceZ[i];  //ルートの中を計算
			playerSwordDistance[i] = sqrt(DistanceXZ[i]);            //ルート計算
			lineAngle[i] = XMConvertToDegrees(atan2(position.x - swordPosition[i].x, position.z - swordPosition[i].z)) + 90;
		}
		else if (haveSword[i])
		{
			ExplosionblinkingFlag[i] = false;
			swordAngle[i].z = -85;
			swordAngle[i].y = 90;
			blinkingFlag[i] = false;
			if (haveSword[0]) { swordPosition[0] = havePosition + Vec3{ +2.25, 10 , -1.0 + 3 } + gravity[0]; swordAngle[0].x = gravityAngle[0] - 20; }
			if (haveSword[1]) { swordPosition[1] = havePosition + Vec3{ -2.25, 10 , -1.0 + 3 } + gravity[1]; swordAngle[1].x = gravityAngle[1] - 20; }
			if (haveSword[2]) { swordPosition[2] = havePosition + Vec3{ +1.5,  10 , +1.0 + 3 } + gravity[2]; swordAngle[2].x = gravityAngle[2] - 20; }
			if (haveSword[3]) { swordPosition[3] = havePosition + Vec3{ -1.5,  10 , +1.0 + 3 } + gravity[3]; swordAngle[3].x = gravityAngle[3] - 20; }
			if (haveSword[4]) { swordPosition[4] = havePosition + Vec3{ +0.75, 10 , -1.0 + 3 } + gravity[4]; swordAngle[4].x = gravityAngle[4] - 20; }
			if (haveSword[5]) { swordPosition[5] = havePosition + Vec3{ -0.75, 10 , -1.0 + 3 } + gravity[5]; swordAngle[5].x = gravityAngle[5] - 20; }
			if (haveSword[6]) { swordPosition[6] = havePosition + Vec3{ 0,     10 , +1.0 + 3 } + gravity[6]; swordAngle[6].x = gravityAngle[6] - 20; }
			gravityCount[i]++;
			if (gravityCount[i] >= 0 && gravityCount[i] <= 60)
			{
				gravity[i].y += 0.1;
				gravityAngle[i] += 0.25;
			}
			else if (gravityCount[i] >= 60 && gravityCount[i] <= 120)
			{
				gravity[i].y += 0.05;
				gravityAngle[i] -= 0.125;
			}
			else if (gravityCount[i] >= 120 && gravityCount[i] <= 180)
			{
				gravity[i].y -= 0.05;
				gravityAngle[i] += 0.125;
			}
			else if (gravityCount[i] >= 180 && gravityCount[i] <= 240)
			{
				gravity[i].y -= 0.1;
				gravityAngle[i] -= 0.25;
			}
			else if (gravityCount[i] >= 240)
			{
				gravityCount[i] = 0;
			}
		}
	}
	//スロウ
	if (slowFlag)
	{
		slowCount++;
		slowValue += 0.01;
	}
	if (slowCount >= 60)
	{
		slowValue = 1;
		slowCount = 0;
		slowFlag = false;
	}
	else if (slowCount > 40)
	{
		slowValue = 0.8;
	}

	//回収後の光
	if (fotonFlag)
	{
		fotonSaveFlag = true;
		fotonLowerScale += 0.025;
		fotonCount++;
	}
	else
	{
		fotonLowerScale = 0;
		fotonCount = 0;
	}
	if (fotonCount >= 20)
	{
		fotonFlag = false;
	}
	//剣戻ってくるやつ処理
	if (returnFlag)
	{
		timeUpEfectFlag = true;
		tornadoScale -= 0.01 * slowValue;
		tornadoAngle += 24 * slowValue;
		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		for (int i = 0; i < 7; i++)
		{
			if (!haveSword[i])
			{
				//自分の方向く
				swordAngle[i].y = XMConvertToDegrees(atan2(havePosition.x - swordPosition[i].x, havePosition.z - swordPosition[i].z)) + 270;
				//刺さったフラグ消す
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					isEnemySting[i][j] = false;
					enemy->SetDamegeFlag(j, false);
				}

				swordPosition[i].x += cos((swordAngle[i].y * 3.14) / -180) * 6 * slowValue;      // x座標を更新
				swordPosition[i].z += sin((swordAngle[i].y * 3.14) / -180) * 6 * slowValue;      // z座標を更新

				//戻ってるときの当たり判定
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					if ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)) {
						if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
						{
							if (enemy->GetExplosionCount(j) == 1)
							{
								explosion[i] = true;
								BossFlag[i] = false;
								ExplosionblinkingFlag[i] = false;
								enemy->SetExplosionFlag(j);
								enemy->SetExplosionCount(j);
								isEnemySting[i][j] = false;
								explosionCount[i] = 0;
								explosion[i] = false;
							}
							enemy->DamegeSword(i, j);
							if (enemyDamegeTime[j] > 0) {
								enemy->SetDamegeFlag(j, true);
								audio->SoundSEPlayWave(sound12);
							}
						}
					}
				}
				//剣がプレイヤーに戻ってくる当たり判定
				if (Collision::CheckBox2Box(haveBox, swordAttackBox[i]) && holdingFlag[i])
				{
					if (fotonSaveFlag == false)
					{
						fotonFlag = true;
					}
					haveSword[i] = true;
				}
			}
		}
		shotNo = 0;
		if (haveSword[0] && haveSword[1] && haveSword[2] && haveSword[3] && haveSword[4] && haveSword[5] && haveSword[6])
		{
			for (int i = 0; i < 7; i++)
			{
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					enemy->SetReturnDamageFlag(i, j);
				}
			}
			tornadoAngle = 0;
			shotNo = 0;
			timeRate = 0;
			nowTime = 0;
			returnFlag = false;
			BlackFlag = false;
		}
	}
	haveBox.minPosition = XMVectorSet(havePosition.x - (r + 5), havePosition.y - r, havePosition.z - (r + 5), 1);
	haveBox.maxPosition = XMVectorSet(havePosition.x + (r + 5), havePosition.y + r, havePosition.z + (r + 5), 1);

	for (int i = 0; i < 7; i++)
	{
		//当たり判定のボックスの位置変
		swordAttackBox[i].maxPosition = XMVectorSet(swordPosition[i].x + 2.0f, swordPosition[i].y, swordPosition[i].z + 2.0f, 1);
		swordAttackBox[i].minPosition = XMVectorSet(swordPosition[i].x - 2.0f, swordPosition[i].y, swordPosition[i].z - 2.0f, 1);

		//剣の飛ぶ方向と向き替え
		if (haveSword[i] && holdingFlag[i])
		{
			reverseAngle[i] = 0;
			reverseValue[i] = 0;
			stingCnt[i] = 0;
			swordAngleVec[i] = (Rangle.y * 3.14) / -180;
			if (homingFlag && enemy->GetEnemySize() > 0)
			{
				swordAngleVec[i] = ((XMConvertToDegrees(atan2(enemy->GetPosition(EnemyNeedNumber(enemy)).x - position.x, enemy->GetPosition(EnemyNeedNumber(enemy)).z - position.z)) + 270) * 3.14) / -180;
			}
			swordStop[i] = false;
			stingCnt[i] = 0;
		}

		//攻撃(飛んでから止まるまで)
		if (isSwordAttack[i] == true)
		{
			//敵との当たり判定
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)))
				{
					isSwordAttack[i] = false;
					isEnemySting[i][j] = true;
					enemy->DamegeThrowSword(j);
					if (enemyDamegeTime[j] > 0) {
						enemy->SetDamegeFlag(j, true);
						audio->SoundSEPlayWave(sound1);

					}
				}
			}
			//角度で進めてる
			for (int s = 0; s < swordSpeed; s++)
			{
				swordPosition[i].x += cos(swordAngleVec[i] + reverseValue[i]) * 1 * slowValue;      // x座標を更新
				swordPosition[i].z += sin(swordAngleVec[i] + reverseValue[i]) * 1 * slowValue;      // z座標を更新
			}
		}
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (enemy->GetDamegeFlag(j) == true) {
				enemyDamegeTime[j] -= 0.1f;
			}
			if (enemy->GetDamegeFlag(j) == false) {
				enemyDamegeTime[j] = 60.0f;
			}
			if (enemyDamegeTime[j] <= 0) {
				enemy->SetDamegeFlag(j, false);
			}
		}
		//当たって取るときの当たり判定たち
		if (isSwordAttack[i] == false && !returnFlag)
		{
			if (Collision::CheckBox2Box(pBox, swordAttackBox[i]) && holdingFlag[i])
			{
				swordSoundFlag = true;
			}
			else {
				swordSoundFlag = false;
			}
			if (haveSword[i]) {
				swordSoundFlag = false;
			}
		}

		if (swordSoundFlag == true) {
			haveSword[i] = true;
			audio->SoundSEPlayWave(sound10);
			swordSoundFlag = false;
		}

		//刺さった敵に剣が追っかける
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{

			if (isEnemySting[i][j])
			{
				if (enemy->GetExplosionCount(j) == 1)
				{
					ExplosionblinkingFlag[i] = true;
				}
				if (enemy->GetType(j) == BossBigOni)
				{
					BossFlag[i] = true;
				}
				if (enemy->GetType(j) != BossBigOni)
				{
					BossFlag[i] = false;
				}
				swordAngle[i].z = 0;
				swordPosition[i] = enemy->GetPosition(j);
			}
			else if (isEnemySting[i][j] == false) {

			}
		}

		if (swordStop[i] == true)
		{

			stingCnt[i]++;//刺さるカウント	
			reverseAngle[i] = 180;
		}

		//止まる
		if (stingCnt[i] >= 2 + rand() % 20)
		{
			swordAngle[i].z = -90;
			isSwordAttack[i] = false;
			swordStop[i] = false;
			stingCnt[i] = 0;
			blinkingFlag[i] = true;
			holdingFlag[i] = true;
		}

		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (enemy->GetHP(j) <= 0 && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
				BossFlag[i] = false;
				ExplosionblinkingFlag[i] = false;
				isEnemySting[i][j] = false;
				explosionAngle[i] = rand() % 360;
				enemy->SetExplosionFlag(j);
			}

			if (enemy->GetExplosionFlag(j) == true && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
				BossFlag[i] = false;
				ExplosionblinkingFlag[i] = false;
				isEnemySting[i][j] = false;
				explosionAngle[i] = rand() % 360;
				enemy->SetExplosionFlag(j);
				enemy->SetExplosionCount(j);
			}
		}

		if (explosion[i])
		{
			explosionCount[i]++;
			swordAngle[i].z = 0;
			swordAngle[i].y = -explosionAngle[i] - 180;
			swordPosition[i].x += cos(XMConvertToRadians(explosionAngle[i])) * 4 * slowValue;      // x座標を更新
			swordPosition[i].z += sin(XMConvertToRadians(explosionAngle[i])) * 4 * slowValue;      // z座標を更新
			holdingFlag[i] = true;

			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (explosionCount[i] >= 3 && isEnemySting[i][j] == false)
				{
					//敵との当たり判定

					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)))
					{
						isSwordAttack[i] = false;
						isEnemySting[i][j] = true;
						enemy->DamegeThrowSword(j);
						if (enemyDamegeTime[j] > 0) {
							enemy->SetDamegeFlag(j, true);
							audio->SoundSEPlayWave(sound12);
						}
					}
					if (enemy->GetDamegeFlag(j) == true) {
						enemyDamegeTime[j] -= 0.1f;
					}
					if (enemy->GetDamegeFlag(j) == false) {
						enemyDamegeTime[j] = 60.0f;
					}
					if (enemyDamegeTime[j] <= 0) {
						enemy->SetDamegeFlag(j, false);
					}
				}
			}
			if (explosionCount[i] >= 6)
			{
				blinkingFlag[i] = true;
				swordAngle[i].z = -90;
				explosionCount[i] = 0;
				explosion[i] = false;
				audio->SoundSEPlayWave(sound11);

			}

		}
	}
	//剣の回収クールタイム
	if (swordCoolTimeFlag == true)
	{
		end_time = time(NULL);
		swordCoolTime = end_time - start_time;//時間計算
		if (swordCoolTime + swordCoolTimePlas >= swordCoolTimeMax)
		{
			swordCoolTimePlas = 0;
			swordCoolTime = swordCoolTimeMax;
			swordCoolTimeFlag = false;
			audio->SoundSEPlayWave(sound6);
		}
	}
	//回収不発時の演出
	if (swordNotTime > 0)
	{
		swordNotTime--;
		if (swordNotTime < 0)
		{
			swordNotTime = 0;
		}
	}
	eslowFlag = true;
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			eslowFlag = false;
		}
	}
	if (eslowFlag == true)
	{
		if (eslowTime > 0)
		{
			eslowTime--;
			slowValue = 0.15;
		}
		else if (eslowTime <= 0)
		{
			slowValue = 1;
			eslowFlag = false;
		}
	}

	//点滅
	if (blinkingFlag[0] || blinkingFlag[1] || blinkingFlag[2] || blinkingFlag[3] || blinkingFlag[4] || blinkingFlag[5] || blinkingFlag[6] ||
		ExplosionblinkingFlag[0] || ExplosionblinkingFlag[1] || ExplosionblinkingFlag[2] || ExplosionblinkingFlag[3] || ExplosionblinkingFlag[4] ||
		ExplosionblinkingFlag[5] || ExplosionblinkingFlag[6])
	{
		blinkingCount++;
		if (blinkingCount > 60)
		{
			blinkingCount = 0;
		}
	}
}
void Player::LifePortion()
{
	//HPが減っていて且つポーションを持っていた場合
	if ((Input::Instance()->KeybordTrigger(DIK_Q) || Input::Instance()->ControllerDown(ButtonY))
		&& portion > 0 && portionFlag == false && avoidanceTime <= 0 && normalAttackTime <= 0 && invincivleTime <= 30)
	{
		if (HP >= HPMAX)
		{//回復不発時
			lifeNotTime = lifeNotTimeMax;
		}
		else
		{//回復実行時
			audio->SoundSEPlayWave(sound7);
			portion--;
			HP += 4;
			if (HP > HPMAX)
			{
				HP = HPMAX;

			}
			HPSub = HP;
			//回復時の膠着時間
			portionTime = portionTimeMax;
			portionFlag = true;
			portionNo = 0;
			if (portionRemFlag == true)
			{
				portionRemOneFlag = true;
				portionRemFlag = false;
			}
		}
	}
	//膠着時間減少
	if (portionFlag == true)
	{
		lifeEffectPos += 0.4;
		portionTime--;
		if (portionTime < 25)
		{
			portionNo = 1;
		}
		if (portionTime <= 0)
		{
			portionFlag = false;
		}
	}
	else if (portionFlag == false)
	{
		lifeEffectPos = -6.0;
	}
	if (lifeNotTime > 0)
	{
		lifeNotTime--;
	}

	//一定のhpまで減ったら回復を催促
	if (HP <= pHPReminder && portion > 0 && portionRemOneFlag == false)
	{
		portionRemFlag = true;
	}
	portionRemTime++;
	if (portionRemTime % 20 == 0)
	{
		portionRemCount++;
		if (portionRemCount == 2)
		{
			portionRemCount = 0;
			portionRemTime = 0;
		}
	}
}
void   Player::Angle()
{
	float rad = 0.0f;
	if (Input::Instance()->KeybordInputArrow())
	{
		if ((normalAttackTime <= 1 && AttackToFlag() == true) || (attackMode == false && AttackToFlag() == false))
		{
			//右上
			if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
				rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
			}//右下
			else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
				rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
			}//左下
			else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
				rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
			}//左上
			else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
				rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
			}//上
			else if (Input::Instance()->KeybordPush(DIK_UP)) {
				rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
			}//右
			else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
				rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
			}//下
			else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
				rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
			}//左
			else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
				rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
			}
			sinRad = sinf(rad);
			cosRad = cosf(rad);
		}
	}
	if (Input::Instance()->ConLeftInput())
	{
		if ((normalAttackTime <= 1 && AttackToFlag() == true) || (attackMode == false && AttackToFlag() == false))
		{
			rad = Input::Instance()->GetLeftAngle();
			sinRad = sinf(-rad);
			cosRad = cosf(rad);
		}
	}
}

void Player::SwordAngle()
{
	//右コントローラ
	if (Input::Instance()->ConRightInput())
	{
		float rad = Input::Instance()->GetRightAngle();
		RsinRad = sinf(-rad);
		RcosRad = cosf(rad);
		Rangle.y = XMConvertToDegrees(atan2(RsinRad, RcosRad)) - 90;
	}
}

void Player::Avoidance()
{
	//回避クールタイム減少

	avoiCoolTime--;
	if (avoiCoolTime <= 0 && avoidanceTime <= 0)
	{
		avoiCoolTime = 0;
		avoidanceFlag = false;
	}

	//回避開始
	if ((Input::Instance()->KeybordTrigger(DIK_F) || Input::Instance()->ControllerDown(ButtonA))
		&& avoidanceFlag == false && invincivleTime <= 30)
	{
		normalAttackCount = 0;
		audio->SoundSEPlayWave(sound4);
		avoidanceFlag = true;
		avoidanceTime = avoidanceTimeMax;
		avoiDirection = direction;
		//攻撃中断
		normalAttackTime = 0;
		attackMode = false;
		attackEfectFlag = false;
		for (int i = 0; i < 3; i++)
		{
			normalAttackFlag[i] = false;
		}
	}
	//回避中
	if (avoidanceTime > 0)
	{
		avoidanceTime--;
		if (avoidanceTime <= 0)
		{
			avoiCoolTime = avoiCoolTimeMax;
		}
		position.x += avoiSpeed * sinRad * slowValue;
		position.z += avoiSpeed * cosRad * slowValue;
	}
}

void Player::PDirection()
{
	if ((normalAttackTime <= 1 && AttackToFlag() == true) || (attackMode == false && AttackToFlag() == false))
	{
		//右上
		if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
			|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
			direction = UpRight;
		}//右下
		else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
			|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
			direction = DownRight;
		}//左下
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
			|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
			direction = DownLeft;
		}//左上
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
			|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
			direction = UpLeft;
		}//上
		else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
			direction = Up;
		}//右
		else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
			direction = Right;
		}//下
		else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
			direction = Down;
		}//左
		else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
			direction = Left;
		}
	}
}

void Player::Damage(int damegeNum)
{
	//ダメージを食らう
	damageTime = damageTimeMax;
	HP -= damegeNum;

	invincivleTime = invincibleTimeMax;
	//ダメージ処理による初期化
	avoidanceTime = 0;
	for (int i = 0; i < 3; i++)
	{
		normalAttackFlag[i] = false;
	}
	normalAttackTime = 0;
	normalAttackCount = 0;
	portionTime = 0;
	audio->SoundSEPlayWave(sound9);
}

void Player::UIDraw()
{
	//HP
	if (HP > 0)
	{
		if (HPSub > HP)
		{
			HPSub -= 0.1f;
		}
		Sprite::Instance()->Draw(HPGaugeSub, Vec2(160.0f, 35.0f), 380.0f * (HPSub / HPMAX), 20.0f);
		Sprite::Instance()->Draw(HPGaugeMain, Vec2(160.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
	}
	Sprite::Instance()->Draw(HPGraph, Vec2(110.0f, 30.0f), 500.0f, 30.0f);

	//ポーション
	if (portion >= 2)
	{
		Sprite::Instance()->Draw(portionSprite[0], Vec2(170.0f, 70.0f), 40.0f, 40.0f);
	}
	if (portion >= 1)
	{
		Sprite::Instance()->Draw(portionSprite[1], Vec2(130.0f, 70.0f), 40.0f, 40.0f);
	}

	//ソードゲージ
	//溜まった時の演出
	if (swordCoolTimeFlag == false)
	{
		swordRotationGraph.rotation -= 2.0f;
		if (swordRotationGraph.rotation < -360)
		{
			swordRotationGraph.rotation = 0.0f;
		}
		Sprite::Instance()->Draw(swordRotationGraph, Vec2(70.0f, 70.0f), 120.0f, 120.0f, Vec2(0.5f, 0.5f));
	}
	//番号
	//クールタイム時間を足す
	int coolTime = (int)swordCoolTime + swordCoolTimePlas;
	if (swordCoolTimeMax <= swordCoolTime + swordCoolTimePlas)
	{
		coolTime = swordCoolTimeMax;
	}
	int number = (int)(swordCoolTimeMax - coolTime) % 10;
	int number2 = (int)(swordCoolTimeMax - coolTime) / 10;
	int swordCoolTimeNum = 15 - coolTime;
	if (swordCoolTimeNum >= 15)
	{
		swordCoolTimeNum = 14;
	}
	//溜まったエフェクトのカウント
	if (swordCoolTimeNum == 0 && timeUpEfectFlag)
	{
		timeUpEfectNo++;
		if (timeUpEfectNo >= 3)
		{
			timeUpEfectNo2++;
			timeUpEfectNo = 0;
			if (timeUpEfectNo2 >= 8)
			{
				timeUpEfectFlag = false;
				timeUpEfectNo2 = 0;
				timeUpEfectNo = 0;
			}
		}
	}
	//ソードゲージ外枠
	Sprite::Instance()->Draw(swordGraph[swordCoolTimeNum], Vec2(-30.0f, -30.0f), 200.0f, 200.0f);
	//溜まったエフェクト
	if (swordCoolTimeNum == 0 && timeUpEfectFlag)
	{
		Sprite::Instance()->Draw(timeUpEffectGraph[timeUpEfectNo2], Vec2(-30.0f, -30.0f), 200.0f, 200.0f);
	}
	//剣の交差画像
	if (swordCoolTimeFlag == false)
	{
		Sprite::Instance()->Draw(swordPre, Vec2(20.0f, 10.0f), 100.0f, 100.0f);
	}
	if (gaugeRightTime % 8 <= 4 && gaugeRightTime > 0)
	{
		Sprite::Instance()->Draw(gaugeRight, Vec2(-30.0f, -30.0f), 200.0f, 200.0f);
	}
	if (coolTime != swordCoolTimeMax)
	{
		//剣クールタイム数字
		Sprite::Instance()->Draw(swordUI[number2], Vec2(25.0f, 36.0f), 45.0f, 45.0f);
		Sprite::Instance()->Draw(swordUI[number], Vec2(70.0f, 36.0f), 45.0f, 45.0f);
	}

	//回収不発時
	if (swordNotTime > 0)
	{
		Sprite::Instance()->Draw(swordNot, Vec2(10.0f + shake.x, 10.0f + shake.y), 120.0f, 120.0f);
	}
	//回復不発時
	if (lifeNotTime > 0)
	{
		if (portion >= 2)
		{
			Sprite::Instance()->Draw(lifeNot, Vec2(170.0f + shake.x, 75.0f + shake.y), 40.0f, 40.0f);
		}
		if (portion >= 1)
		{
			Sprite::Instance()->Draw(lifeNot, Vec2(130.0f + shake.x, 75.0f + shake.y), 40.0f, 40.0f);
		}
	}
}

void Player::TutorialDraw(bool Abutton, bool XButton, bool LBButton, bool RBButton)
{
	//回復催促
	if (Abutton == false && XButton == false && LBButton == false && RBButton == false)
	{
		if (portionRemFlag == true && portionRemOneFlag == false)
		{
			Object::Instance()->Draw(portionRemOBJ, Vec3(position.x, position.y + 20.0f, position.z),
				Vec3{ 1,1,1 }, Vec3{ 30 ,0, 0 }, Vec4{ 1,1,1,1 }, portionRemGraph[portionRemCount]);
		}
	}
}

void Player::EffectDraw()
{
	if (attackEfectFlag == true)
	{
		switch (direction)
		{
		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z + r + AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z - r - AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - r - AttackEffectSize, position.y, position.z),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + r + AttackEffectSize, position.y, position.z),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z + AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z + AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z - AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z - AttackEffectSize),
				AttackScale, AttackAngle, color, AttackEffectGraph[attackEfectNo]);
			break;
		}
	}
}

int Player::EnemyNeedNumber(Enemy* enemy)
{
	//プレイヤーに一番近い敵を求める
	float pNeedLength = 9999.9f;
	int number = 0;
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		if (enemy->GetHP(i) > 0 && enemy->GetType(i) != BossWolfFlock)
		{
			//プレイヤーと敵との距離
			float length = Vec3(position - enemy->GetPosition(i)).length();
			if (pNeedLength > length)
			{
				pNeedLength = length;
				number = i;
			}
		}
	}
	return number;
}

bool Player::IsSwordALLHave()
{
	bool Flag = false;
	for (int i = 0; i < 7; i++)
	{
		if (haveSword[i] == false)
		{
			Flag = true;
			break;
		}
	}
	return Flag;
}

void Player::ShakeUpdate()
{
	if (swordNotTime > 0 || lifeNotTime > 0)
	{
		int powerX = rand() % 30;
		int powerY = rand() % 30;
		shake.x = static_cast<float>(powerX) / 10;
		shake.y = static_cast<float>(powerX) / 10;
	}
}

bool Player::AttackToFlag()
{
	if (normalAttackFlag[0] == true || normalAttackFlag[1] == true || normalAttackFlag[2] == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}



void Player::FiveLife()
{
	if (fiveLifeFlag[0] == true)
	{//初期化
		//fiveLifePos.fill(position);
		fiveTime.fill(0.0f);
		fiveLifeFlag2.fill(true);
		startFiveTime = startFiveTimeMax;
		fiveLifeFlag[0] = false;
		fiveLifeFlag[1] = true;
	}
	//実際の動きの更新
	endFive = position + endFivePos;
	if (position.x < 110.0f)
	{
		endFive.x = 70.0f;
	}
	else if (position.x > 890.0f)
	{
		endFive.x = 850.0f;
	}
	if (position.z > -65.0f)
	{
		endFive.z = -55.0f;
	}
	else if (position.z < -350.0f)
	{
		endFive.z = -340.0f;
	}
	//球がゲージに向かっていく処理
	for (int i = 0; i < fiveLifeFlag2.size(); i++)
	{
		if (fiveLifeFlag2[i])
		{
			fiveLifePos[i] = position;
			if (startFiveTime[i] <= 0)
			{
				fiveTime[i] += 0.5f;
				float rat = fiveTime[i] / fiveTimeMax;
				fiveLifePos[i] = Easing::easeOut(position, endFive, rat);
				if (rat >= 1.0f)
				{
					fiveLifeFlag2[i] = false;
					swordCoolTimePlas += 1;
					gaugeRightTime = gaugeRightTimeMax;
					if (i == fiveLifeFlag2.size() - 1)
					{
						fiveLifeFlag[1] = false;
					}
				}
			}
			startFiveTime[i] -= 1.0f;

		}
	}

	if (gaugeRightTime > 0)
	{
		gaugeRightTime--;
	}
}

Vec3 Player::GetCameraPos()
{
	Vec3 cameraPos = position;
	if (position.x < 110.0f)
	{
		cameraPos.x = 110.0f;

	}
	else if (position.x > 890.0f)
	{
		cameraPos.x = 890.0f;
	}
	if (position.z > -65.0f)
	{
		cameraPos.z = -65.0f;

	}
	else if (position.z < -350.0f)
	{
		cameraPos.z = -350.0f;
	}

	//カメラシェイク
	if (invincivleTime >= 30 && HP > 0) {
		std::random_device randX;
		std::mt19937 rtX(randX());
		std::uniform_int_distribution<>rand2X(0, 2);//0~2の範囲
		int randPosX = rand2X(rtX) - 1;
		std::random_device randY;
		std::mt19937 rtY(randY());
		std::uniform_int_distribution<>rand2Y(0, 2);//0~2の範囲
		int randPosY = rand2Y(rtY) - 1;
		cameraPos = { cameraPos.x - randPosX,cameraPos.y ,cameraPos.z - randPosY };
	}

	if (BlackFlag == true)
	{
		cameraPos += collectPos;
		//y軸
		if (cameraPos.y > +40.0f)
		{
			cameraPos.y = +40.0f;
		}
		else
		{
			collectCount++;
			if (collectCount % 3 == 0)
			{
				collectPos.z += 0.20f;
			}
			collectPos.y += 1.80f;
		}
	}
	else
	{
		collectPos.y -= 1.0f;
		collectPos.z -= 0.15f;
		//y調整
		if (collectPos.y < 0.0f)
		{
			collectPos.y = 0.0f;
		}
		if (collectPos.z < 0.0f)
		{
			collectPos.z = 0.0f;
		}
		cameraPos += collectPos;
	}
	return cameraPos;
}

void Player::NormalFieldDirection()
{
	switch (direction)
	{
	case Up:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLengthSub / 2, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLengthSub / 2, position.y, position.z, 1);
		break;
	case Down:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLengthSub / 2, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLengthSub / 2, position.y, position.z - normalLength, 1);
		break;
	case Left:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z + normalLengthSub / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLengthSub / 2, 1);
		break;
	case Right:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z + normalLengthSub / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalLengthSub / 2, 1);
		break;
	case UpRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case UpLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z, 1);
		break;
	case DownRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalLength, 1);
		break;
	case DownLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLength, 1);
		break;
	}
}

void Player::InvincivleUpdate()
{
	if (invincivleTime > 0)
	{
		invincivleTime--;
		if (invincivleTime < 0)
		{
			invincivleTime = 0;
		}
	}
}
