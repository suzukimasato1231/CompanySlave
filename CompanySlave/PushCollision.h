#pragma once
#include"Player.h"
#include"MapStage.h"
#include"Collision.h"
class PushCollision
{
public:
	/// <summary>
	/// �}�b�v�`�b�v�ƃv���C���[
	/// </summary>
	/// <param name="player">�v���C���[�N���X</param>
	/// <param name="mapStage">�}�b�v�`�b�v�N���X</param>
	static void Player2Mapchip(Player*player,MapStage *mapStage);
};