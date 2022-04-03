#pragma once
#include"LoadCSV.h"
#include"Object.h"
#include <string>
#include"CollisionPrimitive.h"
#include"Enemy.h"
/// <summary>
/// �u���b�N�̎��
/// </summary>
enum ObjectStatus
{
	NONE,			//��
	WALLWIDTH,         //�u���b�N
	WALLHIGHT,
	WALLCORNER,
	STRAW,
	SMOKEWALL,//�G�S���|������������
	NextStageBlock,//�����ɐG�ꂽ�玟�̃X�e�[�W��
};
/// <summary>
/// �}�b�v�E�X�e�[�W
/// </summary>
class MapStage
{
private: // �G�C���A�X
// std::���ȗ�
	using string = std::string;
public:
	//�R���X�g���N�^
	MapStage();
	//�f�X�g���N�^
	~MapStage();
public://�萔

public:
	//������
	void Init();
	//�X�e�[�W���ς��Ƃ��̏�����
	void StageInit(int stageNum);
	//�X�V
	void Update(Enemy *enemy);
	//�`��
	void Draw(Vec3 pPos);

	Box GetPositionBlock(int i, int j);

	Vec3 GetPosition(int i, int j);

	int GetMap(int i, int j);

	float GetSize();
private:
	Object::ObjectData block;//�}�b�v�̃u���b�N
	Object::ObjectData wallBlock;//�}�b�v�̕ǃu���b�N
	Object::ObjectData cornerBlock;//�}�b�v�̕ǃu���b�N
	Object::ObjectData strawBlock;//�m

	int blockGraph = 0;
	int nextGraph = 0;//��
	int smokeGraph = 0;//��

	int	map[MAP_HEIGHT][MAP_WIDTH];//�}�b�v�`�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu

	Vec3 scale = { 1.0f,1.0f,1.0f };//�u���b�N�̑傫��

	Vec4 color = { 1.0f,1.0f,1.0f,1.0f };//�u���b�N�̐F

	Vec4 color2 = { 0.0f,0.0f,0.0f,0.5f };

	bool nextFlag = false;
};