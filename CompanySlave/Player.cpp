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
	playerObject = Object::Instance()->CreateOBJ("player");
	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;

	//コンボUI
	comboPolygon = Shape::CreateRect(20.0f, 10.0f);
	comboGraph = Object::Instance()->LoadTexture(L"Resources/ComboUI/ComboGauge.png");

	black = Shape::CreateRect(16.0f, 2.0f);
	yellowColor = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	//コンボ数字
	comboNumberObj = Shape::CreateRect(5.0f, 7.0f);

	comboNumberGraph[0] = Object::Instance()->LoadTexture(L"Resources/ComboUI/0.png");
	comboNumberGraph[1] = Object::Instance()->LoadTexture(L"Resources/ComboUI/1.png");
	comboNumberGraph[2] = Object::Instance()->LoadTexture(L"Resources/ComboUI/2.png");
	comboNumberGraph[3] = Object::Instance()->LoadTexture(L"Resources/ComboUI/3.png");
	comboNumberGraph[4] = Object::Instance()->LoadTexture(L"Resources/ComboUI/4.png");
	comboNumberGraph[5] = Object::Instance()->LoadTexture(L"Resources/ComboUI/5.png");
	comboNumberGraph[6] = Object::Instance()->LoadTexture(L"Resources/ComboUI/6.png");
	comboNumberGraph[7] = Object::Instance()->LoadTexture(L"Resources/ComboUI/7.png");
	comboNumberGraph[8] = Object::Instance()->LoadTexture(L"Resources/ComboUI/8.png");
	comboNumberGraph[9] = Object::Instance()->LoadTexture(L"Resources/ComboUI/9.png");

#if _DEBUG
	attackField = Shape::CreateRect(10.0f, 20.0f);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	normalFieldOBJ = Shape::CreateRect(5.0f, 5.0f);
	normalGraph = Object::Instance()->LoadTexture(L"Resources/kengeki.png");
#endif
}

void Player::Update(Enemy *enemy)
{
	if (enemy == nullptr)
	{
		return;
	}
	//移動
	Move();
	//プレイヤーの向きを決める
	PDirection();
	//通常攻撃
	NormalAttack(enemy);

	//斬りに行く敵の座標を探す
	PlayerAttack(enemy);

	//攻撃を止める
	StopAttack();
	//回避
	Avoidance();

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
	Object::Instance()->Draw(playerObject, position, scale, angle, color);

	if (nowComboTime > 0)
	{
		//ゲージ
		float parsent = nowComboTime / comboTime;
		Vec3 UIPos = { position.x - (1.0f - parsent) * 8,position.y - 1.0f,position.z - 12.0f };
		Object::Instance()->Draw(black, UIPos, Vec3(parsent, 1.0f, 1.0f), UIAngle, Vec4(0.0f, 0.0f, 0.0f, 0.0f), yellowColor);
		//数字
		UIPos = { position.x,position.y - 1.0f,position.z - 7.0f };
		//1桁目
		int one = comboNum % 10;
		Object::Instance()->Draw(comboNumberObj, UIPos, scale, UIAngle, color, comboNumberGraph[one]);
		//２桁目
		int two = comboNum % 100 / 10;
		if (comboNum >= 10)
		{
			UIPos = { position.x - 5.0f,position.y,position.z - 8.0f };
			Object::Instance()->Draw(comboNumberObj, UIPos, scale, UIAngle, color, comboNumberGraph[two]);
		}
		//枠
		UIPos = { position.x,position.y,position.z - 8.0f };
		Object::Instance()->Draw(comboPolygon, UIPos, scale, UIAngle, color, comboGraph);
	}
}

//移動
void Player::Move()
{
	oldPosition = position;
	//移動
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
		|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
	{
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}
	if (attackFlag == false && nowComboTime <= 0 && avoidanceTime <= 0)
	{
		//移動
		if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight))
		{
			position.x += speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft))
		{
			position.x -= speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp))
		{
			position.z += speed.z;
		}
		if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown))
		{
			position.z -= speed.z;
		}
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

//プレイヤーとエネミーとの最小距離の敵を見つける
void Player::PlayerAttack(Enemy *enemy)
{
	if (enemy == nullptr) { return; }
	//攻撃開始
	if ((Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->KeybordPush(DIK_RIGHT)
		|| Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
		|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
		&& (Input::Instance()->KeybordPush(DIK_C) || Input::Instance()->ControllerPush(ButtonA))
		&& coolTime <= 0 && attackFlag == false)
	{
		attackAngle = Angle();
		attackFlag = true;
		comboFlag = false;
		attackTime = attackMaxTime;
	}
	//敵に向かっていく処理
	if (attackFlag == true && attackTime > 0)
	{
		//敵の方向に向かっていく
		position.x += attackSpeed.x * cosf(attackAngle);
		position.z += attackSpeed.z * sinf(attackAngle);

		//敵と当たったか
		for (int i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (Collision::CheckBox2Box(pBox, enemy->GetBox(i))
				&& enemy->GetWasAttackFlag(i) == false)
			{
				enemy->WasAttack(i);
				comboFlag = true;
				comboNum++;
			}
		}
		//攻撃時間減少
		attackTime--;
		if (attackTime == 0)
		{//敵を斬り終わったら
			attackFlag = false;
			//コンボ成功時
			if (comboFlag == true)
			{
				comboTime = comboMaxTime;
				nowComboTime = comboTime;
			}
			//コンボ失敗時
			else
			{
				coolTime = coolTimeMax;
				nowComboTime = 0;
				comboNum = 0;
			}
		}
	}
	//コンボタイム減少
	if (comboFlag == true && attackFlag == false)
	{
		nowComboTime--;
		if (nowComboTime == 0)
		{
			comboFlag = false;
			coolTime = coolTimeMax;
			comboNum = 0;
		}
	}
	//クールタイム現象
	if (coolTime > 0 && comboFlag == false && attackFlag == false)
	{
		coolTime--;
	}
}

void Player::StopAttack()
{
	if (Input::Instance()->KeybordPush(DIK_SPACE))
	{
		attackFlag = false;
		comboFlag = false;
		coolTime = coolTimeMax;
		nowComboTime = 0;
		comboNum = 0;
	}
}

void Player::NormalAttack(Enemy *enemy)
{
	//クールタイム
	if (normalAttackCoolTime >= 0)
	{
		normalAttackCoolTime--;
		if (normalAttackCoolTime <= 0)
		{
			normalAttackFlag = false;
		}
	}
	if (Input::Instance()->KeybordTrigger(DIK_D) && normalAttackFlag == false &&
		attackFlag == false && comboFlag == false && avoidanceFlag == false)
	{
		normalAttackFlag = true;
		normalAttackCoolTime = normalAttackCoolTimeMax;
		for (int i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (Collision::CheckSphere2Box(enemy->GetSphere(i), normalAttackBox))
			{
				enemy->DamegeNormal(i);
			}
		}
	}
}

float  Player::Angle()
{	//右上
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		float rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//右下
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//左下
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//左上
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//上
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		return rad;
	}//右
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		float rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		return rad;
	}//下
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		return rad;
	}//左
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		float rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		return rad;
	}
	return 0.0f;
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
		&& attackFlag == false && comboFlag == false && avoidanceFlag == false)
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
		float radDir = 0.0f;
		switch (avoiDirection)
		{
		case Up:
			position += Vec3(0.0f, 0.0f, +avoiSpeed); break;
		case Down:
			position += Vec3(0.0f, 0.0f, -avoiSpeed); break;
		case Left:
			position += Vec3(-avoiSpeed, 0.0f, 0.0f); break;
		case Right:
			position += Vec3(avoiSpeed, 0.0f, 0.0f); break;
		case UpLeft:
			radDir = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
			//敵の方向に向かっていく
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case UpRight:
			radDir = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
			//敵の方向に向かっていく
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case DownLeft:
			radDir = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
			//敵の方向に向かっていく
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case DownRight:
			radDir = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
			//敵の方向に向かっていく
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		}
	}
}


void Player::PDirection()
{
	//右上
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		direction = UpRight;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z + normalFieldR, 1);
	}//右下
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownRight;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z - normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z - normalFieldR, 1);
	}//左下
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownLeft;
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalField, position.y, position.z - normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z - normalFieldR, 1);
	}//左上
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		direction = UpLeft;
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalField, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z + normalFieldR, 1);
	}//上
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		direction = Up;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z, 1);
	}//右
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		direction = Right;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z + normalFieldR, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalFieldR, 1);
	}//下
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		direction = Down;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z - normalField, 1);
	}//左
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		direction = Left;
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z + normalFieldR, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalField, position.y, position.z - normalFieldR, 1);
	}
}

void Player::DebugDraw()
{
	Vec3 attackPosition{};
	Vec3 attackAngle{};
	//コンボ方向
	if (Input::Instance()->KeybordPush(DIK_UP) && Input::Instance()->KeybordPush(DIK_RIGHT))
	{
		attackPosition = { position.x + 10.0f,position.y,position.z + 10.0f };
		attackAngle = { 90.0f,0.0f,135.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_DOWN) && Input::Instance()->KeybordPush(DIK_LEFT))
	{
		attackPosition = { position.x - 10.0f,position.y,position.z - 10.0f };
		attackAngle = { 90.0f,0.0f,135.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_UP) && Input::Instance()->KeybordPush(DIK_LEFT))
	{
		attackPosition = { position.x - 10.0f,position.y,position.z + 10.0f };
		attackAngle = { 90.0f,0.0f,45.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_DOWN) && Input::Instance()->KeybordPush(DIK_RIGHT))
	{
		attackPosition = { position.x + 10.0f,position.y,position.z - 10.0f };
		attackAngle = { 90.0f,0.0f,45.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_RIGHT))
	{
		attackPosition = { position.x + 10.0f,position.y,position.z };
		attackAngle = { 90.0f,0.0f,90.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_LEFT))
	{
		attackPosition = { position.x - 10.0f,position.y,position.z };
		attackAngle = { 90.0f,0.0f,90.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_UP))
	{
		attackPosition = { position.x,position.y,position.z + 10.0f };
		attackAngle = { 90.0f,0.0f,0.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	else if (Input::Instance()->KeybordPush(DIK_DOWN))
	{
		attackPosition = { position.x,position.y,position.z - 10.0f };
		attackAngle = { 90.0f,0.0f,0.0f };
		Object::Instance()->Draw(attackField, attackPosition, scale, attackAngle, color, redColor);
	}
	//通常攻撃エフェクト
	if (normalAttackFlag == true && normalAttackCoolTime == normalAttackCoolTimeMax)
	{
		switch (direction)
		{
		case Up:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z + r), scale, UIAngle, color, normalGraph);
			break;
		case Down:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z - r), scale, UIAngle, color, normalGraph);
			break;
		case Left:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - r, position.y, position.z), scale, UIAngle, color, normalGraph);
			break;
		case Right:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + r, position.y, position.z), scale, UIAngle, color, normalGraph);
			break;
		case UpRight:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + r, position.y + r, position.z), scale, UIAngle, color, normalGraph);
			break;
		case UpLeft:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - r, position.y + r, position.z), scale, UIAngle, color, normalGraph);
			break;
		case DownRight:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + r, position.y - r, position.z), scale, UIAngle, color, normalGraph);
			break;
		case DownLeft:
			Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - r, position.y - r, position.z), scale, UIAngle, color, normalGraph);
			break;
		}
	}
}
