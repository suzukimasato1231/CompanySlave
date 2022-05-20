#include "TwinBoar.h"
#include"Player.h"
#include"Shape.h"
TwinBoar::TwinBoar()
{}

TwinBoar::~TwinBoar()
{}

void TwinBoar::Init()
{
#if _DEBUG
	debugField = Shape::CreateRect(attackField.y * 2, attackField.x * 2);
#endif
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//�ʏ���
	enemyObject[0] = Object::Instance()->CreateOBJ("Boar1-0", "BoarOBJ/");
	enemyObject[1] = Object::Instance()->CreateOBJ("Boar1-1", "BoarOBJ/");
	enemyObject[2] = Object::Instance()->CreateOBJ("Boar1-2", "BoarOBJ/");
	enemyObject[3] = Object::Instance()->CreateOBJ("Boar1-3", "BoarOBJ/");
	enemyObject[4] = Object::Instance()->CreateOBJ("Boar1-4", "BoarOBJ/");
	//�U�����[�V����
	attackOBJ[0] = Object::Instance()->CreateOBJ("Boar1-0", "BoarOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("Boar1-1", "BoarOBJ/");
	attackOBJ[2] = Object::Instance()->CreateOBJ("Boar1-2", "BoarOBJ/");
	attackOBJ[3] = Object::Instance()->CreateOBJ("Boar1-3", "BoarOBJ/");
	attackOBJ[4] = Object::Instance()->CreateOBJ("Boar1-4", "BoarOBJ/");


	//�{�X�̐��l
	bossData.HP = 168.0f;
	bossData.HPMax = 168.0f;
	bossData.scale = { 1.5f,1.5f,1.5f };
	bossData.r = 10.0f;
	bossData.bossFlag = true;
	bossData.nockPossibleFlag = false;
}

void TwinBoar::Draw(EnemyData* oniData, int i)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case  BOSSATTACK://���ːi
		if (oniData->StatusTime >= sAttackMotionTime - sAttackHoldTime)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= 0)
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else
		{//�ړ���
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	case BOSSATTACK2://�����ːi
		if (attackStats == PREOPERATION)//�U������
		{
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackStats == DOUBLEATTACK)//�U����
		{
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else if (attackStats == DOUBLEAFTER)//�U����̌�
		{
			Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	case BOSSATTACK3://���ݓːi
		if (attackStats == PREOPERATION && bFlag[i] == true && bFinishFlag[i] == false)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackStats == DOUBLEATTACK && bFlag[i] == true && bFinishFlag[i] == false)
		{
			Object::Instance()->Draw(attackOBJ[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
#if _DEBUG
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
#endif
		}
		else
		{
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	}
}

void TwinBoar::LoopInit()
{
	searchEnemyFlag = false;
}

void TwinBoar::SearchPlayer(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	Box enemiesBox = SearchField(oniData);
	oniData->direction = Down;
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[�ƓG�̋���
		float Length = Vec3(player->GetPosition() - oniData->position).length();
		if (Length < player2EnemyLength)
		{//�v���C���[���U�������ɂ�����
			oniData->Status = BOSSATTACK;//���ˌ�
			searchEnemyFlag = true;
			boarNum = boarNumMax;
			sTime = sTimeMax;
			sAttack[0] = false;
			sAttack[1] = false;
		}
	}
}

void TwinBoar::AttackShortRush(EnemyData* oniData, Player* player, int i)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	sTime--;
	//�ړ�
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	if (oniData->StatusTime <= 0 && sTime >= 0)
	{
		//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
		Vec3 memoryPosition = player->GetPosition() - oniData->position;
		//���������߂�
		float length = memoryPosition.length();
		if (length < sPlayer2EnemyLength)
		{
			//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//�v���C���[�̌���
			oniData->pDirection = memoryPosition.normalize();
			oniData->attackDirection = oniData->direction;
			oniData->StatusTime = sAttackMotionTime;
		}
		else
		{
			//�v���C���[�̌���
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
	}
	//�U��
	//�U�����\���鎞��
	if (oniData->StatusTime >= sAttackMotionTime - sAttackHoldTime)
	{

	}
	//�U�����Ԓ�
	else if (oniData->StatusTime > 0)
	{
		//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
		if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
		}
		//�ړ��U��
		oniData->oldPosition = oniData->position;
		slowValue = player->GetSlow();
		oniData->position += oniData->pDirection * sAttackSpeed * slowValue;
	}

	//���Ԃ��I�������ːi���݂��Q�̓����ːi
	oniData->StatusTime--;

	if (oniData->StatusTime <= 0 && sTime <= 0)
	{
		if (fixedPosition[i].x - 5.0f < oniData->position.x && fixedPosition[i].x + 5.0f > oniData->position.x &&
			fixedPosition[i].z - 5.0f < oniData->position.z && fixedPosition[i].z + 5.0f > oniData->position.z)
		{//����̈ʒu�ֈړ�������
			sAttack[i] = true;
		}
		else
		{//����̈ʒu�ֈړ�		
			oniData->bowAngle = atan2(fixedPosition[i].z - oniData->position.z, fixedPosition[i].x - oniData->position.x);
			Vec3 memoryPosition = fixedPosition[i] - oniData->position;
			oniData->pDirection = memoryPosition.normalize();
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveFixedSpeed * slowValue;
		}
	}

	if (sTime <= 0 && sAttack[0] == true && sAttack[1] == true)
	{
		if (attackFlag == true)
		{
			oniData->Status = BOSSATTACK2;
			attackStats = PREOPERATION;
			doublePreTime = doublePreTimeMax;
		}
		else
		{
			oniData->Status = BOSSATTACK3;
			altAttackNum = 0;
			bFlag[0] = false;
			bFlag[1] = false;
			bFinishFlag[0] = false;
			bFinishFlag[1] = false;
		}
		boarNum = boarNumMax;
	}
}


void TwinBoar::AttackRush(EnemyData* oniData, Player* player, int num)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//�U�����s���Ă��Ȃ����ːi���܂����Ă��Ȃ�������
	if (bFlag[num] == false && boarNum > 0 && attackFlag == false)
	{
		attackStats = PREOPERATION;
		bPreTime = bPreTimeMax;
		bFlag[num] = true;
		attackFlag = true;
		oniData->attackDirection = oniData->direction;
	}
	//�U����
	if (bFlag[num] == true && bFinishFlag[num] == false)
	{
		if (attackStats == PREOPERATION)
		{//�U���\������

			bPreTime--;
			if (bPreTime <= 0)
			{
				attackStats = DOUBLEATTACK;
				oniData->attackDirection = oniData->direction;
				//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
				Vec3 memoryPosition = player->GetPosition() - oniData->position;
				//�v���C���[�̌���
				oniData->pDirection = memoryPosition.normalize();
				dAttackTime = dAttackTimeMax;
			}
		}
		else if (attackStats == DOUBLEATTACK)
		{
			//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
			if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(1);
			}
			//�ړ��U��
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
			dAttackTime -= 1;
			if (dAttackTime <= 0)
			{//�ːi���I�������
				attackStats = DOUBLEAFTER;
			}
		}
		else
		{
			boarNum--;
			attackFlag = false;
			bFinishFlag[num] = true;
		}
	}

	//���݂ɓːi�R��I�������
	if (boarNum <= 0 && altAttackNum >= 3)
	{
		oniData->Status = BOSSATTACK;
		sTime = sTimeMax;
		sAttack[0] = false;
		sAttack[1] = false;
		attackFlag = true;
	}
	//���݂ɓːi���R��s�����߂̏���
	if (boarNum <= 0 && altAttackNum < 3)
	{
		altAttackNum++;
		boarNum = boarNumMax;
		bFlag[0] = false;
		bFlag[1] = false;
		bFinishFlag[0] = false;
		bFinishFlag[1] = false;
	}
}

void TwinBoar::AttackDoubleRush(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	if (attackStats == PREOPERATION)
	{//�U���\������
		doublePreTime--;
		//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
		Vec3 memoryPosition = player->GetPosition() - oniData->position;
		//�v���C���[�̌���
		oniData->pDirection = memoryPosition.normalize();
		oniData->attackDirection = oniData->direction;
		if (doublePreTime <= 0)
		{//�������Ԃ��I�������U�����[�h�ɉf��
			attackStats = DOUBLEATTACK;
			doubleTime = doubleTimeMax;
			boarNum = boarNumMax;

		}
	}
	else if (attackStats == DOUBLEATTACK)//�U����
	{
		//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
		if (Collision::CheckBox2Box(oniData->eBox, player->GetBox()) && player->GetInvincivleTime() == 0)
		{
			player->Damage(1);
		}
		if (boarNum == 2)
		{
			//�ړ��U��
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
		}
		doubleTime -= 1;
		if (doubleTime <= 0)
		{//�ːi���I�������
			attackStats = DOUBLEAFTER;
			boarNum--;
			//2�̂Ƃ��ʂ�����
			if (boarNum == 0)
			{
				doubleAfterTime = doubleAfterTimeMax;
			}
		}
	}
	else
	{//�U����̌��̎���2�b

		doubleAfterTime--;
		if (doubleAfterTime <= 0)
		{
			attackFlag = false;
			oniData->Status = BOSSATTACK;
			sTime = sTimeMax;
			sAttack[0] = false;
			sAttack[1] = false;
		}
	}
}

Box TwinBoar::SearchField(EnemyData* oniData)
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

Vec3 TwinBoar::DirectionAngle(int direction)
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
