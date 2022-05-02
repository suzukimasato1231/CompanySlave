#include "BigOniBoss.h"
#include"Player.h"
#include"Shape.h"
BigOniBoss::BigOniBoss()
{}

BigOniBoss::~BigOniBoss()
{}

void BigOniBoss::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);
	attackBigOBJ = Shape::CreateRect(attackBigField.x, attackBigField.y);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	//�ʏ���
	enemyObject = Object::Instance()->CreateOBJ("OniKari");
	//�U�����[�V����
	attackOBJ[0] = Object::Instance()->CreateOBJ("OniKari2-1");
	attackOBJ[1] = Object::Instance()->CreateOBJ("OniKari2-2");
	//�m�b�N�o�b�N
	nockBackOBJ[0] = Object::Instance()->CreateOBJ("OniNockback1");
	nockBackOBJ[1] = Object::Instance()->CreateOBJ("OniNockback2");

	//�U���G�t�F�N�g
	//AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect9.png");
	//�{�X�̐��l
	bossData.HP = 84.0f;
	bossData.HPMax = 84.0f;
	bossData.scale = { 4.0f,4.0f,4.0f };
	bossData.r = 15;
	bossData.bossFlag = true;
}

void BigOniBoss::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
#if _DEBUG
		if (oniData->direction == Up) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z + attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Down) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x, 0.0f, oniData->position.z - attackEnemies.y / 2),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Left) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x - attackEnemies.y / 2, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
		else if (oniData->direction == Right) {
			Object::Instance()->Draw(debugField, Vec3(oniData->position.x + attackEnemies.y / 2, 0.0f, oniData->position.z),
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
		}
#endif
	case  BOSSATTACK:
		if (attackSmallTime >= attackMotionDamege)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackSmallTime > 0)
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//�ړ���
			Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
#if _DEBUG
		if (attackSmallTime >= attackMotionDamege)
		{
			switch (oniData->attackDirection)
			{
			case Up:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, 0.0f, oniData->position.z + attackField.y / 2),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
				break;
			case Down:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, 0.0f, oniData->position.z - attackField.y / 2),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
				break;
			case Left:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x - attackField.y / 2, 0.0f, oniData->position.z),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
				break;
			case Right:
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x + attackField.y / 2, 0.0f, oniData->position.z),
					Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 90.0f, 0.0f), oniData->color, redColor);
				break;
			}
		}
#endif
		break;
	case BOSSATTACK2:
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
	case SUMMON:
		if (attackSmallTime >= attackMotionDamege)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else if (attackSmallTime > 0)
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//�ړ���
			Object::Instance()->Draw(enemyObject, Vec3(oniData->position.x, oniData->position.y + 15.0f, oniData->position.z), oniData->scale, DirectionAngle(oniData->direction), oniData->color);
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

void BigOniBoss::DrawHP(EnemyData* oniData)
{



}


void BigOniBoss::SearchPlayer(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	Box enemiesBox = SearchField(oniData);
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[�ƓG�̋���
		float Length = Vec3(player->GetPosition() - oniData->position).length();
		if (Length < player2EnemyLength)
		{//�v���C���[���U�������ɂ�����
			oniData->Status = BOSSATTACK;//���Ō�
			attackSmallNum = 0;			//���U���񐔃��Z�b�g
			attackSmallTime = 0;

		}
	}
}

void BigOniBoss::AttackSmall(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	//�j�ɂȂ�����U������߂�
	if (oniData->explosionFlag == true)
	{
		attackSmallTime = 0;
	}

	//�v���C���[�ƓG�̋���
	float Length = Vec3(player->GetPosition() - oniData->position).length();
	//�U�������Ԃ���Ȃ������炱�̏������s��
	if (attackSmallTime <= 0 && attackSmallNum < 3)
	{
		if (Length > attackSmallLength)
		{//�U�����邽�߂Ƀv���C���[�ɋ߂Â�����
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//���������߂�
			float length = memoryPosition.length();
			//�v���C���[�̌���
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
		else
		{
			//�˒��͈͂Ƀv���C���[������̂ōU�����鏈��	
			attackSmallNum++;
			attackSmallTime = attackSmallTimeMax;
			oniData->attackDirection = oniData->direction;

		}
	}
	//�U����
	if (attackSmallTime > 0)
	{
		Box attackBox = AttackField(oniData);
		//�U�����[�V�������̃_���[�W��^����^�C�~���O
		if (attackSmallTime == attackMotionDamege)
		{
			//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
			if (Collision::CheckBox2Box(attackBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(2);
			}
		}
		attackSmallTime--;
		if (attackSmallTime <= 0 && attackSmallNum == 3)
		{
			start_time = time(NULL);//�v���J�n
		}

		//�U����2��܂��͔j�����������Ƃ��U������߂�
		if (oniData->explosionFlag == true)
		{
			oniData->nockbackTime = EnemySupport::nockBackTimeMax;
		}
	}

	//3�񏬍U�����I�����玟�̍U���ֈڂ�
	if (attackSmallNum == 3 && attackSmallTime <= 0)//�U�����Ԃ��O�ɂȂ�����
	{
		end_time = time(NULL);
		int smallTime = end_time - start_time;//���Ԍv�Z
		if (smallTime > 3)
		{
			attackSmallNum = 0;
			if (attackTypeFlag == true)
			{//��U����
				oniData->Status = BOSSATTACK2;
				attackBigStatus = PREOPERATION;
				big_start = time(NULL);
				oniData->nockPossibleFlag = false;
				attackTypeFlag = false;
			}
			else
			{//�����U����
				oniData->Status = SUMMON;
				firstSummonFlag = true;
				attackTypeFlag = true;
			}
		}
	}
}

void BigOniBoss::AttackBig(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	if (attackBigStatus == PREOPERATION)
	{
		big_end = time(NULL);
		int bigTime = big_end - big_start;
		//�V�b�o������U��
		if (bigTime >= 7)
		{
			attackBigStatus = BIGATTACK;
		}
	}
	else if (attackBigStatus == BIGATTACK)
	{
		//�����蔻��
		if (Collision::CheckBox2Box(player->GetBox(), AttackBIG(oniData)))
		{
			player->Damage(4);
		}
		start_time = time(NULL);//�v���J�n
		attackBigStatus = BIGAFTER;
	}
	else if (attackBigStatus == BIGAFTER)
	{
		//�U������������玟�̍U���ֈڂ�
		end_time = time(NULL);
		int nextTime = end_time - start_time;
		if (nextTime >= 3)
		{
			oniData->nockPossibleFlag = true;
			oniData->Status = BOSSATTACK;
		}
	}
}

void BigOniBoss::AttackSummon(EnemyData* oniData, Player* player, int eNum)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	//�ŏ�������������
	if (firstSummonFlag == true)
	{
		firstSummonFlag = false;
	}

	//�v���C���[�ƓG�̋���
	float Length = Vec3(player->GetPosition() - oniData->position).length();
	//�U�������Ԃ���Ȃ������炱�̏������s��
	if (attackSmallTime <= 0)
	{
		if (Length > attackSmallLength)
		{//�U�����邽�߂Ƀv���C���[�ɋ߂Â�����
			oniData->oldPosition = oniData->position;
			slowValue = player->GetSlow();
			//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
			Vec3 memoryPosition = player->GetPosition() - oniData->position;
			//���������߂�
			float length = memoryPosition.length();
			//�v���C���[�̌���
			Vec3 direction = memoryPosition.normalize();
			oniData->position += direction * moveSpeed * slowValue;
		}
		else
		{
			//�˒��͈͂Ƀv���C���[������̂ōU�����鏈��	
			attackSmallTime = attackSmallTimeMax;
			oniData->attackDirection = oniData->direction;

		}
	}
	//�U����
	if (attackSmallTime > 0)
	{
		Box attackBox = AttackField(oniData);
		//�U�����[�V�������̃_���[�W��^����^�C�~���O
		if (attackSmallTime == attackMotionDamege)
		{
			//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
			if (Collision::CheckBox2Box(attackBox, player->GetBox()) && player->GetInvincivleTime() == 0)
			{
				player->Damage(2);
			}
		}
		attackSmallTime--;
	}

	//�q�������Ȃ��Ȃ����玟�̍U���ֈڂ�
	if (eNum <= 1 && attackSmallTime <= 0)
	{
		oniData->Status = BOSSATTACK;
	}
}


void BigOniBoss::EffectDraw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	/*if (AttackEffect == true)
	{
		switch (oniData->attackDirection)
		{
		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + oniData->r + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - oniData->r - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - oniData->r - AttackEffectSize, oniData->position.y, oniData->position.z), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + oniData->r + AttackEffectSize, oniData->position.y, oniData->position.z), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + AttackEffectSize, oniData->position.y, oniData->position.z + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - AttackEffectSize, oniData->position.y, oniData->position.z + AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x + AttackEffectSize, oniData->position.y, oniData->position.z - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);

			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(oniData->position.x - AttackEffectSize, oniData->position.y, oniData->position.z - AttackEffectSize), AttackScale, AttackAngle, oniData->color, AttackEffectGraph[effectCount]);
			break;
		}
	}*/
}

Box BigOniBoss::SearchField(EnemyData* oniData)
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

Vec3 BigOniBoss::DirectionAngle(int direction)
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

Box BigOniBoss::AttackField(EnemyData* oniData)
{
	Box attackBox;
	switch (oniData->attackDirection)
	{
	case Up:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.x / 2, oniData->position.y, oniData->position.z + attackField.y, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.x / 2, oniData->position.y, oniData->position.z, 1);
		break;
	case Left:
		attackBox.maxPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.y, oniData->position.y, oniData->position.z - attackField.x / 2, 1);
		break;
	case Right:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.y, oniData->position.y, oniData->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x, oniData->position.y, oniData->position.z - attackField.x / 2, 1);
		break;
	case Down:
		attackBox.maxPosition = XMVectorSet(oniData->position.x + attackField.x / 2, oniData->position.y, oniData->position.z, 1);
		attackBox.minPosition = XMVectorSet(oniData->position.x - attackField.x / 2, oniData->position.y, oniData->position.z - attackField.y, 1);
		break;
	default:
		attackBox.maxPosition = {};
		attackBox.minPosition = {};
	}
	return attackBox;
}

Box BigOniBoss::AttackBIG(EnemyData* oniData)
{
	Box attackBox;
	attackBox.maxPosition = XMVectorSet(oniData->position.x + attackBigField.x / 2, oniData->position.y, oniData->position.z + attackBigField.y, 1);
	attackBox.minPosition = XMVectorSet(oniData->position.x - attackBigField.x / 2, oniData->position.y, oniData->position.z - attackBigField.y, 1);
	return attackBox;
}
