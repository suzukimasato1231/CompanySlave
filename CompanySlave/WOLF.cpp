#include "WOLF.h"
#include"Player.h"
#include"Shape.h"
Wolf::Wolf()
{}

Wolf::~Wolf()
{}

void Wolf::Init()
{
	debugField = Shape::CreateRect(attackEnemies.y, attackEnemies.x);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);

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
	AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/Eeffect9.png");
}

void Wolf::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		return;
	}
	switch (oniData->Status)
	{
	case NORMAL:
		Object::Instance()->Draw(enemyObject, oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
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
		Object::Instance()->Draw(enemyObject, oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		break;
	case ATTACK:
		if (oniData->StatusTime >= attackMotionDamege)
		{//����U��グ
			Object::Instance()->Draw(attackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		else
		{//����U�艺�낵
			Object::Instance()->Draw(attackOBJ[1], oniData->position, oniData->scale, DirectionAngle(oniData->attackDirection), oniData->color);
		}
		EffectDraw(oniData);
		switch (oniData->attackDirection)
		{
		case Up:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, oniData->position.y, oniData->position.z + attackField.y / 2),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Down:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x, oniData->position.y, oniData->position.z - attackField.y / 2),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Left:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x - attackField.y / 2, oniData->position.y, oniData->position.z),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		case Right:
			if (oniData->StatusTime == attackMotionDamege)
			{
				Object::Instance()->Draw(debugField2, Vec3(oniData->position.x + attackField.y / 2, oniData->position.y, oniData->position.z),
					oniData->scale, Vec3(90.0f, 0.0f, 0.0f), oniData->color, redColor);
			}
			break;
		}
		break;
	case NOCKBACK:
		if (oniData->nockbackTime >= 3)
		{
			Object::Instance()->Draw(nockBackOBJ[0], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		else
		{
			Object::Instance()->Draw(nockBackOBJ[1], oniData->position, oniData->scale, DirectionAngle(oniData->direction), oniData->color);
		}
		break;
	}
}

void Wolf::Move(EnemyData* oniData, Player* player)
{
	oniData->oldPosition = oniData->position;
	slowValue = player->GetSlow();
	//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
	Vec3 memoryPosition = player->GetPosition() - oniData->position;
	//���������߂�
	float length = memoryPosition.length();

	if (length < player2EnemyLength)
	{
		oniData->Status = ATTACK;
		oniData->attackDirection = oniData->direction;
		oniData->StatusTime = attackMotionTime;
	}
	else
	{
		//�v���C���[�̌���
		Vec3 direction = memoryPosition.normalize();
		oniData->position += direction * oniData->speed * slowValue;
	}
}

void Wolf::SearchPlayer(EnemyData* oniData, Player* player)
{
	Box enemiesBox = SearchField(oniData);
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[�ƓG�̋���
		float Length = Vec3(player->GetPosition() - oniData->position).length();

		if (Length > player2EnemyLength)
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

void Wolf::Attack(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		return;
	}

	Box attackBox = AttackField(oniData);
	//�U�����[�V�������̃_���[�W��^����^�C�~���O
	if (oniData->StatusTime == attackMotionDamege)
	{
		//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
		if (Collision::CheckBox2Box(attackBox, player->GetBox()))
		{
			//�G�t�F�N�g�̃t���OTRUE
			AttackEffect = true;
			player->Damage();
		}
	}
	//�G�t�F�N�g�֌W
	if (AttackEffect == true) {

		if (effectCount < 8) {
			effectCount++;
		}
		else if (effectCount == 8) {
			effectCount = 0;
			AttackEffect = false;
		}
	}
	//���Ԃ��I���������G�ɂ��ǂ�
	oniData->StatusTime--;
	if (oniData->StatusTime <= 0)
	{
		oniData->Status = MOVE;
	}
}

void Wolf::EffectDraw(EnemyData* oniData)
{
	if (AttackEffect == true)
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
	}
}

Box Wolf::SearchField(EnemyData* oniData)
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

Vec3 Wolf::DirectionAngle(int direction)
{
	Vec3 angle = {};
	switch (direction)
	{
	case Up:
		angle = { 0.0f,90.0f,0.0f };
		break;
	case Left:
		angle = { 0.0f,0.0f,0.0f };
		break;
	case Right:
		angle = { 0.0f,180.0f,0.0f };
		break;
	case Down:
		angle = { 0.0f,270.0f,0.0f };
		break;
	default:
		angle = { 0.0f,0.0f,0.0f };
	}
	return angle;
}

Box Wolf::AttackField(EnemyData* oniData)
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