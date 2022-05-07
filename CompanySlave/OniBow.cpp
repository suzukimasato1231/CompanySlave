
#include "OniBow.h"
#include"Player.h"
#include"Shape.h"
OniBow::OniBow()
{
}

OniBow::~OniBow()
{
}

void OniBow::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);

	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	//�G�̓ǂݍ���
	//enemyObject = Object::Instance()->CreateOBJ("OniKari");
	//�G�̍U����Ԃ̓ǂݍ���
	attackOBJ[0] = Object::Instance()->CreateOBJ("Oniyumi2-0","OniyumiOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("Oniyumi2-1", "OniyumiOBJ/");
	attackOBJ[2] = Object::Instance()->CreateOBJ("Oniyumi2-2", "OniyumiOBJ/");
	attackOBJ[3] = Object::Instance()->CreateOBJ("Oniyumi2-3", "OniyumiOBJ/");
	attackOBJ[4] = Object::Instance()->CreateOBJ("Oniyumi2-4", "OniyumiOBJ/");

	//�G�|����
	enemyWalk[0] = Object::Instance()->CreateOBJ("Oniyumi1-0", "OniyumiOBJ/");
	enemyWalk[1] = Object::Instance()->CreateOBJ("Oniyumi1-1", "OniyumiOBJ/");
	enemyWalk[2] = Object::Instance()->CreateOBJ("Oniyumi1-2", "OniyumiOBJ/");

	//���obj�Ǎ�
	bowOBJ = Object::Instance()->CreateOBJ("enemyArrow","OniyumiOBJ/");

	bowRaysOBJ = Shape::CreateRect(2.0f, 10.0f);
}

void OniBow::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		switch (oniData->direction)
		{
		case Up:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + attackEnemies.y / 2),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Down:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - attackEnemies.y / 2),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Left:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x - attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		case Right:
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x + attackEnemies.y / 2, oniData->position.y, oniData->position.z),
				oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			break;
		}
		break;
	case MOVE:
		Object::Instance()->Draw(enemyWalk[oniData->walkNum], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case ATTACK:
		if (oniData->StatusTime >= attackMotionTime - 25)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 50)
		{
			Object::Instance()->Draw(attackOBJ[1], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 75)
		{
			Object::Instance()->Draw(attackOBJ[2], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else if (oniData->StatusTime >= attackMotionTime - 100)
		{
			Object::Instance()->Draw(attackOBJ[3], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		else
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[4], oniData->position, oniData->scale, Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 30.0f, 0.0f), oniData->color);
		}
		break;
	}


	//��̕`��
	if (oniData->bowTime > 0)
	{
		Object::Instance()->Draw(bowOBJ, oniData->bowPos, Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, -XMConvertToDegrees(oniData->bowAngle) + 90.0f, 0.0f));
	}
	//��̎ː������̕`��
	if (oniData->StatusTime >= attackMotionDamege)
	{
		//Object::Instance()->Draw(bowRaysOBJ, Vec3(oniData->position.x, 0.0f, oniData->position.z), Vec3(2.0f, 2.0f, 2.0f), Vec3(90.0f, -XMConvertToDegrees(oniData->bowAngle) + 90.0f, 0.0f));
	}
}

void OniBow::Move(EnemyData* oniData, Player* player)
{
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();

	//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
	Vec3 memoryPosition = player->GetPosition() - oniData->position;
	//���������߂�
	float length = memoryPosition.length();
	//�v���C���[�ƓG���߂������痣���
	if (length < player2EnemyDislikeLength)
	{
		//�v���C���[�̌���
		Vec3 direction = memoryPosition.normalize();
		oniData->position -= direction * moveSpeed * slowValue;
		if (oniData->StatusTime <= 0)
		{
			oniData->Status = ATTACK;
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = attackMotionTime;
		}
	}
	//�v���C���[�ƓG���ǂ�������������U��
	else if (length < player2EnemyLength)
	{
		oniData->Status = ATTACK;
		oniData->attackDirection = oniData->direction;
		oniData->StatusTime = attackMotionTime;
	}
	//�v���C���[�Ɨ��ꂷ������U��
	else
	{
		//�v���C���[�̌���
		Vec3 direction = memoryPosition.normalize();
		oniData->position += direction * moveSpeed * slowValue;
	}
	//�����`��̐���
	if (oniData->walkTime % 10 == 0)
	{
		oniData->walkNum++;
		if (oniData->walkNum >= 3)
		{
			oniData->walkNum = 0;
			oniData->walkTime = 0;
		}
	}
	oniData->walkTime++;
	oniData->StatusTime--;
}

void OniBow::SearchPlayer(EnemyData* oniData, Player* player)
{
	Box enemiesBox = SearchField(oniData);
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[�ƓG�̋���
		float Length = Vec3(player->GetPosition() - oniData->position).length();

		if (Length < player2EnemyLength)
		{//�v���C���[���U�������ɂ��Ȃ�������ړ�
			oniData->Status = MOVE;
			oniData->StatusTime = moveTime;
		}
		else
		{//�v���C���[���U�������ɂ�����
			oniData->Status = ATTACK;
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = attackMotionTime;
		}
	}
}

void OniBow::Attack(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		return;
	}

	//�v���C���[��_��
	if (oniData->StatusTime > attackMotionDamege)
	{

		oniData->bowAngle = atan2(player->GetPosition().z - oniData->position.z, player->GetPosition().x - oniData->position.x);
	}
	//�v���C���[�Ɍ���
	else if (oniData->StatusTime == attackMotionDamege && oniData->bowTime <= 0)
	{
		oniData->bowFlag = true;
		oniData->bowMove = true;
		oniData->bowPos = oniData->position;
		oniData->bowTime = bowTimeMax;
	}

	//���Ԃ��I���������G�ɂ��ǂ�
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
		oniData->StatusTime = moveTime;
		oniData->bowPos = { 0.0f,0.0f,0.0f };
	}
	if (oniData->bowTime == bowTimeMax)
	{
		oniData->bowAFlag = true;
	}
	else {
		oniData->bowAFlag = false;
	}
}

void OniBow::BowUpdate(EnemyData* oniData, Player* player)
{
	if (oniData->bowTime >= 0)
	{
		OBB playerOBB;
		playerOBB.Initilize(player->GetPosition(), player->GetAngle(), Vec3(player->GetPSize(), player->GetPSize(), player->GetPSize()));
		OBB bowOBB;
		bowOBB.Initilize(oniData->bowPos, Vec3(0.0f, 0.0f, oniData->bowAngle), bowSize);
		oniData->bowOldPos = oniData->bowPos;
		if (oniData->bowMove == true)
		{
			oniData->bowPos.x += cosf(oniData->bowAngle) * bowSpeed;
			oniData->bowPos.z += sinf(oniData->bowAngle) * bowSpeed;
		}

		//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
		if (OBBCollision::ColOBBs(playerOBB, bowOBB) && oniData->bowFlag == true && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
			oniData->bowFlag = false;
		}
		oniData->bowTime--;
		if (oniData->bowTime <= 0)
		{
			oniData->bowFlag = false;
		}
	}
}

Box OniBow::SearchField(EnemyData* oniData)
{
	Box enemiesBox;
	switch (oniData->direction)
	{
	case Up:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.x / 2, oniData->position.y, oniData->position.z + attackEnemies.y, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.x / 2, oniData->position.y, oniData->position.z, 1);
		break;
	case Left:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.y, oniData->position.y, oniData->position.z - attackEnemies.x / 2, 1);
		break;
	case Right:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.y, oniData->position.y, oniData->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z - attackEnemies.x / 2, 1);
		break;
	case Down:
		enemiesBox.maxPosition = XMVectorSet(oniData->position.x + attackEnemies.x / 2, oniData->position.y, oniData->position.z, 1);
		enemiesBox.minPosition = XMVectorSet(oniData->position.x - attackEnemies.x / 2, oniData->position.y, oniData->position.z - attackEnemies.y, 1);
		break;
	default:
		enemiesBox.maxPosition = {};
		enemiesBox.minPosition = {};
	}
	return enemiesBox;
}

Vec3 OniBow::DirectionAngle(int direction)
{
	Vec3 angle = {};
	switch (direction)
	{
	case Up:
		angle = { 0.0f,270.0f,0.0f };
		break;
	case Left:
		angle = { 0.0f,180.0f,0.0f };
		break;
	case Right:
		angle = { 0.0f,0.0f,0.0f };
		break;
	case Down:
		angle = { 0.0f,90.0f,0.0f };
		break;
	default:
		angle = { 0.0f,0.0f,0.0f };
	}
	return angle;
}



