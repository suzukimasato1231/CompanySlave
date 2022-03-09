#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{

	playerObject = Object::Instance()->CreateOBJ("sphere");

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);

	oldPosition = position;
}

void Player::Update()
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

	groundFlag = false;
	//座標を合わせる
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);
}

void Player::PushBlock(Vec3 BPos, float blockSize, const int up, const int down)
{
	//プレイヤー座標
	float PRight = position.x + r, PLeft = position.x - r;
	float PUp = position.z + r, PDown = position.z - r;
	//プレイヤーの前の座標
	float oldRight = oldPosition.x + r, oldLeft = oldPosition.x - r;
	float oldUp = oldPosition.z + r, oldDown = oldPosition.z - r;
	//ブロック座標
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;


	//左上
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
	}
	//左下
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			position.z = BUp + r;
		}
	}
	//右上
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
	}
	//右下
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			position.z = BUp + r;
		}
	}
	else
	{
		//右
		if (oldRight <= BLeft)
		{
			position.x = BLeft - r;
		}
		//左
		if (oldLeft >= BRight)
		{
			position.x = BRight + r;
		}
		//上
		if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
		//下
		if (oldDown >= BUp)
		{
			position.z = BUp + r;
		}
	}
}

Vec3 Player::GetPosition()
{
	return position;
}

Box Player::GetBox()
{
	return pBox;
}
