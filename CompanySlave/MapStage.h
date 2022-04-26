#pragma once
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
/// <summary>
/// �u���b�N�̎��
/// </summary>
enum ObjectStatus
{
	FLOOR_TILE1,	//�΃^�C���P
	FLOOR_TILE2,	//�΃^�C���Q
	FLOOR_TILE3,	//�Β[�P
	FLOOR_TILE4,	//�Β[�Q
	FLOOR_TILE5,
	FLOOR_TILE6,
	FLOOR_TILE7,
	FLOOR_TILE8,
	FLOOR_TILE9,
	FLOOR_TILE10,
	FLOOR_TILE11,
	FLOOR_TILE12,
	FLOOR_TILE13,
	FLOOR_TILE14,
	FLOOR_TILE15,
	FLOOR_TILE16,
	FLOOR_TILE17,
	FLOOR_TILE18,
	FLOOR_TILE19,
	FLOOR_TILE20,
	FLOOR_TILE21,
	FLOOR_TILE22,
	FLOOR_TILE23,
	FLOOR_TILE24,
	FLOOR_TILE25,
	FLOOR_TILE26,
	FLOOR_TILE27,
	FLOOR_TILE28,
	FLOOR_TILE29,
	FLOOR_TILE30,
};

enum OBJPOSITION
{
	NONE,			//��
	WALLWIDTH,      //�ǂ̉�
	WALLHIGHT,		//�ǂ̏c
	WALL_LU,		//�ǂ̐ڑ�������
	WALL_RU,		//�ǂ̐ڑ����E��
	WALL_RD,		//�ǂ̐ڑ����E��
	WALL_LD,		//�ǂ̐ڑ�������
	OKE,			//���̃I�u�W�F�N�g
	SMOKEWALL,		//�G�S���|������������
	NextStageBlock,//���̃X�e�[�W��
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
	//�p�[�e�B�N���`��
	void DrawParticle(Vec3 pPos);

	Box GetPositionBlock(int i, int j);

	Vec3 GetPosition(int i, int j);

	int GetMap(int i, int j);

	int GetMapGround(int i, int j) { return map[j][i]; };

	float GetSize();

	//floorNum��Getter��Setter
	int GetfloorNum() { return FloorNum; }
	int SetfloorNum(int floorNum) { return this->FloorNum = floorNum; }

	
private:
	Object::ObjectData block;//�}�b�v�̃u���b�N
	Object::ObjectData wallBlock;//�}�b�v�̕ǃu���b�N
	Object::ObjectData cornerBlock;//�}�b�v�̕ǃu���b�N
	Object::ObjectData okeBlock;//�m
	ObjectStatus status;
	//int blockGraph = 0;
	int floor_Tile[30] = {};

	int nextGraph = 0;//��
	int smokeGraph = 0;//��

	int	map[MAP_HEIGHT][MAP_WIDTH];//�}�b�v�`�b�v

	int mapOBJ[MAP_HEIGHT][MAP_WIDTH];//�}�b�v�`�b�vOBJ
	
	int FloorNum = 0;//�p�[�e�B�N���p�̕ϐ�

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//�}�b�v�`�b�v�̏����ʒu

	Vec3 scale = { 5.0f,1.0f,5.0f };//�u���b�N�̑傫��

	Vec4 color = { 1.0f,1.0f,1.0f,1.0f };//�u���b�N�̐F

	Vec4 color2 = { 0.0f,0.0f,0.0f,0.5f };

	bool nextFlag = false;
	//����������
	ParticleManager* particleMan = nullptr;
};