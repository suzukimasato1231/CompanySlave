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

	//�R���{UI
	comboPolygon = Shape::CreateRect(20.0f, 10.0f);
	comboGraph = Object::Instance()->LoadTexture(L"Resources/ComboUI/ComboGauge.png");

	black = Shape::CreateRect(16.0f, 2.0f);
	yellowColor = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	//�R���{����
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

	normalFieldOBJ = Shape::CreateRect(normalLength, normalLength);
	normalGraph = Object::Instance()->LoadTexture(L"Resources/kengeki.png");
#endif
}

void Player::Update(Enemy *enemy)
{
	if (enemy == nullptr)
	{
		return;
	}
	Angle();
	//�ړ�
	Move();
	//�v���C���[�̌��������߂�
	PDirection();
	//�ʏ�U��
	NormalAttack(enemy);

	//���
	Avoidance();


	//���W�����킹��
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
}

void Player::Draw()
{
#if _DEBUG
	DebugDraw();
#endif
	//�v���C���[
	if (damageTime % 2 == 0)
	{
		Object::Instance()->Draw(playerObject, position, scale, angle, color);
	}
}

//�ړ�
void Player::Move()
{
	oldPosition = position;
	//�ړ�
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
	if (avoidanceTime <= 0 && normalAttackTime <= 0)
	{
		//�ړ�
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
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

void Player::NormalAttack(Enemy *enemy)
{

	if (Input::Instance()->KeybordTrigger(DIK_D) && avoidanceTime <= 0)
	{
		//�A���U���𐔂���
		if (normalAttackCount < 3)
		{
			normalAttackFlag[normalAttackCount] = true;
			normalAttackCount++;
		}
	}
	//�U�����Ă��鎞��
	if (normalAttackTime > 0)
	{
		normalAttackTime--;
		position.x += attackMoveSpeed * cosf(rad);
		position.z += attackMoveSpeed * sinf(rad);
	}

	for (int j = 0; j < 3; j++)
	{//�U������͈�ԍŏ�
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
	//�A���؂�܂ł̗P�\
	if (normalGraceTime >= 0 && normalAttackFlag[0] == false && normalAttackFlag[1] == false && normalAttackFlag[2] == false)
	{
		normalGraceTime--;
		//���ԏI�������A���؂胊�Z�b�g
		if (normalGraceTime <= 0)
		{
			normalAttackCount = 0;
		}
	}

}

void   Player::Angle()
{	//�E��
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		 rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		 rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		 rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		 rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
	
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		 rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
	
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		 rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		 rad = atan2(position.z - 10.0f - position.z, position.x - position.x);

	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		 rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
	}
}


void Player::Avoidance()
{
	//����N�[���^�C������
	if (avoiCoolTime > 0)
	{
		avoiCoolTime--;
		if (avoiCoolTime <= 0)
		{
			avoidanceFlag = false;
		}
	}

	//����J�n
	if ((Input::Instance()->KeybordTrigger(DIK_F) || Input::Instance()->ControllerDown(ButtonB))
		&& avoidanceFlag == false && normalAttackTime <= 0)
	{
		avoidanceFlag = true;
		avoidanceTime = avoidanceTimeMax;
		avoiDirection = direction;
	}
	//���
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
			position.x += avoiSpeed * cosf(radDir);
			position.z += avoiSpeed * sinf(radDir);
			break;
		case UpRight:
			radDir = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
			position.x += avoiSpeed * cosf(radDir);
			position.z += avoiSpeed * sinf(radDir);
			break;
		case DownLeft:
			radDir = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
			position.x += avoiSpeed * cosf(radDir);
			position.z += avoiSpeed * sinf(radDir);
			break;
		case DownRight:
			radDir = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
			position.x += avoiSpeed * cosf(radDir);
			position.z += avoiSpeed * sinf(radDir);
			break;
		}
	}
}


void Player::PDirection()
{
	//�E��
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		direction = UpRight;
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownRight;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownLeft;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		direction = UpLeft;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		direction = Up;
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		direction = Right;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		direction = Down;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		direction = Left;
	}
}

void Player::Damage(Enemy *enemy)
{
	if (avoidanceFlag == false)
	{
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			Vec2 pPos = { position.x,position.z };
			Vec2 ePos = { enemy->GetPosition(i).x,enemy->GetPosition(i).z };
			if (Collision::CircleCollision(pPos, ePos, r, enemy->GetEnemyR(i)))
			{
				damageTime = damageTimeMax;
				HP--;
			}
		}
	}
	if (damageTime > 0)
	{
		damageTime--;
	}
}

void Player::DebugDraw()
{
	Vec3 attackPosition{};
	Vec3 attackAngle{};

	//�ʏ�U���G�t�F�N�g

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
	}

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