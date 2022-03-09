#pragma once
#include"Player.h"
#include"MapStage.h"
#include"Collision.h"
class PushCollision
{
public:
	/// <summary>
	/// マップチップとプレイヤー
	/// </summary>
	/// <param name="player">プレイヤークラス</param>
	/// <param name="mapStage">マップチップクラス</param>
	static void Player2Mapchip(Player*player,MapStage *mapStage);
};