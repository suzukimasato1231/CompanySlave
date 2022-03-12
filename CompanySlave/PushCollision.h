#pragma once
#include"Player.h"
#include"MapStage.h"
#include"Collision.h"
#include"Enemy.h"
class PushCollision
{
public:
	/// <summary>
	/// マップチップとプレイヤー
	/// </summary>
	/// <param name="player">プレイヤークラス</param>
	/// <param name="mapStage">マップチップクラス</param>
	static void Player2Mapchip(class Player *player, class MapStage *mapStage);


	/// <summary>
	/// 押し戻し処理
	/// </summary>
	/// <param name="Pos">座標</param>
	/// <param name="oldPos">動く前の座標</param>
	/// <param name="size">押し戻したいものの大きさ</param>
	/// <param name="BPos">当たったブロックの座標</param>
	/// <param name="blockSize">ブロックの大きさ</param>
	/// <param name="up">上のブロック</param>
	/// <param name="down">下のブロック</param>
	static Vec3 PushBack(Vec3 pos,Vec3 oldPos,float size,Vec3 BPos, float blockSize, const int up, const int down);
};