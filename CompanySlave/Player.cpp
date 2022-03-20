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
	//�ړ�
	Move();

	//�a��ɍs���G�̍��W��T��
	PlayerAttack(enemy);

	//�U�����~�߂�
	StopAttack();
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);
}

//�ړ�
void Player::Move()
{
	oldPosition = position;

	//�ړ�
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN))
	{
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	if (attackFlag == false && nowComboTime <= 0)
	{
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
	if (((Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->KeybordPush(DIK_UP) ||
		Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->KeybordPush(DIK_RIGHT)))
		&& coolTime <= 0 && attackFlag == false && Input::Instance()->KeybordPush(DIK_C))
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
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
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
	else if (Input::Instance()->KeybordPush(DIK_UP)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		return rad;
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
		float rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		return rad;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		return rad;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
		float rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		return rad;
	}
	return 0.0f;
}
