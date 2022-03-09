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
	//�ړ�
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
	//���W�����킹��
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);
}

void Player::PushBlock(Vec3 BPos, float blockSize, const int up, const int down)
{
	//�v���C���[���W
	float PRight = position.x + r, PLeft = position.x - r;
	float PUp = position.z + r, PDown = position.z - r;
	//�v���C���[�̑O�̍��W
	float oldRight = oldPosition.x + r, oldLeft = oldPosition.x - r;
	float oldUp = oldPosition.z + r, oldDown = oldPosition.z - r;
	//�u���b�N���W
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;


	//����
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
	}
	//����
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			position.z = BUp + r;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			position.z = BUp + r;
		}
	}
	else
	{
		//�E
		if (oldRight <= BLeft)
		{
			position.x = BLeft - r;
		}
		//��
		if (oldLeft >= BRight)
		{
			position.x = BRight + r;
		}
		//��
		if (oldUp <= BDown)
		{
			position.z = BDown - r;
		}
		//��
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
