#include"PushCollision.h"
#include<time.h>

void PushCollision::Player2Mapchip(class Player *player, class Enemy *enemy, class MapStage *mapStage, bool &stageFlag)
{
	srand(time(NULL));
	if (player == nullptr || mapStage == nullptr) { return; }
	//判定する箇所だけ行うため
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
			if (!(mapStage->GetMap(i, j) == NONE))//0以外当たり判定
			{
				//プレイヤー
				bool HitFlag = Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j));
				if (HitFlag)
				{
					player->SetPosition(PushBack(player->GetPosition(), player->GetOldPosition(), player->GetPSize(),
						mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
						mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
				}
			}
			//このブロックに触れたら次のステージへ
			if (mapStage->GetMap(i, j) == NextStageBlock)
			{
				if (Collision::CheckBox2Box(player->GetBox(), mapStage->GetPositionBlock(i, j)))
				{
					stageFlag = true;
				}
			}
		}
	}
	//敵
	for (int n = 0; n < enemy->GetEnemySize(); n++)
	{
		X = player->GetPosition().x / mapStage->GetSize();
		Z = player->GetPosition().z / (-mapStage->GetSize());
		for (int j = (X - 2); j < (X + 2); j++)
		{
			for (int i = (Z - 2); i < (Z + 2); i++)
			{
				if (j < 0 || i < 0 || j >= MAP_HEIGHT || i >= MAP_WIDTH)
				{
					continue;
				}
				if (!(mapStage->GetMap(i, j) == NONE))//0以外当たり判定
				{
					bool HitFlag = Collision::CheckBox2Box(enemy->GetBox(n), mapStage->GetPositionBlock(i, j));
					if (HitFlag)
					{
						enemy->SetPosition(n, PushBack(enemy->GetPosition(n), enemy->GetOldPosition(n), enemy->GetEnemyR(n),
							mapStage->GetPosition(i, j), mapStage->GetSize() / 2,
							mapStage->GetMap(i, (j + 1) % MAP_HEIGHT), mapStage->GetMap(i, (j - 1) % MAP_HEIGHT)));
					}
				}
			}
		}
	}

	//剣
	for (int n = 0; n < 7; n++)
	{
		//判定する箇所だけ行うため
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
				if (!(mapStage->GetMap(i, j) == NONE))//0以外当たり判定
				{
					//プレイヤー
					bool HitFlag = Collision::CheckBox2Box(player->GetSwordBox(n), mapStage->GetPositionBlock(i, j));
					if (HitFlag)
					{
						float reverseWallAngle = rand() % 78 - 39;
						float b = reverseWallAngle / 100;
						player->SetSwordReverse(3.141592 +  b, n);
						player->SetSwordStop(n);
					}
				}
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
		if (!(up == NONE))
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
		if (!(down == NONE))
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
		if (!(up == NONE))
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
		if (!(down == NONE))
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


