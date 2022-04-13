#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
#include<time.h>

Player::Player()
{}

Player::~Player()
{}

void Player::Init()
{
	playerSwordWalkObject[0] = Object::Instance()->CreateOBJ("playerKari2-1");
	playerSwordWalkObject[1] = Object::Instance()->CreateOBJ("playerKari2-2");
	playerSwordWalkObject[2] = Object::Instance()->CreateOBJ("playerKari2-1");
	playerSwordWalkObject[3] = Object::Instance()->CreateOBJ("playerKari2-3");

	playerAttackObject[0] = Object::Instance()->CreateOBJ("playerAttack1-1");
	playerAttackObject[1] = Object::Instance()->CreateOBJ("playerAttack1-2");
	playerAttackObject[2] = Object::Instance()->CreateOBJ("playerAttack1-3");
	playerAttackObject[3] = Object::Instance()->CreateOBJ("playerAttack2-1");
	playerAttackObject[4] = Object::Instance()->CreateOBJ("playerAttack2-2");
	playerAttackObject[5] = Object::Instance()->CreateOBJ("playerAttack2-3");
	playerAttackObject[6] = Object::Instance()->CreateOBJ("playerAttack3-1");
	playerAttackObject[7] = Object::Instance()->CreateOBJ("playerAttack3-2");
	playerAttackObject[8] = Object::Instance()->CreateOBJ("playerAttack3-3");

	swordObject = Object::Instance()->CreateOBJ("sword");
	swordEffectObject = Object::Instance()->CreateOBJ("Effect");
	cursorGraph = Object::Instance()->LoadTexture(L"Resources/Effect/Line.png");
	cursorObject = Shape::CreateRect(5, 0.5);

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;


	yellowColor = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	//プレイヤーHP
	HPGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/HPGauge.png");
	HPGaugeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	HPGaugeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/yellow.png");
	//ソードゲージUI
	swordGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge.png");
	swordGargeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	swordGargeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/blue.png");
#if _DEBUG
	attackField = Shape::CreateRect(10.0f, 20.0f);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	normalFieldOBJ = Shape::CreateRect(normalLength, normalLength);
	//normalGraph = Object::Instance()->LoadTexture(L"Resources/kengeki.png");

#endif
	AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect9.png");
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			isEnemySting[i][j] = false;
		}
	}
}

void Player::StageInit(int stageNum)
{
	//ステージごと
	switch (stageNum)
	{
	case 1:
		position = { 100.0f,0.0f,-120.0f };	//座標
		oldPosition = position;				//1つ前の座標
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
		pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
		direction = Right;					//プレイヤーの向き
		break;
	case 2:
		position = { 100.0f,0.0f,-120.0f };	//座標
		oldPosition = position;				//1つ前の座標
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
		pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
		direction = Right;
		break;
	}
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
	for (int i = 0; i < 3; i++)
	{
		normalAttackFlag[i] = false;				//通常攻撃可能か
	}
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

	//エフェクト関係
	AttackEffect = false;
	effectTime = 10;
	effectCount = 0;

}

void Player::Update(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	Angle();

	SwordAngle();
	//移動
	Move();
	//プレイヤーの向きを決める
	PDirection();
	//通常攻撃
	NormalAttack(enemy);
	//剣攻撃
	SwordAttack(enemy);
	//回避
	Avoidance();

	if (damageTime > 0)
	{
		damageTime--;
	}

	//座標を合わせる
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
}

void Player::Draw()
{
#if _DEBUG
	DebugDraw();
#endif
	//プレイヤー
	if (damageTime % 2 == 0)
	{
		if (attackMode == false) { Object::Instance()->Draw(playerSwordWalkObject[walkNo], position, scale, angle, color); }
		if (attackMode == true) { Object::Instance()->Draw(playerAttackObject[attackNo], position, scale, angle, color); }
	}
	Object::Instance()->Draw(cursorObject, { position.x,-2,position.z, }, { 10,2,1 }, { 90,Rangle.y,0 }, color, cursorGraph);
	for (int i = 0; i < 7; i++)
	{
		if (returnFlag == true)
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
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
		|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
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

		if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
			|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
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
	if (avoidanceTime <= 0 && normalAttackTime <= 0)
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
		if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft) ||
			Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
		{
			position.x += speed.x * sinRad * slowValue;
			position.z += speed.z * cosRad * slowValue;
		}
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

void Player::NormalAttack(Enemy* enemy)
{//歩きアニメーション用
	if (attackCount >= 10 && attackMode == true)
	{
		attackCount = 0;
		attackNo++;
		if (attackNo >= 9)
		{
			attackNo = 8;
		}
	}
	if (normalAttackFlag[2] && attackNo >= 9) { attackNo = 8; }
	if (normalAttackFlag[1] && attackNo >= 6) { attackNo = 5; }
	if (normalAttackFlag[0] && attackNo >= 3) { attackNo = 2; }


	if (attackMode == true) { attackCount++; }//アニメーションのカウント
	if ((Input::Instance()->KeybordTrigger(DIK_D) || Input::Instance()->ControllerDown(ButtonX)) && avoidanceTime <= 0)
	{
		attackMode = true;
		//連続攻撃を数える
		if (normalAttackCount < 3)
		{
			normalAttackFlag[normalAttackCount] = true;
			normalAttackCount++;
		}
	}
	//描画するNO
	if (normalAttackTime == normalAttackTimeMax)
	{
		if (normalAttackFlag[0]) { attackNo = 0; }
		else if (normalAttackFlag[1]) { attackNo = 3; }
		else if (normalAttackFlag[2]) { attackNo = 6; }
		attackCount = 0;//カウントリセット
	}

	//攻撃している時間
	if (normalAttackTime > 0)
	{
		normalAttackTime--;
		position.x += attackMoveSpeed * sinRad * slowValue;
		position.z += attackMoveSpeed * cosRad * slowValue;
	}

	for (int j = 0; j < 3; j++)
	{//攻撃判定は一番最初
		if (normalAttackFlag[j] == true && normalAttackTime <= 0)
		{
			normalAttackFlag[j] = false;
			NormalFieldDirection();
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if (enemy->GetHP(i) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(i), normalAttackBox))
					{
						enemy->DamegeNormal(i, direction);

						enemy->SetDamegeFlag(i, true);

					}
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
			attackNo = 0;
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
	if (normalAttackTime == normalAttackTimeMax / 2) { AttackEffect = true; }

	if (AttackEffect == true) {

		if (effectCount < 8) {
			effectCount++;
		}
		else if (effectCount == 8) {
			effectCount = 0;
			AttackEffect = false;
		}
	}
}

//剣撃つ
void Player::SwordAttack(Enemy* enemy)
{
	if (Input::Instance()->KeybordPush(DIK_U))
	{

	}
	//撃つ
	if (Input::Instance()->ControllerDown(ButtonRB) && haveSword[shotNo] && !returnFlag)
	{
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
	if (Input::Instance()->ControllerDown(ButtonLB))
	{
		slowValue = 0.25;
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
	}

	//剣戻ってくるやつ処理
	if (returnFlag)
	{

		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		for (int i = 0; i < 7; i++)
		{
			//自分の方向く
			swordAngle[i].y = XMConvertToDegrees(atan2(position.x - swordPosition[i].x, position.z - swordPosition[i].z)) + 270;
			//刺さったフラグ消す
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				isEnemySting[i][j] = false;
				enemy->SetDamegeFlag(j, false);
			}

			//ラープ
			swordPosition[i] = Easing::easeIn(swordPosition[i], position, timeRate);

			//戻ってるときの当たり判定
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (enemy->GetHP(j) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
					{
						enemy->DamegeSword(j);
						enemy->SetDamegeFlag(j, true);
					}
				}
			}
		}
		shotNo = 0;
		if (nowTime >= 5)
		{
			timeRate = 0;
			nowTime = 0;
			slowValue = 1;
			returnFlag = false;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		//当たり判定のボックスの位置変
		swordAttackBox[i].maxPosition = XMVectorSet(swordPosition[i].x + 2, swordPosition[i].y, swordPosition[i].z + 2, 1);
		swordAttackBox[i].minPosition = XMVectorSet(swordPosition[i].x - 2, swordPosition[i].y, swordPosition[i].z - 2, 1);

		//剣の飛ぶ方向と向き替え
		if (haveSword[i] && holdingFlag[i])
		{
			reverseAngle[i] = 0;
			reverseValue[i] = 0;
			stingCnt[i] = 0;
			swordAngle[i] = Rangle;
			swordPosition[i] = position;
			swordAngleVec[i] = (Rangle.y * 3.14) / -180;
			swordStop[i] = false;
			stingCnt[i] = 0;
		}

		//攻撃(飛んでから止まるまで)
		if (isSwordAttack[i] == true)
		{
			//敵との当たり判定
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (enemy->GetHP(j) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && enemy->GetHP(j) > 0)
					{
						isSwordAttack[i] = false;
						isEnemySting[i][j] = true;
						enemy->DamegeThrowSword(j);
						if (enemyDamegeTime[j] > 0) {
							enemy->SetDamegeFlag(j, true);
						}

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
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (isSwordAttack[i] == false)
			{
				if (Collision::CheckBox2Box(pBox, swordAttackBox[i]) && holdingFlag[i])
				{
					haveSword[i] = true;
				}
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
			//reverseValue[i] = 180;
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
			}
		}

		if (explosion[i])
		{
			explosionCount[i]++;
			swordPosition[i].x += cos(swordAngleVec[i] + reverseValue[i]) * 2 * slowValue;      // x座標を更新
			swordPosition[i].z += sin(swordAngleVec[i] + reverseValue[i]) * 2 * slowValue;      // z座標を更新
			holdingFlag[i] = true;
			if (explosionCount[i] >= 20)
			{
				swordAngle[i].z = -90;
				explosion[i] = false;
			}
		}
	}
}



void   Player::Angle()
{
	float rad = 0.0f;
	//右上
	if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//右下
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//左下
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//左上
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//上
	else if (Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//右
	else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
		rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//下
	else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//左
	else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
		rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}

	if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft) ||
		Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
	{
		rad = Input::Instance()->GetLeftAngle();
		sinRad = sinf(-rad);
		cosRad = cosf(rad);
	}
}

void Player::SwordAngle()
{
	//右コントローラ
	if (Input::Instance()->ControllerPush(RButtonRight) || Input::Instance()->ControllerPush(RButtonLeft) ||
		Input::Instance()->ControllerPush(RButtonUp) || Input::Instance()->ControllerPush(RButtonDown))
	{
		float rad = 0.0f;
		rad = Input::Instance()->GetRightAngle();
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
		&& avoidanceFlag == false && normalAttackTime <= 0)
	{
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

void Player::Damage()
{
	//ダメージを食らう
	damageTime = damageTimeMax;
	HP--;
}

void Player::UIDraw()
{
	//HP
	if (HP > 0)
	{
		Sprite::Instance()->Draw(HPGaugeSub, Vec2(50.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
		Sprite::Instance()->Draw(HPGaugeMain, Vec2(50.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
	}
	Sprite::Instance()->Draw(HPGraph, Vec2(0.0f, 30.0f), 500.0f, 30.0f);

	//ソードゲージ
	//Sprite::Instance()->Draw(swordGargeSub, Vec2(), 100.0f, 100.0f);
	//Sprite::Instance()->Draw(swordGargeMain, Vec2(), 100.0f, 100.0f);
	Sprite::Instance()->Draw(swordGraph, Vec2(), 100.0f, 100.0f);

}

void Player::DebugDraw()
{
#if _DEBUG

#endif
}

void Player::EffectDraw()
{
	if (AttackEffect == true)
	{
		switch (direction)
		{

		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z + r + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z - r - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - r - AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + r + AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		}
	}
	//AttackEffect = true;
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
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalLength, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case DownRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z - normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case DownLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	}
}