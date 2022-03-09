#include"PushCollision.h"

void PushCollision::Player2Mapchip(Player *player, MapStage *mapStage)
{
	if (player == nullptr || mapStage == nullptr)
	{
		return;
	}

	for (int j = 0; j < MAP_HEIGHT; j++)
	{
		for (int i = 0; i < MAP_WIDTH; i++)
		{
			if (mapStage->GetMap(i, j) == BLOCK)
			{
				bool HitFlag = Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j));

				if (HitFlag)
				{
					player->PushBlock(mapStage->GetPosition(i, j), mapStage->GetSize() / 2, mapStage->GetMap(i,(j + 1) % MAP_HEIGHT), mapStage->GetMap(i,(j - 1) % MAP_HEIGHT));
				}
			}
		}
	}
}
