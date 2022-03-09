#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
class Player
{
public:
	Player();//�R���X�g���N�^

	~Player();//�ł��R���X�g���N�^

	void Init();//������

	void Update();//�X�V


	void Draw();//�`��

	/// <summary>
	/// �����߂�����
	/// </summary>
	/// <param name="BPos">�u���b�N�̈ʒu</param>
	/// <param name="blockSize">�u���b�N�T�C�Y</param>
	/// <param name="up">��̃u���b�N</param>
	/// <param name="down">���̃u���b�N</param>
	void PushBlock(Vec3 BPos, float blockSize, const int up, const int down);

	Vec3 GetPosition();//���W�擾

	Box GetBox();
private:
	Object::ObjectData playerObject;//�v���C���[�I�u�W�F�N�g

	Box pBox;//�v���C���[�̋�

	Vec3 position{ 10.0f,0.0f,0.0f };//���W
	Vec3 oldPosition{};
	Vec3 speed{ 2.0f,2.0f,2.0f };//�v���C���[�X�s�[�h

	Vec3 scale{ 1.0f,1.0f,1.0f };//�傫��
	Vec3 angle{ 0.0f,0.0f,0.0f };//�p�x
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };//�F

	float r = 5;

	//�W�����v
	const float jumpPowerMax = 5.0f;
	float jumpPower = 5.0f;//�W�����v�p���[
	float jumpPowerDelay = 0.2;
	float gravity = 2.0f;//�d��
	bool jumpFlag = false;//�W�����v��������
	bool groundFlag = false;//�n�ʂɐڂ��Ă��邩�ǂ���

};