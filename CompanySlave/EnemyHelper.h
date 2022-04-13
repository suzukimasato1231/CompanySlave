#pragma once
#pragma once
#include "Object.h"
#include"Collision.h"
#include"Direction.h"
enum EnemyType
{
	Oni = 1,//近接鬼
	OniBow,//弓鬼
	Wolf//狼,
};

enum spawnNo
{
	NOTSPAWN,
	ONI,
	ONIBOW,
	WOLF,
};

//敵の状態
enum Status
{
	NORMAL, //通常状態
	MOVE,	//動き
	ATTACK,	//攻撃
	ENEMIES,//敵を見つけたら
	NOCKBACK,//ノックバック中
};
//敵の情報
struct EnemyData
{
	Box eBox;							//敵ボックス
	Sphere eSphere;						//敵球
	Vec3 position{ 50.0f,0.0f,0.0f };	//座標
	Vec3 oldPosition{};					//1個前の座標
	float speed = 0.3f;					//敵スピード
	Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
	float HPMax = 10;					//最大HP
	float HP = 10;						//HP
	float r = 5;						//大きさ
	int Status = NORMAL;				//状態
	int StatusTime = 0;					//状態時間
	int direction = Down;				//向いている向き
	int attackDirection = Down;			//攻撃の向き
	int damegeTime = 0;					//点滅時間
	bool DamegeFlag = false;			//ダメージを受けたか

	int type = 0;						//敵の種類
	//ノックバック数値
	int nockbackTime = 0;				//ノックバック時間
	int nockDirection = 0;				//ノックバックする方向
	//遠距離用数値
	float bowAngle = 0.0f;				//狙う角度
	Vec3 bowPos = {};					//矢の座標
	bool bowFlag = false;				//弓を撃ったか
	int bowTime = 0;					//矢が飛んでいく時間

	//破のやつ
	bool explosionFlag = false;
	int explosionCount = 2;
	bool explosionDelay = 0;
};