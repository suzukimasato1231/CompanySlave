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

//�v���C���[�ƃG�l�~�[�Ƃ̍ŏ������̓G��������
void Player::PlayerAttack(Enemy *enemy)
{
	if (enemy == nullptr) { return; }
	//�G�|��
	if (Input::Instance()->KeybordTrigger(DIK_SPACE))
	{
		attackFlag = true;

	}

	if (attackFlag == true && nowComboTime == comboTime)
	{
		bool isHit = true;
		bool discoverFlag = false;
		//�����蔻����͈͓̔��ɂ���G���ǂ���
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			//�����蔻��
			isHit = Collision::CircleCollision(Vec2(position.x, position.z),
				Vec2(enemy->GetPosition(i).x, enemy->GetPosition(i).z),
				100.0f, 5.0f);
			//�͈͓��ɂ��Ċ��G���R���{�U�����󂯂Ă��Ȃ�
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

	//�v���C���[�Ɉ�ԋ߂��G��T��
	if (comboTime == comboMaxTime && nowComboTime == 0 && attackFlag == true)
	{
		float minPosition = 999.9f;//�v���C���[�ƓG�̍��̍ŏ��l
		int enemyNum = 0;//�G�̔z��̈ʒu
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (enemy->GetWasAttackFlag(i) == false)
			{
				//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
				Vec3 memoryPosition = position - enemy->GetPosition(i);
				//���������߂�
				float length = memoryPosition.length();
				//�����̍ŏ��l�����߂�
				if (length < minPosition)
				{
					minPosition = length;
					enemyNum = i;
				}
			}
		}
		//�v���C���[�ƓG�̍��W�̍����������G�̍��W������
		if (enemy->GetEnemySize() != 0)
		{
			enemyPos = enemy->GetPosition(enemyNum);
			enemy->WasAttack(enemyNum);
		}
	}

	//�G�Ɍ������Ă�������
	if (nowComboTime != comboTime)
	{
		nowComboTime++;
		float timeRoate = min(nowComboTime / comboTime, 1.0f);
		position = Easing::easeIn(startPos, enemyPos, timeRoate);
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}



