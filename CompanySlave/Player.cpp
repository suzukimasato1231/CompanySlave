#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
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

	playerAttackObject[0] = Object::Instance()->CreateOBJ("playerKari3-0");
	playerAttackObject[1] = Object::Instance()->CreateOBJ("playerKari3-1");
	playerAttackObject[2] = Object::Instance()->CreateOBJ("playerKari3-1");
	playerAttackObject[3] = Object::Instance()->CreateOBJ("playerKari3-1");

	swordObject = Object::Instance()->CreateOBJ("sword");
	cursorGraph = Object::Instance()->LoadTexture(L"Resources/Effect/Line.png");
	cursorObject = Shape::CreateRect(5, 0.5);

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;

	//コンボUI
	/*comboPolygon = Shape::CreateRect(20.0f, 10.0f);
	comboGraph = Object::Instance()->LoadTexture(L"Resources/ComboUI/ComboGauge.png");

	black = Shape::CreateRect(16.0f, 2.0f);*/
	yellowColor = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	//コンボ数字
	//comboNumberObj = Shape::CreateRect(5.0f, 7.0f);

	/*comboNumberGraph[0] = Object::Instance()->LoadTexture(L"Resources/ComboUI/0.png");
	comboNumberGraph[1] = Object::Instance()->LoadTexture(L"Resources/ComboUI/1.png");
	comboNumberGraph[2] = Object::Instance()->LoadTexture(L"Resources/ComboUI/2.png");
	comboNumberGraph[3] = Object::Instance()->LoadTexture(L"Resources/ComboUI/3.png");
	comboNumberGraph[4] = Object::Instance()->LoadTexture(L"Resources/ComboUI/4.png");
	comboNumberGraph[5] = Object::Instance()->LoadTexture(L"Resources/ComboUI/5.png");
	comboNumberGraph[6] = Object::Instance()->LoadTexture(L"Resources/ComboUI/6.png");
	comboNumberGraph[7] = Object::Instance()->LoadTexture(L"Resources/ComboUI/7.png");
	comboNumberGraph[8] = Object::Instance()->LoadTexture(L"Resources/ComboUI/8.png");
	comboNumberGraph[9] = Object::Instance()->LoadTexture(L"Resources/ComboUI/9.png");*/
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
	normalGraph = Object::Instance()->LoadTexture(L"Resources/kengeki.png");

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

void Player::Update(Enemy *enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	Angle();
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
	EffectDraw();
	//プレイヤー
	if (damageTime % 2 == 0)
	{
		if (attackMode == false) { Object::Instance()->Draw(playerSwordWalkObject[walkNo], position, scale, angle, color); }
		if (attackMode == true) { Object::Instance()->Draw(playerAttackObject[attackNo], position, scale, angle, color); }
	}
	for (int i = 0; i < 7; i++)
	{
		Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,1.5f }, swordAngle[i], color);
	}
	Object::Instance()->Draw(cursorObject, { position.x,0,position.z, }, { 10,10,10 }, { 90,angle.y,0 }, color, cursorGraph);

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
		if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) { angle.y = 0; }
		else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) { angle.y = 180; }
		else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) { angle.y = 270; }
		else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) { angle.y = 90; }
		if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) ||
			(Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
			angle.y = 315;
		}
		else if ((Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP))
			|| (Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp))) {
			angle.y = 225;
		}
		else if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN))
			|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)))
		{
			angle.y = 45;
		}
		else if ((Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN))
			|| (Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)))
		{
			angle.y = 135;
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
			position.x += speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_LEFT))
		{
			position.x -= speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_UP))
		{
			position.z += speed.z;
		}
		if (Input::Instance()->KeybordPush(DIK_DOWN))
		{
			position.z -= speed.z;
		}
		//コントローラー
		if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft) ||
			Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
		{

			position.x += speed.x * sinRad;
			position.z += speed.z * cosRad;
		}
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

void Player::NormalAttack(Enemy *enemy)
{//歩きアニメーション用
	if (attackCount >= 10)
	{
		attackCount = 0;
		attackNo++;
	}
	if (attackNo >= 4)
	{
		attackNo = 0;
	}
	if (attackMode == true) { attackCount++; }//アニメーションのカウント
	if (Input::Instance()->KeybordTrigger(DIK_D) || Input::Instance()->ControllerDown(ButtonA) && avoidanceTime <= 0)
	{
		attackCount = 0;//カウントリセット
		attackNo = 0;//ナンバーをリセット
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
		position.x += attackMoveSpeed * sinRad;
		position.z += attackMoveSpeed * cosRad;
	}

	for (int j = 0; j < 3; j++)
	{//攻撃判定は一番最初
		if (normalAttackFlag[j] == true && normalAttackTime <= 0)
		{
			normalAttackFlag[j] = false;
			NormalFieldDirection();
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if (Collision::CheckSphere2Box(enemy->GetSphere(i), normalAttackBox))
				{
					enemy->DamegeNormal(i);
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
	if (normalAttackTime == normalAttackTimeMax) { AttackEffect = true; }

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

void Player::SwordAttack(Enemy *enemy)
{
	//撃つ
	if (Input::Instance()->KeybordTrigger(DIK_V) && haveSword[shotNo] && !returnFlag)
	{
		isSwordAttack[shotNo] = true;
		haveSword[shotNo] = false;
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
	if (Input::Instance()->KeybordPush(DIK_B))
	{
		returnFlag = true;
	}

	//剣戻ってくるやつ処理
	if (returnFlag)
	{
		
		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		for (int i = 0; i < 7; i++)
		{
			//刺さったフラグ消す
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				isEnemySting[i][j] = false;
			}

			//ラープ
			swordPosition[i] = Easing::easeIn(swordPosition[i], position, timeRate);
			
			//戻ってるときの当たり判定
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
				{
					
				}
			}
		}
		shotNo = 0;
		if (nowTime >= 5)
		{
			timeRate = 0;
			nowTime = 0;
			returnFlag = false;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		//当たり判定のボックスの位置変
		swordAttackBox[i].maxPosition = XMVectorSet(swordPosition[i].x + 3, swordPosition[i].y, swordPosition[i].z + 3, 1);
		swordAttackBox[i].minPosition = XMVectorSet(swordPosition[i].x - 3, swordPosition[i].y, swordPosition[i].z - 3, 1);

		//剣の飛ぶ方向と向き替え
		if (haveSword[i])
		{
			stingCnt[i] = 0;
			swordAngle[i] = angle;
			swordPosition[i] = position;
			swordAngleVec[i] = (angle.y * 3.14) / -180;
		}

		//攻撃(飛んでから止まるまで)
		if (isSwordAttack[i] == true)
		{
			stingCnt[i]++;//刺さるカウント
			
			//敵との当たり判定
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
				{
					isSwordAttack[i] = false;
					isEnemySting[i][j] = true;
				}
			}

			//角度で進めてる
			for (int s = 0; s < 3; s++)
			{
				swordPosition[i].x += cos(swordAngleVec[i]) * 1;      // x座標を更新
				swordPosition[i].z += sin(swordAngleVec[i]) * 1;      // z座標を更新

			}

			//止まる
			if (stingCnt[i] >= 60)
			{
				isSwordAttack[i] = false;
			}
		}

		//当たって取るときの当たり判定たち
		else if (isSwordAttack[i] == false)
		{
			if (Collision::CheckBox2Box(pBox, swordAttackBox[i]))
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
	if ((Input::Instance()->KeybordTrigger(DIK_F) || Input::Instance()->ControllerDown(ButtonB))
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
		position.x += avoiSpeed * sinRad;
		position.z += avoiSpeed * cosRad;
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
	Vec3 attackPosition{};
	Vec3 attackAngle{};

	//通常攻撃エフェクト

	if (normalAttackTime == normalAttackTimeMax)
	{
		switch (direction)
		{
		case Up:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z + r + normalLength), scale, UIAngle, color, normalGraph);

			break;
		case Down:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z - r - normalLength), scale, UIAngle, color, normalGraph);

			break;
		case Left:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - r - normalLength, position.y, position.z), scale, UIAngle, color, normalGraph);

			break;
		case Right:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + r + normalLength, position.y, position.z), scale, UIAngle, color, normalGraph);

			break;
		case UpRight:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + normalLength, position.y, position.z + normalLength), scale, UIAngle, color, normalGraph);

			break;
		case UpLeft:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - normalLength, position.y, position.z + normalLength), scale, UIAngle, color, normalGraph);

			break;
		case DownRight:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + normalLength, position.y, position.z - normalLength), scale, UIAngle, color, normalGraph);

			break;
		case DownLeft:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - normalLength, position.y, position.z - normalLength), scale, UIAngle, color, normalGraph);

			break;
		}
		//AttackEffect = true;
	}

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
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength / 2, position.y, position.z + r, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength / 2, position.y, position.z + r + normalLength, 1);
		break;
	case Down:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength / 2, position.y, position.z - r, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength / 2, position.y, position.z - r - normalLength, 1);
		break;
	case Left:
		normalAttackBox.maxPosition = XMVectorSet(position.x - r, position.y, position.z + normalLength / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - r - normalLength, position.y, position.z - normalLength / 2, 1);
		break;
	case Right:
		normalAttackBox.maxPosition = XMVectorSet(position.x + r + normalLength, position.y, position.z + normalLength / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x + r, position.y, position.z - normalLength / 2, 1);
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