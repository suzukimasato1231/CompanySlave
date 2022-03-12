#pragma once
#include"Player.h"
#include"MapStage.h"
#include"Collision.h"
#include"Enemy.h"
class PushCollision
{
public:
	/// <summary>
	/// �}�b�v�`�b�v�ƃv���C���[
	/// </summary>
	/// <param name="player">�v���C���[�N���X</param>
	/// <param name="mapStage">�}�b�v�`�b�v�N���X</param>
	static void Player2Mapchip(class Player *player, class MapStage *mapStage);


	/// <summary>
	/// �����߂�����
	/// </summary>
	/// <param name="Pos">���W</param>
	/// <param name="oldPos">�����O�̍��W</param>
	/// <param name="size">�����߂��������̂̑傫��</param>
	/// <param name="BPos">���������u���b�N�̍��W</param>
	/// <param name="blockSize">�u���b�N�̑傫��</param>
	/// <param name="up">��̃u���b�N</param>
	/// <param name="down">���̃u���b�N</param>
	static Vec3 PushBack(Vec3 pos,Vec3 oldPos,float size,Vec3 BPos, float blockSize, const int up, const int down);
};