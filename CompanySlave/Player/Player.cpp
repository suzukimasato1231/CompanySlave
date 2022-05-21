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
	sound5 = Audio::SoundLoadWave("Resources/Music/SE/bbb.wav");
	sound6 = Audio::SoundLoadWave("Resources/Music/SE/return.wav");
	//6のみSE探し中

	playerSwordWalkObject[0] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[1] = Object::Instance()->CreateOBJ("playerKari2-2", "playerOBJ/");
	playerSwordWalkObject[2] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[3] = Object::Instance()->CreateOBJ("playerKari2-3", "playerOBJ/");
	playerDamageObject = Object::Instance()->CreateOBJ("playerDead", "playerOBJ/");

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
	tornadoObject = Object::Instance()->CreateOBJ("tornado");
	swordEffectObject = Object::Instance()->CreateOBJ("Effect");
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

	skillUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill1.png");
	skillUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill2.png");

	swordNot = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordNot.png");
	lifeNot = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordNot.png");

	swordRotationGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordRotion.png");

	portionSprite[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");
	portionSprite[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");

	BlackGraph = Sprite::Instance()->SpriteCreate(L"Resources/BlackOut.png");
}

void Player::LoopInit()
{
	HP = HPMAX;
	HPSub = HPMAX;
	//ポーション
	portion = portionMax;
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

void Player::Draw()
{
	//プレイヤー
	if (damageTime % 2 == 0)
	{
		if (invincivleTime >= 30) { Object::Instance()->Draw(playerDamageObject, position, scale, angle, color); }
		else if (portionFlag == true) { Object::Instance()->Draw(playerLifeObject[portionNo], position, scale, angle, color); }
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
		else if (haveSword[i] == true || isSwordAttack[i] == false)
		{
			Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
		}
		else
		{
			Object::Instance()->Draw(swordEffectObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 0.5f,0.5f ,2.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i] + 90, swordAngle[i].z }, color);
		}
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
	EffectDraw();
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
	if ((Input::Instance()->KeybordInputArrow() || Input::Instance()->ConLeftInput()) && portionFlag == false && invincivleTime <= 30)
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
		{
			position.x += attackMoveHighSpeed * sinRad * slowValue;
			position.z += attackMoveHighSpeed * cosRad * slowValue;
		}
		else
		{
			position.x += attackMoveSpeed * sinRad * slowValue;
			position.z += attackMoveSpeed * cosRad * slowValue;
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
			bool swordCoolTimeFlag = false;
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if ((enemy->GetHP(i) > 0 && enemy->GetType(i) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(i) == BossTwinBoar)) {
					if (Collision::CheckBox2Box(enemy->GetBox(i), normalAttackBox))
					{
						enemy->DamegeNormal(i, direction);
						enemy->SetDamegeFlag(i, true);
						swordCoolTimeFlag = true;
					}
				}
			}
			//20％の確率で５秒剣投げクールタイム減少
			if (swordCoolTimeFlag == true)
			{
				std::random_device rnd;
				std::mt19937 mt(rnd());
				std::uniform_int_distribution<>rand100(0, 99);//0~99の範囲
				int num = rand100(mt);
				if (num < 20)
				{//２０未満ならクールタイム５秒現象
					swordCoolTimePlas += 5;
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

//剣撃つ
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
	if (Input::Instance()->ControllerDown(ButtonLB) && portionFlag == false && invincivleTime <= 30)
	{
		if (IsSwordALLHave() == true && swordCoolTimeFlag == false)
		{//剣を全部持っていた場合
			swordCoolTimeFlag = true;
			start_time = time(NULL);//クールタイム計測開始
			swordCoolTimePlas = 0;
			slowValue = 0.15;
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

		if (haveSword[i])
		{
			swordAngle[i].z = -90;
			swordAngle[i].y = 51.4 * i;

			swordPosition[i] = havePosition;
		}
	}
	//剣戻ってくるやつ処理
	if (returnFlag)
	{

		tornadoScale -= 0.01 * slowValue;
		tornadoAngle += 24 * slowValue;
		if (slowFlag)
		{
			slowCount++;
		}
		if (slowCount > 10)
		{
			slowValue = 1;
			slowCount = 0;
			slowFlag = false;
		}
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

				swordPosition[i].x += cos((swordAngle[i].y * 3.14) / -180) * swordSpeed * slowValue;      // x座標を更新
				swordPosition[i].z += sin((swordAngle[i].y * 3.14) / -180) * swordSpeed * slowValue;      // z座標を更新

				//戻ってるときの当たり判定
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					if ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)) {
						if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
						{
							enemy->DamegeSword(j);
							if (enemyDamegeTime[j] > 0) {
								enemy->SetDamegeFlag(j, true);
							}
						}
					}
				}
				//剣がプレイヤーに戻ってくる当たり判定
				if (Collision::CheckBox2Box(haveBox, swordAttackBox[i]) && holdingFlag[i])
				{
					haveSword[i] = true;
				}
			}
		}
		shotNo = 0;
		if (haveSword[0] && haveSword[1] && haveSword[2] && haveSword[3] && haveSword[4] && haveSword[5] && haveSword[6])
		{
			tornadoAngle = 0;
			shotNo = 0;
			timeRate = 0;
			nowTime = 0;
			slowValue = 1;
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
				haveSword[i] = true;
			}
		}

		//刺さった敵に剣が追っかける
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (isEnemySting[i][j])
			{
				swordPosition[i] = enemy->GetPosition(j);
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
			holdingFlag[i] = true;
		}

		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (enemy->GetHP(j) <= 0 && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
				isEnemySting[i][j] = false;
				enemy->SetExplosionFlag(j);
			}

			if (enemy->GetExplosionFlag(j) == true && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
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
			swordPosition[i].x += cos(XMConvertToRadians(explosionAngle[i])) * 5 * slowValue;      // x座標を更新
			swordPosition[i].z += sin(XMConvertToRadians(explosionAngle[i])) * 5 * slowValue;      // z座標を更新
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
				swordAngle[i].z = -90;
				explosionCount[i] = 0;
				explosion[i] = false;
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
		}
	}
	//膠着時間減少
	if (portionFlag == true)
	{
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

	if (lifeNotTime > 0)
	{
		lifeNotTime--;
	}

}
void   Player::Angle()
{
	float rad = 0.0f;
	if (Input::Instance()->KeybordInputArrow())
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
	if (Input::Instance()->ConLeftInput())
	{
		rad = Input::Instance()->GetLeftAngle();
		sinRad = sinf(-rad);
		cosRad = cosf(rad);
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
	if (avoiCoolTime > 0)
	{
		avoiCoolTime--;
		if (avoiCoolTime <= 0)
		{
			avoidanceFlag = false;
		}
	}
	//回避開始
	if ((Input::Instance()->KeybordTrigger(DIK_F) || Input::Instance()->ControllerDown(ButtonA))
		&& avoidanceFlag == false && normalAttackTime <= 0 && invincivleTime <= 30)
	{
		audio->SoundSEPlayWave(sound4);
		avoidanceFlag = true;
		avoidanceTime = avoidanceTimeMax;
		avoiDirection = direction;
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
		Sprite::Instance()->Draw(HPGaugeSub, Vec2(130.0f, 35.0f), 380.0f * (HPSub / HPMAX), 20.0f);
		Sprite::Instance()->Draw(HPGaugeMain, Vec2(130.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
	}
	Sprite::Instance()->Draw(HPGraph, Vec2(80.0f, 30.0f), 500.0f, 30.0f);

	//ポーション
	if (portion >= 2)
	{
		Sprite::Instance()->Draw(portionSprite[0], Vec2(160.0f, 70.0f), 40.0f, 40.0f);
	}
	if (portion >= 1)
	{
		Sprite::Instance()->Draw(portionSprite[1], Vec2(120.0f, 70.0f), 40.0f, 40.0f);
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
		Sprite::Instance()->Draw(swordRotationGraph, Vec2(65.0f, 65.0f), 120.0f, 120.0f, Vec2(0.5f, 0.5f));
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
	Sprite::Instance()->Draw(swordGraph[swordCoolTimeNum], Vec2(-10.0f, -10.0f), 140.0f, 140.0f);
	if (coolTime != swordCoolTimeMax)
	{
		//剣クールタイム数字
		Sprite::Instance()->Draw(swordUI[number2], Vec2(20.0f, 36.0f), 45.0f, 45.0f);
		Sprite::Instance()->Draw(swordUI[number], Vec2(65.0f, 36.0f), 45.0f, 45.0f);
	}

	//回収不発時
	if (swordNotTime > 0)
	{
		Sprite::Instance()->Draw(swordNot, Vec2(0.0f + shake.x, 0.0f + shake.y), 120.0f, 120.0f);
	}
	//回復不発時
	if (lifeNotTime > 0)
	{
		if (portion >= 2)
		{
			Sprite::Instance()->Draw(lifeNot, Vec2(160.0f+shake.x, 75.0f + shake.y), 40.0f, 40.0f);
		}
		if (portion >= 1)
		{
			Sprite::Instance()->Draw(lifeNot, Vec2(120.0f + shake.x, 75.0f + shake.y), 40.0f, 40.0f);
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
		shake.x = static_cast<float>(powerX)/10;
		shake.y = static_cast<float>(powerX) / 10;
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
	if (BlackFlag == true)
	{
		collectCount -= 0.25f;
		cameraPos.y += collectCount;
		if (cameraPos.y < -3.0f)
		{
			cameraPos.y = -3.0f;
			collectCount = -8.0f;
		}
	}
	else
	{
		collectCount += 0.1f;
		if (collectCount > 0.0f)
		{
			collectCount = 0.0f;
		}
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
