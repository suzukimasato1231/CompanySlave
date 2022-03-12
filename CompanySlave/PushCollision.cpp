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
				//プレイヤー
				bool HitFlag = Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j));
				if (HitFlag)
				{
					player->SetPosition(PushBack(player->GetPosition(),player->GetOldPosition(),player->GetPSize(),
						mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
						mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
				}
				//敵
			}
		}
	}
}

Vec3 PushCollision::PushBack(Vec3 pos, Vec3 oldPos, float size, Vec3 BPos, float blockSize, const int up, const int down)
{
	//プレイヤー座標
	float PRight = pos.x + size, PLeft = pos.x - size;
	float PUp = pos.z + size, PDown = pos.z - size;
	//プレイヤーの前の座標
	float oldRight = oldPos.x + size, oldLeft = oldPos.x - size;
	float oldUp = oldPos.z + size, oldDown = oldPos.z - size;
	//ブロック座標
	float BRight = BPos.x + blockSize, BLeft = BPos.x - blockSize;
	float BUp = BPos.z + blockSize, BDown = BPos.z - blockSize;
	//左上
	if (oldLeft >= BRight && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
	}
	//左下
	else if (oldLeft >= BRight && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}
	//右上
	else if (oldRight <= BLeft && oldUp <= BDown)
	{
		if (up == BLOCK)
		{
		}
		//上にブロックがなかったら上優先
		else if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
	}
	//右下
	else if (oldRight <= BLeft && oldDown >= BUp)
	{
		if (down == BLOCK)
		{
		}
		//下にブロックがなかったら下優先
		else if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}
	else
	{
		//右
		if (oldRight <= BLeft)
		{
			pos.x = BLeft - size;
		}
		//左
		if (oldLeft >= BRight)
		{
			pos.x = BRight + size;
		}
		//上
		if (oldUp <= BDown)
		{
			pos.z = BDown - size;
		}
		//下
		if (oldDown >= BUp)
		{
			pos.z = BUp + size;
		}
	}

	return pos;
}


