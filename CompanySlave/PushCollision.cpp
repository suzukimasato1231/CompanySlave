#include"PushCollision.h"

void PushCollision::Player2Mapchip(class Player* player, class Enemy* enemy, class MapStage* mapStage, bool& changeFlag)
{

	if (player == nullptr || mapStage == nullptr) { return; }
	//���肷��ӏ������s������
	int X = player->GetPosition().x / mapStage->GetSize();
	int Z = player->GetPosition().z / (-mapStage->GetSize());

	for (int j = (Z - 2); j < (Z + 2); j++)
	{
		for (int i = (X - 2); i < (X + 2); i++)
		{
			if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
			{
				continue;
			}
			if (!(mapStage->GetMap(i, j) == NONE))//0�ȊO�����蔻��
			{
				//�v���C���[
				bool HitFlag = Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j));
				if (HitFlag)
				{
					player->SetPosition(PushBack(player->GetPosition(), player->GetOldPosition(), player->GetPSize(),
						mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
						mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
				}
			}
			//���̃u���b�N�ɐG�ꂽ�玟�̃X�e�[�W��
			if (mapStage->GetMap(i, j) == NextStageBlock)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					changeFlag = true;
				}
			}
			if (mapStage->GetMapGround(i, j) == FLOOR_TILE1 || mapStage->GetMapGround(i, j) == FLOOR_TILE2)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					mapStage->SetfloorNum(0);
				}
			}
			if (mapStage->GetMapGround(i, j) == FLOOR_TILE3 || mapStage->GetMapGround(i, j) == FLOOR_TILE4)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					mapStage->SetfloorNum(1);
				}
			}
			if (mapStage->GetMapGround(i, j) == FLOOR_TILE7 || mapStage->GetMapGround(i, j) == FLOOR_TILE8)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					mapStage->SetfloorNum(2);
				}
			}
			if (mapStage->GetMapGround(i, j) == FLOOR_TILE9 || mapStage->GetMapGround(i, j) == FLOOR_TILE10)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					mapStage->SetfloorNum(3);
				}
			}
		}
	}
	//�G
	for (int n = 0; n < enemy->GetEnemySize(); n++)
	{
		X = enemy->GetPosition(n).x / mapStage->GetSize();
		Z = enemy->GetPosition(n).z / (-mapStage->GetSize());
		for (int j = (Z - 2); j < (Z + 2); j++)
		{
			for (int i = (X - 2); i < (X + 2); i++)
			{
				if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
				{
					continue;
				}
				if (!(mapStage->GetMap(i, j) == NONE))//0�ȊO�����蔻��
				{
					bool HitFlag = Collision::CheckBox2Box(enemy->GetBox(n), mapStage->GetPositionBlock(i, j));
					if (HitFlag)
					{
						enemy->SetPosition(n, PushBack(enemy->GetPosition(n), enemy->GetOldPosition(n), 5.0f,
							mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
							mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
					}
				}
			}
		}
		//��̓����蔻��
		if (enemy->GetStatus(n) == OniBow)
		{
			X = enemy->GetBowPosition(n).x / mapStage->GetSize();
			Z = enemy->GetBowPosition(n).z / (-mapStage->GetSize());
			for (int j = (Z - 2); j < (Z + 2); j++)
			{
				for (int i = (X - 2); i < (X + 2); i++)
				{
					if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
					{
						continue;
					}
					if (!(mapStage->GetMap(i, j) == NONE))//0�ȊO�����蔻��
					{
						OBB mapOBB;
						mapOBB.Initilize(mapStage->GetPosition(i, j), Vec3(), Vec3(mapStage->GetSize(), mapStage->GetSize(), mapStage->GetSize()));
						OBB bowOBB;
						bowOBB.Initilize(enemy->GetBowPosition(n), Vec3(0.0f, 0.0f, enemy->GetBowAngle(n)), Vec3(10.0f, 10.0f, 10.0f));
						bool HitFlag = OBBCollision::ColOBBs(bowOBB, mapOBB);
						if (HitFlag)
						{
							enemy->SetBowFlag(n);
						}
					}
				}
			}
		}
	}

	//��
	for (int n = 0; n < 7; n++)
	{
		//���肷��ӏ������s������
		int X = player->GetSwordPosition(n).x / mapStage->GetSize();
		int Z = player->GetSwordPosition(n).z / (-mapStage->GetSize());

		for (int j = (Z - 2); j < (Z + 2); j++)
		{
			for (int i = (X - 2); i < (X + 2); i++)
			{
				if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
				{
					continue;
				}
				if (!(mapStage->GetMap(i, j) == NONE))//0�ȊO�����蔻��
				{
					//�v���C���[
					bool HitFlag = Collision::CheckBox2Box(player->GetSwordBox(n), mapStage->GetPositionBlock(i, j));
					if (HitFlag)
					{
						player->SetSwordReverse(3.141592, n);
						player->SetSwordStop(n);
					}
				}
			}
		}
	}
}


Vec3 PushCollision::PushBack(Vec3 pos, Vec3 oldPos, float size, Vec3 BPos, float blockSize, const int up, const int down)
{
	//�v���C���[���W
	float PRight = pos.x + size, PLeft = pos.x - size;
	float PUp = pos.z + size, PDown = pos.z - size;
	//�v���C���[�̑O�̍��W
	float oldRight = oldPos.x + size, oldLeft = oldPos.x - size;
	float oldUp = oldPos.z + size, oldDown = oldPos.z - size;
	//�u���b�N���W
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;
	//����
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (!(up == NONE))
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
	}
	//����
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (!(down == NONE))
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (!(up == NONE))
		{
		}
		//��Ƀu���b�N���Ȃ��������D��
		else if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
	}
	//�E��
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (!(down == NONE))
		{
		}
		//���Ƀu���b�N���Ȃ������牺�D��
		else if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}
	else
	{
		//�E
		if (oldRight <= BLeft)
		{
			pos.x = BLeft - size;
		}
		//��
		if (oldLeft >= BRight)
		{
			pos.x = BRight + size;
		}
		//��
		if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
		//��
		if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}

	return pos;
}


