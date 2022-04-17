#pragma once
#include"LoadCSV.h"
#include"Object.h"
#include <string>
#include"CollisionPrimitive.h"
#include"Enemy.h"
#include <ParticleManager.h>
/// <summary>
/// ブロックの種類
/// </summary>
enum ObjectStatus
{
	FLOOR_TILE1,//石タイル１
	FLOOR_TILE2,//石タイル２
	FLOOR_TILE3,//石端１
	FLOOR_TILE4,//石端２
	FLOOR_TILE5,
	FLOOR_TILE6,
	FLOOR_TILE7,
	FLOOR_TILE8,
	FLOOR_TILE9,
	FLOOR_TILE10,
	NONE,			//空
	WALLWIDTH,      //ブロック
	WALLHIGHT,
	WALLCORNER,
	STRAW,
	SMOKEWALL,//敵全部倒したら消える壁
	NextStageBlock,//ここに触れたら次のステージへ
};
/// <summary>
/// マップ・ステージ
/// </summary>
class MapStage
{
private: // エイリアス
// std::を省略
	using string = std::string;
public:
	//コンストラクタ
	MapStage();
	//デストラクタ
	~MapStage();
public://定数

public:
	//初期化
	void Init();
	//ステージが変わるときの初期化
	void StageInit(int stageNum);
	//更新
	void Update(Enemy *enemy);
	//描画
	void Draw(Vec3 pPos);

	Box GetPositionBlock(int i, int j);

	Vec3 GetPosition(int i, int j);

	int GetMap(int i, int j);

	float GetSize();
private:
	Object::ObjectData block;//マップのブロック
	Object::ObjectData wallBlock;//マップの壁ブロック
	Object::ObjectData cornerBlock;//マップの壁ブロック
	Object::ObjectData strawBlock;//藁

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

	int nextGraph = 0;//仮
	int smokeGraph = 0;//仮

	int	map[MAP_HEIGHT][MAP_WIDTH];//マップチップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置

	Vec3 scale = { 5.0f,1.0f,5.0f };//ブロックの大きさ

	Vec4 color = { 1.0f,1.0f,1.0f,1.0f };//ブロックの色

	Vec4 color2 = { 0.0f,0.0f,0.0f,0.5f };

	bool nextFlag = false;
	//もくもく煙
	ParticleManager* particleMan = nullptr;
};