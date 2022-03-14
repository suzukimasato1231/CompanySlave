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

#if _DEBUG
	Vec3 angle{ 90.0f,00.0f,0.0f };
	Object::Instance()->Draw(circleM, position, scale, angle, color, white);

	////��
	//Object::Instance()->Draw(circleS, Vec3(position.x, position.y + 0.4f, position.z + circleMain), scale, angle, color, white);
	////��
	//Object::Instance()->Draw(circleS, Vec3(position.x, position.y + 0.3f, position.z - circleMain), scale, angle, color, white);
	////�E
	//Object::Instance()->Draw(circleS, Vec3(position.x + circleMain, position.y + 0.1f, position.z), scale, angle, color, white);
	////��
	//Object::Instance()->Draw(circleS, Vec3(position.x - circleMain, position.y + 0.2f, position.z), scale, angle, color, white);
#endif

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

	if (attackFlag == false)
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
	if ((Input::Instance()->KeybordTrigger(DIK_A) || Input::Instance()->KeybordTrigger(DIK_W) ||
		Input::Instance()->KeybordTrigger(DIK_S) || Input::Instance()->KeybordTrigger(DIK_D)) && attackFlag == false)
	{
		if (Input::Instance()->KeybordTrigger(DIK_A))
		{//��
			attackDirection = AttackLeft;
		}
		if (Input::Instance()->KeybordTrigger(DIK_W))
		{//��
			attackDirection = AttackUp;
		}
		if (Input::Instance()->KeybordTrigger(DIK_S))
		{//��
			attackDirection = AttackDown;
		}
		if (Input::Instance()->KeybordTrigger(DIK_D))
		{//�E
			attackDirection = AttackRight;
		}
		attackFlag = true;
		attackPos = Vec2(position.x, position.z);
	}
	//�R���{���r�؂��
	if (attackFlag == false)
	{
		comboNum = 0;
	}

	if (attackFlag == true && selectEnemyFlag == false)
	{
		bool isHit = false;
		bool discoverFlag = false;
		//�����蔻����͈͓̔��ɂ���G���ǂ���
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			isHit = AttackDirection(enemy, i);
			//�͈͓��ɂ��Ċ��G���R���{�U�����󂯂Ă��Ȃ�
			if (isHit == true && enemy->GetWasAttackFlag(i) == false)
			{
				discoverFlag = true;
				comboNum++;
				break;
			}
		}
		if (discoverFlag == true)
		{
			comboTime = comboMaxTime;
			nowComboTime = 0;
			selectEnemyFlag = true;
		}
		else
		{
			attackFlag = false;
			selectEnemyFlag = false;
		}
	}

	//�v���C���[�Ɉ�ԋ߂��G��T��
	if (nowComboTime == 0 && selectEnemyFlag == true && attackFlag == true)
	{
		float minPosition = 999.9f;//�v���C���[�ƓG�̍��̍ŏ��l

		//direction = {};//�v���C���[���猩���G�̌���
		for (size_t i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (enemy->GetWasAttackFlag(i) == false)
			{
				bool isHit = AttackDirection(enemy, i);

				if (isHit == true)
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
						direction = memoryPosition.normalize();
					}
				}
			}
		}
		//�v���C���[�ƓG�̍��W�̍����������G�̍��W������
		if (enemy->GetEnemySize() != 0)
		{
			enemyPos = enemy->GetPosition(enemyNum);
			enemy->WasAttack(enemyNum);
			eBox.minPosition = XMVectorSet(
				enemy->GetPosition(enemyNum).x - enemy->GetEnemyR(enemyNum) - direction.x * 15,
				enemy->GetPosition(enemyNum).y - enemy->GetEnemyR(enemyNum) - direction.y * 15,
				enemy->GetPosition(enemyNum).z - enemy->GetEnemyR(enemyNum) - direction.z * 15,
				1);
			eBox.maxPosition = XMVectorSet(
				enemy->GetPosition(enemyNum).x + enemy->GetEnemyR(enemyNum) - direction.x * 15,
				enemy->GetPosition(enemyNum).y + enemy->GetEnemyR(enemyNum) - direction.y * 15,
				enemy->GetPosition(enemyNum).z + enemy->GetEnemyR(enemyNum) - direction.z * 15,
				1);
		}
	}

	//�G�Ɍ������Ă�������
	if (attackFlag == true && selectEnemyFlag == true)
	{
		//�G�ƃv���C���[������������
		if (Collision::CheckBox2Box(pBox, eBox))
		{
			enemy->WasAttack(enemyNum);
			selectEnemyFlag = false;
		}
		//�G�̕����Ɍ������Ă���
		position -= direction * AttackSpeed;

		//�R���{�^�C���{
		nowComboTime++;
		//�R���{�^�C���I�������attackFlag��false�R���{�_���[�W�~�X�t���O��true�ɂ���
		if (nowComboTime >= comboTime)
		{
			attackFlag = false;
			selectEnemyFlag = false;
		}
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
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
			(Vec2(position.x - circleMain, position.z), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackRight:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5)
			&& Collision::CircleCollision
			(Vec2(position.x + circleMain, position.z), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackUp:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			Collision::CircleCollision
			(Vec2(position.x, position.z + circleMain), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			enemy->GetWasAttackFlag(enemyNumber) == false)
		{
			return  true;
		}
		break;
	case AttackDown:
		if (Collision::CircleCollision
		(attackPos, Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
			Collision::CircleCollision
			(Vec2(position.x, position.z - circleMain), Vec2(enemy->GetPosition(enemyNumber).x, enemy->GetPosition(enemyNumber).z), circleMain, 5) &&
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
	}
}



