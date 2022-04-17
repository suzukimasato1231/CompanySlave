#pragma once
#include"LoadCSV.h"
#include"Object.h"
#include <string>
#include"CollisionPrimitive.h"
#include"Enemy.h"
#include <ParticleManager.h>
/// <summary>
/// �u���b�N�̎��
/// </summary>
enum ObjectStatus
{
	FLOOR_TILE1,//�΃^�C���P
	FLOOR_TILE2,//�΃^�C���Q
	FLOOR_TILE3,//�Β[�P
	FLOOR_TILE4,//�Β[�Q
	FLOOR_TILE5,
	FLOOR_TILE6,
	FLOOR_TILE7,
	FLOOR_TILE8,
	FLOOR_TILE9,
	FLOOR_TILE10,
	NONE,			//��
	WALLWIDTH,      //�u���b�N
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

	//int blockGraph = 0;
	int floor_Tile1 = 0;
	int floor_Tile2 = 0;
	int floor_Tile3 = 0;
	int floor_Tile4 = 0;
	int floor_Tile5 = 0;
	int floor_Tile6 = 0;
	int floor_Tile7 = 0;
	int floor_Tile8 = 0;
	int floor_Tile9 = 0;
	int floor_Tile10 = 0;

	int nextGraph = 0;//��
	int smokeGraph = 0;//��

	int	map[MAP_HEIGHT][MAP_WIDTH];//�}�b�v�`�b�v

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu

	Vec3 scale = { 5.0f,1.0f,5.0f };//�u���b�N�̑傫��

	Vec4 color = { 1.0f,1.0f,1.0f,1.0f };//�u���b�N�̐F

	Vec4 color2 = { 0.0f,0.0f,0.0f,0.5f };

	bool nextFlag = false;
	//����������
	ParticleManager* particleMan = nullptr;
};