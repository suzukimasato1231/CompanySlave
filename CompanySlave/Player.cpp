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
	//�ړ�
	Move();
	//�v���C���[�̌��������߂�
	PDirection();
	//�ʏ�U��
	NormalAttack(enemy);

	//�a��ɍs���G�̍��W��T��
	PlayerAttack(enemy);

	//�U�����~�߂�
	StopAttack();
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
	Object::Instance()->Draw(playerObject, position, scale, angle, color);

	if (nowComboTime > 0)
	{
		//�Q�[�W
		float parsent = nowComboTime / comboTime;
		Vec3 UIPos = { position.x - (1.0f - parsent) * 8,position.y - 1.0f,position.z - 12.0f };
		Object::Instance()->Draw(black, UIPos, Vec3(parsent, 1.0f, 1.0f), UIAngle, Vec4(0.0f, 0.0f, 0.0f, 0.0f), yellowColor);
		//����
		UIPos = { position.x,position.y - 1.0f,position.z - 7.0f };
		//1����
		int one = comboNum % 10;
		Object::Instance()->Draw(comboNumberObj, UIPos, scale, UIAngle, color, comboNumberGraph[one]);
		//�Q����
		int two = comboNum % 100 / 10;
		if (comboNum >= 10)
		{
			UIPos = { position.x - 5.0f,position.y,position.z - 8.0f };
			Object::Instance()->Draw(comboNumberObj, UIPos, scale, UIAngle, color, comboNumberGraph[two]);
		}
		//�g
		UIPos = { position.x,position.y,position.z - 8.0f };
		Object::Instance()->Draw(comboPolygon, UIPos, scale, UIAngle, color, comboGraph);
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
	if (attackFlag == false && nowComboTime <= 0 && avoidanceTime <= 0)
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

//�v���C���[�ƃG�l�~�[�Ƃ̍ŏ������̓G��������
void Player::PlayerAttack(Enemy *enemy)
{
	if (enemy == nullptr) { return; }
	//�U���J�n
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
	//�G�Ɍ������Ă�������
	if (attackFlag == true && attackTime > 0)
	{
		//�G�̕����Ɍ������Ă���
		position.x += attackSpeed.x * cosf(attackAngle);
		position.z += attackSpeed.z * sinf(attackAngle);

		//�G�Ɠ���������
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
		//�U�����Ԍ���
		attackTime--;
		if (attackTime == 0)
		{//�G���a��I�������
			attackFlag = false;
			//�R���{������
			if (comboFlag == true)
			{
				comboTime = comboMaxTime;
				nowComboTime = comboTime;
			}
			//�R���{���s��
			else
			{
				coolTime = coolTimeMax;
				nowComboTime = 0;
				comboNum = 0;
			}
		}
	}
	//�R���{�^�C������
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
	//�N�[���^�C������
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
	//�N�[���^�C��
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
{	//�E��
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		float rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		return rad;
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		float rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		return rad;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		return rad;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		float rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		return rad;
	}
	return 0.0f;
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
		&& attackFlag == false && comboFlag == false && avoidanceFlag == false)
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
			//�G�̕����Ɍ������Ă���
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case UpRight:
			radDir = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
			//�G�̕����Ɍ������Ă���
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case DownLeft:
			radDir = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
			//�G�̕����Ɍ������Ă���
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
			break;
		case DownRight:
			radDir = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
			//�G�̕����Ɍ������Ă���
			position.x += attackSpeed.x * cosf(radDir);
			position.z += attackSpeed.z * sinf(radDir);
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
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z + normalFieldR, 1);
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownRight;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z - normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z - normalFieldR, 1);
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownLeft;
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalField, position.y, position.z - normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z - normalFieldR, 1);
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		direction = UpLeft;
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalField, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z + normalFieldR, 1);
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		direction = Up;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z + normalField, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z, 1);
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		direction = Right;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalField, position.y, position.z + normalFieldR, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalFieldR, 1);
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		direction = Down;
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalFieldR, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalFieldR, position.y, position.z - normalField, 1);
	}//��
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
	//�R���{����
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
	//�ʏ�U���G�t�F�N�g
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
