#include"Enemy.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//���������
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		delete eData[i];
		eData.erase(eData.begin() + i);
	}
}

void Enemy::Init()
{
	char *Filepath = (char *)"Resources/map/stage01spawnMap.csv";

	LoadCSV(spawnMap, Filepath);

	for (size_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (size_t i = 0; i < MAP_WIDTH; i++)
		{
			switch (spawnMap[j][i])
			{
			case ONESPAWN:
				eData.push_back(new EnemyData);
				eData[eData.size() - 1]->enemyObject = Object::Instance()->CreateOBJ("OniKari");
				eData[eData.size() - 1]->position = { basePosition.x + i * mapSize, 0, basePosition.y + j * (-mapSize) };
				eData[eData.size() - 1]->oldPosition = eData[eData.size() - 1]->position;
				//���W�����킹��
				eData[eData.size() - 1]->eBox.minPosition = XMVectorSet(
					eData[eData.size() - 1]->position.x - eData[eData.size() - 1]->r,
					eData[eData.size() - 1]->position.y - eData[eData.size() - 1]->r,
					eData[eData.size() - 1]->position.z - eData[eData.size() - 1]->r, 1);
				eData[eData.size() - 1]->eBox.maxPosition = XMVectorSet(
					eData[eData.size() - 1]->position.x + eData[eData.size() - 1]->r,
					eData[eData.size() - 1]->position.y + eData[eData.size() - 1]->r,
					eData[eData.size() - 1]->position.z + eData[eData.size() - 1]->r, 1);
				break;
			default:
				break;
			}
		}
	}
	debugField = Shape::CreateRect(attackEnemies.x, attackEnemies.y);
	debugField2 = Shape::CreateRect(attackField.x, attackField.y);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
}

void Enemy::Update(Player *player)
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		switch (eData[i]->Status)
		{
		case NORMAL:
			SearchPlayer(i, player);
			break;
		case MOVE:
			eData[i]->direction = Direction(i, player);
			//�ړ�
			Move(i, player);
			break;
		case ATTACK:
			eData[i]->direction = Direction(i, player);
			Attack(i, player);
			break;
		case ENEMIES:
			break;
		}

		if (eData[i]->damegeTime > 0)
		{
			eData[i]->damegeTime--;
		}
	}
	//�폜
	Delete();
}

void Enemy::Draw()
{
	for (size_t i = 0; i < eData.size(); i++)
	{
		if (eData[i]->damegeTime % 2 == 0)
		{
			Object::Instance()->Draw(eData[i]->enemyObject, eData[i]->position, eData[i]->scale, eData[i]->angle, eData[i]->color);
		}
		switch (eData[i]->Status)
		{
		case NORMAL:
			switch (eData[i]->direction)
			{
			case Up:
				Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackEnemies.y / 2),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				break;
			case Down:
				Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackEnemies.y / 2),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				break;
			case Left:
				Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x - attackEnemies.y / 2, eData[i]->position.y, eData[i]->position.z),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				break;
			case Right:
				Object::Instance()->Draw(debugField, Vec3(eData[i]->position.x + attackEnemies.y / 2, eData[i]->position.y, eData[i]->position.z),
					eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				break;
			}
			break;
		case ATTACK:
			switch (eData[i]->attackDirection)
			{
			case Up:
				if (eData[i]->StatusTime == attackMotionDamege)
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackField.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				}
				else
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackField.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color);
				}
				break;
			case Down:
				if (eData[i]->StatusTime == attackMotionDamege)
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackField.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				}
				else
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackField.y / 2),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color);
				}
				break;
			case Left:
				if (eData[i]->StatusTime == attackMotionDamege)
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x - attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				}
				else
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x - attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color);
				}
				break;
			case Right:
				if (eData[i]->StatusTime == attackMotionDamege)
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x + attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color, redColor);
				}
				else
				{
					Object::Instance()->Draw(debugField2, Vec3(eData[i]->position.x + attackField.y / 2, eData[i]->position.y, eData[i]->position.z),
						eData[i]->scale, Vec3(90.0f, 0.0f, 0.0f), eData[i]->color);
				}
				break;
			}
			break;
		}
	}
}


void Enemy::WasAttack(int i)
{
	//eData[i]->wasAttackFlag = true;
}

void Enemy::SetPosition(int i, Vec3 position)
{
	eData[i]->position = position;
}

void Enemy::DamegeNormal(int i)
{
	eData[i]->HP -= 2;
	eData[i]->damegeTime = 10;
}

void Enemy::Move(int i, Player *player)
{
	eData[i]->oldPosition = eData[i]->position;

	//�v���C���[�ƃG�l�~�[�̈ʒu�̍�
	Vec3 memoryPosition = player->GetPosition() - eData[i]->position;
	//���������߂�
	float length = memoryPosition.length();

	if (length < 5.0f)
	{
		eData[i]->Status = ATTACK;
		eData[i]->attackDirection = eData[i]->direction;
		eData[i]->StatusTime = attackMotionTime;
	}
	else
	{
		//�v���C���[�̌���
		Vec3 direction = memoryPosition.normalize();
		eData[i]->position += direction * eData[i]->speed;

		//���W�����킹��
		eData[i]->eBox.minPosition = XMVectorSet(eData[i]->position.x - eData[i]->r, eData[i]->position.y - eData[i]->r, eData[i]->position.z - eData[i]->r, 1);
		eData[i]->eBox.maxPosition = XMVectorSet(eData[i]->position.x + eData[i]->r, eData[i]->position.y + eData[i]->r, eData[i]->position.z + eData[i]->r, 1);
		eData[i]->eSphere.center = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z, 1);
	}
}

void Enemy::SearchPlayer(int i, Player *player)
{
	Box enemiesBox = SearchField(i);
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[�ƓG�̋���
		float Length = Vec3(player->GetPosition() - eData[i]->position).length();

		if (Length > player2EnemyLength)
		{//�v���C���[���U�������ɂ��Ȃ�������ړ�
			eData[i]->Status = MOVE;
			eData[i]->StatusTime = moveTime;
		}
		else
		{//�v���C���[���U�������ɂ�����
			eData[i]->Status = ATTACK;
			eData[i]->attackDirection = eData[i]->direction;
			eData[i]->StatusTime = attackMotionTime;
		}
	}
}

void Enemy::Attack(int i, Player *player)
{
	Box attackBox = AttackField(i);
	//�U�����[�V�������̃_���[�W��^����^�C�~���O
	if (eData[i]->StatusTime == attackMotionDamege)
	{
		//�U���͈͓��ɂ�����v���C���[�Ƀ_���[�W
		if (Collision::CheckBox2Box(attackBox, player->GetBox()))
		{
			player->Damage();
		}
	}
	//���Ԃ��I���������G�ɂ��ǂ�
	eData[i]->StatusTime--;
	if (eData[i]->StatusTime <= 0)
	{
		eData[i]->Status = MOVE;
	}
}

void Enemy::Delete()
{
	//�G��������
	for (int i = (int)eData.size() - 1; i >= 0; i--)
	{
		if (eData[i]->HP <= 0)
		{
			delete eData[i];
			eData.erase(eData.begin() + i);
		}
	}
}

Box Enemy::SearchField(int i)
{
	Box enemiesBox;
	switch (eData[i]->direction)
	{
	case Up:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z + attackEnemies.y, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		break;
	case Left:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.y, eData[i]->position.y, eData[i]->position.z - attackEnemies.x / 2, 1);
		break;
	case Right:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.y, eData[i]->position.y, eData[i]->position.z + attackEnemies.x / 2, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackEnemies.x / 2, 1);
		break;
	case Down:
		enemiesBox.maxPosition = XMVectorSet(eData[i]->position.x + attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		enemiesBox.minPosition = XMVectorSet(eData[i]->position.x - attackEnemies.x / 2, eData[i]->position.y, eData[i]->position.z - attackEnemies.y, 1);
		break;
	default:
		enemiesBox.maxPosition = {};
		enemiesBox.minPosition = {};
	}
	return enemiesBox;
}

Box Enemy::AttackField(int i)
{
	Box attackBox;
	switch (eData[i]->attackDirection)
	{
	case Up:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.x / 2, eData[i]->position.y, eData[i]->position.z + attackField.y, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		break;
	case Left:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.y, eData[i]->position.y, eData[i]->position.z - attackField.x / 2, 1);
		break;
	case Right:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.y, eData[i]->position.y, eData[i]->position.z + attackField.x / 2, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x, eData[i]->position.y, eData[i]->position.z - attackField.x / 2, 1);
		break;
	case Down:
		attackBox.maxPosition = XMVectorSet(eData[i]->position.x + attackField.x / 2, eData[i]->position.y, eData[i]->position.z, 1);
		attackBox.minPosition = XMVectorSet(eData[i]->position.x - attackField.x / 2, eData[i]->position.y, eData[i]->position.z - attackField.y, 1);
		break;
	default:
		attackBox.maxPosition = {};
		attackBox.minPosition = {};
	}
	return attackBox;
}

int Enemy::Direction(int i, Player *player)
{
	int direction = Right;

	float X = eData[i]->position.x - player->GetPosition().x;
	if (X < 0)
	{
		X *= -1;
	}
	float Z = eData[i]->position.z - player->GetPosition().z;
	if (Z < 0)
	{
		Z *= -1;
	}

	if (X > Z)
	{
		if (eData[i]->position.x < player->GetPosition().x)
		{
			return Right;
		}
		else
		{
			return Left;
		}
	}
	else
	{
		if (eData[i]->position.z < player->GetPosition().z)
		{
			return Up;
		}
		else
		{
			return Down;
		}
	}
}
