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
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);
}

//移動
void Player::Move()
{
	oldPosition = position;
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

//プレイヤーとエネミーとの最小距離の敵を見つける
void Player::PlayerAttack(Enemy *enemy)
{
	if (enemy == nullptr) { return; }
	//敵倒す
	if (Input::Instance()->KeybordTrigger(DIK_SPACE))
	{
		attackFlag = true;

	}

	if (attackFlag == true && nowComboTime == comboTime)
	{
		bool isHit = true;
		bool discoverFlag = false;
		//当たり判定一定の範囲内にいる敵かどうか
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			//当たり判定
			isHit = Collision::CircleCollision(Vec2(position.x, position.z),
				Vec2(enemy->GetPosition(i).x, enemy->GetPosition(i).z),
				100.0f, 5.0f);
			//範囲内にいて且つ敵がコンボ攻撃を受けていない
			if (isHit == true && enemy->GetWasAttackFlag(i) == false)
			{
				discoverFlag = true;
				break;
			}
			if (enemy->GetEnemySize())
			{

			}
		}
		if (discoverFlag == true)
		{
			comboTime = comboMaxTime;
			startPos = position;
			nowComboTime = 0;
		}
		else
		{
			attackFlag = false;
		}
	}

	//プレイヤーに一番近い敵を探す
	if (comboTime == comboMaxTime && nowComboTime == 0 && attackFlag == true)
	{
		float minPosition = 999.9f;//プレイヤーと敵の差の最小値
		int enemyNum = 0;//敵の配列の位置
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (enemy->GetWasAttackFlag(i) == false)
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
				}
			}
		}
		//プレイヤーと敵の座標の差が小さい敵の座標を入れる
		if (enemy->GetEnemySize() != 0)
		{
			enemyPos = enemy->GetPosition(enemyNum);
			enemy->WasAttack(enemyNum);
		}
	}

	//敵に向かっていく処理
	if (nowComboTime != comboTime)
	{
		nowComboTime++;
		float timeRoate = min(nowComboTime / comboTime, 1.0f);
		position = Easing::easeIn(startPos, enemyPos, timeRoate);
		//座標を合わせる
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}



