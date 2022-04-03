#pragma once
#include"LoadCSV.h"
#include"Object.h"
#include <string>
#include"CollisionPrimitive.h"
#include"Enemy.h"
/// <summary>
/// ブロックの種類
/// </summary>
enum ObjectStatus
{
	NONE,			//空
	WALLWIDTH,         //ブロック
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

	int blockGraph = 0;
	int nextGraph = 0;//仮
	int smokeGraph = 0;//仮

	int	map[MAP_HEIGHT][MAP_WIDTH];//マップチップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置

	Vec3 scale = { 1.0f,1.0f,1.0f };//ブロックの大きさ

	Vec4 color = { 1.0f,1.0f,1.0f,1.0f };//ブロックの色

	Vec4 color2 = { 0.0f,0.0f,0.0f,0.5f };

	bool nextFlag = false;
};