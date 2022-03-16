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
	oldPosition = position;

#if _DEBUG
	circleM = Shape::CreateCircle(circleMain, 30);

	circleS = Shape::CreateCircle(ciecleSub, 30);

	white = Object::Instance()->LoadTexture(L"Resources/color/white.png");
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

	//斬りに行く敵の座標を探す
	PlayerAttack(enemy);

	//攻撃を止める
	StopAttack();
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);

#if _DEBUG
	Vec3 angle{ 90.0f,00.0f,0.0f };
	Object::Instance()->Draw(circleM, position, scale, angle, color, white);

	////上
	//Object::Instance()->Draw(circleS, Vec3(position.x, position.y + 0.4f, position.z + circleMain), scale, angle, color, white);
	////下
	//Object::Instance()->Draw(circleS, Vec3(position.x, position.y + 0.3f, position.z - circleMain), scale, angle, color, white);
	////右
	//Object::Instance()->Draw(circleS, Vec3(position.x + circleMain, position.y + 0.1f, position.z), scale, angle, color, white);
	////左
	//Object::Instance()->Draw(circleS, Vec3(position.x - circleMain, position.y + 0.2f, position.z), scale, angle, color, white);
#endif

}

//移動
void Player::Move()
{
	oldPosition = position;

	//移動
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN))
	{
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	if (attackFlag == false)
	{
		//移動
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
	if ((Input::Instance()->KeybordTrigger(DIK_A) || Input::Instance()->KeybordTrigger(DIK_W) ||
		Input::Instance()->KeybordTrigger(DIK_S) || Input::Instance()->KeybordTrigger(DIK_D))
		&& attackFlag == false)
	{
		if (Input::Instance()->KeybordTrigger(DIK_A))
		{//左
			attackDirection = AttackLeft;
		}
		if (Input::Instance()->KeybordTrigger(DIK_W))
		{//上
			attackDirection = AttackUp;
		}
		if (Input::Instance()->KeybordTrigger(DIK_S))
		{//下
			attackDirection = AttackDown;
		}
		if (Input::Instance()->KeybordTrigger(DIK_D))
		{//右
			attackDirection = AttackRight;
		}
		attackFlag = true;
		attackPos = Vec2(position.x, position.z);
	}
	ComboDirection();
	//コンボが途切れる
	if (attackFlag == false)
	{
		comboNum = 0;
	}

	if (attackFlag == true && selectEnemyFlag == false && timeOutFlag == true)
	{
		bool isHit = false;
		bool discoverFlag = false;
		timeOutFlag = false;
		//当たり判定一定の範囲内にいる敵かどうか
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			isHit = AttackDirection(enemy, i);
			//範囲内にいて且つ敵がコンボ攻撃を受けていない
			if (isHit == true && enemy->GetWasAttackFlag(i) == false)
			{
				if (comboNum == 0)
				{//コンボ最初
					discoverFlag = true;
					comboNum++;
					break;
				}
				else if (AttackComboDirection(enemy, i) && comboFlag == true)
				{//コンボ継続
					comboNum++;
					discoverFlag = true;
					break;
				}
				else if (AttackComboDirection(enemy, i) && comboFlag == false)
				{//コンボ失敗
					discoverFlag = true;
					comboNum = 0;
					break;
				}
			}
		}
		if (discoverFlag == true)
		{
			selectEnemyFlag = true;
		}

	}

	//プレイヤーに一番近い敵を探す
	if (attackSpeedTime == 0 && selectEnemyFlag == true && attackFlag == true)
	{
		float minPosition = 999.9f;//プレイヤーと敵の差の最小値
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (enemy->GetWasAttackFlag(i) == false)
			{
				bool isHit = AttackDirection(enemy, i);

				if (isHit == true)
				{
					//プレイヤーとエネミーの位置の差
					Vec3 memoryPosition = position - enemy->GetPosition(i);
					//長さを求める
					float length = memoryPosition.length();
					//距離の最小値を求める
					if (length < minPosition)
					{
						minPosition = length;
						enemyNum = i;
						direction = memoryPosition.normalize();
					}
				}
			}
		}
		//プレイヤーと敵の座標の差が小さい敵の座標を入れる
		if (enemy->GetEnemySize() != 0)
		{
			startPos = position;
			enemyPos = enemy->GetPosition(enemyNum) - direction * 6.0f;
			enemy->WasAttack(enemyNum);
		}
	}

	//敵に向かっていく処理
	if (attackFlag == true && selectEnemyFlag == true)
	{
		//敵の方向に向かっていく
		float timeRote = min(attackSpeedTime / attackSpeedMaxTime, 1.0f);
		position = Easing::easeIn(startPos, enemyPos, timeRote);
		attackSpeedTime++;
		if (timeRote >= 1.0f)
		{//敵を斬り終わったら
			enemy->WasAttack(enemyNum);
			attackSpeedTime = 0;
			selectEnemyFlag = false;
			//コンボ猶予時間
			nowComboTime = comboTime;
			//comboFlag = true;
		}
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}

	//コンボ時間が減っていく
	if (nowComboTime > 0 && selectEnemyFlag == false && attackFlag == true)
	{
		nowComboTime--;

	}

}

void Player::Damege()
{
}

bool Player::AttackDirection(Enemy *enemy, int enemyNumber)
{
	switch (attackDirection)
	{
	case AttackLeft:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5)
			&& Collision::CircleCollision
			(Vec2(attackPos.x - circleMain, attackPos.y), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackRight:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5)
			&& Collision::CircleCollision
			(Vec2(attackPos.x + circleMain, attackPos.y), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackUp:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			Collision::CircleCollision
			(Vec2(attackPos.x, attackPos.y + circleMain), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackDown:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			Collision::CircleCollision
			(Vec2(attackPos.x, attackPos.y - circleMain), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	default:
		return false;
		break;
	}
}

void Player::StopAttack()
{
	if (Input::Instance()->KeybordTrigger(DIK_SPACE))
	{
		attackFlag = false;
		selectEnemyFlag = false;
	}
}

void Player::ComboDirection()
{
	//攻撃開始
	if ((Input::Instance()->KeybordTrigger(DIK_A) || Input::Instance()->KeybordTrigger(DIK_W) ||
		Input::Instance()->KeybordTrigger(DIK_S) || Input::Instance()->KeybordTrigger(DIK_D))
		&& attackFlag == true)
	{
		comboDirection = AttackNULL;
		if (Input::Instance()->KeybordTrigger(DIK_A))
		{//左
			comboDirection = AttackLeft;
		}
		if (Input::Instance()->KeybordTrigger(DIK_W))
		{//上
			comboDirection = AttackUp;
		}
		if (Input::Instance()->KeybordTrigger(DIK_S))
		{//下
			comboDirection = AttackDown;
		}
		if (Input::Instance()->KeybordTrigger(DIK_D))
		{//右
			comboDirection = AttackRight;
		}
		if (nowComboTime > 0)
		{
			nowComboTime = 0;
			comboFlag = true;
		}
		else
		{
			comboFlag = false;
			nowComboTime = 0;
		}
		timeOutFlag = true;
	}
}



bool Player::AttackComboDirection(Enemy *enemy, int enemyNumber)
{
	switch (comboDirection)
	{
	case AttackLeft:
		if (enemy->GetPosition(enemyNumber).x < position.x)
		{
			return  true;
		}
		break;
	case AttackRight:
		if (enemy->GetPosition(enemyNumber).x > position.x)
		{
			return  true;
		}
		break;
	case AttackUp:
		if (enemy->GetPosition(enemyNumber).z > position.z)
		{
			return  true;
		}
		break;
	case AttackDown:
		if (enemy->GetPosition(enemyNumber).z < position.z)
		{
			return  true;
		}
		break;
	default:
		return false;
		break;
	}

	return false;
}