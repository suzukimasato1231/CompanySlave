#include "TwinBoar.h"
#include"Player.h"
#include"Shape.h"
TwinBoar::TwinBoar()
{}

TwinBoar::~TwinBoar()
{}

void TwinBoar::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	//attackBigOBJ = Shape::CreateRect(attackBigField.x, attackBigField.y);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//�ʏ���
	enemyObject[0] = Object::Instance()->CreateOBJ("OniKari1-0", "OniOBJ/");
	enemyObject[1] = Object::Instance()->CreateOBJ("OniKari1-1", "OniOBJ/");
	enemyObject[2] = Object::Instance()->CreateOBJ("OniKari1-2", "OniOBJ/");
	//�U�����[�V����
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1", "OniOBJ/");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2", "OniOBJ/");
	//�m�b�N�o�b�N
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("OniNockback1");
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("OniNockback2");

	//�{�X�̐��l
	bossData.HP = 168.0f;
	bossData.HPMax = 168.0f;
	bossData.scale = { 4.0f,4.0f,4.0f };
	bossData.r = 15.0f;
	bossData.bossFlag = true;
	bossData.nockPossibleFlag = false;
}

void TwinBoar::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case  BOSSATTACK://���ːi
		if (oniData->StatusTime >= sAttackMotionTime)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (oniData->StatusTime >= sAttackHoldTime)
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//�ړ���
			Object::Instance()->Draw(enemyObject[oniData->walkNum], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	case BOSSATTACK2://�����ːi
		if (attackBigStatus == PREOPERATION)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
			Object::Instance()->Draw(attackBigOBJ, oniData->position, Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
		else
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	case BOSSATTACK3://���ݓːi
		if (attackBigStatus == PREOPERATION)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
			Object::Instance()->Draw(attackBigOBJ, oniData->position, Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
		else
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		break;
	case NOCKBACK:
		if (oniData->nockbackTime >= 3)
		{
			Object::Instance()->Draw(nockBackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		else
		{
			Object::Instance()->Draw(nockBackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
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
		}
	}
}

void TwinBoar::AttackShortRush(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	//�ړ�
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	if (oniData->StatusTime <= 0)
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
	else
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

	if (oniData->StatusTime == 0)
	{
		if (attackFlag == true)
		{
			oniData->Status = BOSSATTACK2;
			attackStats = PREOPERATION;
		}
		else
		{
			oniData->Status = BOSSATTACK3;
			altAttackNum = 0;
		}
		boarNum--;
		oniData->StatusTime = -1;
		if (boarNum <= 0)
		{//�ǂ����̍U�����؂�ւ���
			if (attackFlag == true)
			{
				attackFlag = false;
			}
			else
			{
				attackFlag = true;
			}
			boarNum = boarNumMax;
			d_start = time(NULL);
		}
	}
}


void TwinBoar::AttackRush(EnemyData* oniData, Player* player, int num)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//�U�����s���Ă��Ȃ����ːi���܂����Ă��Ȃ�������
	if (altFlag == false && boarNum == num + 1)
	{
		altFlag = true;
		attackStats = PREOPERATION;
		d_start = time(NULL);
	}
	//�U����
	if (altFlag == true && boarNum == num + 1)
	{
		if (attackStats == PREOPERATION)
		{//�U���\������
			d_end = time(NULL);

			int dTime = d_end - d_start;
			if (dTime >= 1)
			{
				attackStats = DOUBLEATTACK;
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
				player->Damage(3);
			}
			//�ړ��U��
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
			dAttackTime -= 1;
			if (dAttackTime <= 0)
			{//�ːi���I�������
				attackStats = DOUBLEAFTER;
				d_start = time(NULL);
			}
		}
		else
		{
			altFlag = false;
			boarNum--;
		}
	}

	//���݂ɓːi�R��I�������
	if (altFlag == false && boarNum == 0 && altAttackNum >= 3)
	{
		oniData->Status = BOSSATTACK;
	}
	//���݂ɓːi���R��s�����߂̏���
	if (altFlag == false && boarNum == 0)
	{
		altAttackNum++;
		boarNum = boarNumMax;
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
		d_end = time(NULL);

		int dTime = d_end - d_start;
		if (dTime >= 2)
		{
			attackStats = DOUBLEATTACK;
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
			player->Damage(2);
		}
		//�ړ��U��
		oniData->oldPosition = oniData->position;
		slowValue = player->GetSlow();
		oniData->position += oniData->pDirection * dAttackSpeed * slowValue;
		dAttackTime -= 1;
		if (dAttackTime <= 0)
		{//�ːi���I�������
			attackStats = DOUBLEAFTER;
			d_start = time(NULL);
		}
	}
	else
	{//�U����̌��̎���2�b
		d_end = time(NULL);
		int afterTime = d_end - d_start;

		if (afterTime >= 2)
		{
			oniData->Status = BOSSATTACK;
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
