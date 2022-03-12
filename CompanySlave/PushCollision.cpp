#include"PushCollision.h"

void PushCollision::Player2Mapchip(Player *player, MapStage *mapStage)
{
	if (player == nullptr || mapStage == nullptr) { return; }

	for (int j = 0; j < MAP_HEIGHT; j++)
	{
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			if (mapStage->GetMap(i, j) == BLOCK)
			{
				//�v���C���[
				bool HitFlag = Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j));
				if (HitFlag)
				{
					player->SetPosition(PushBack(player->GetPosition(),player->GetOldPosition(),player->GetPSize(),
						mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
						mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
				}
				//�G
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
		if (up == BLOCK)
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
		if (down == BLOCK)
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
		if (up == BLOCK)
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
		if (down == BLOCK)
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


