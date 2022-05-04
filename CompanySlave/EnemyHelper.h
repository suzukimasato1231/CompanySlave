#pragma once
#pragma once
#include "Object.h"
#include"Collision.h"
#include"Direction.h"
enum EnemyType
{
	Oni = 1,	//近接鬼
	OniBow,		//弓鬼
	WolfType,	//狼,
	BossBigOni,	//巨大鬼
	BossWolfFlock,//狼群れ
};

enum spawnNo
{
	ONIUP = 10,
	ONIDOWN,
	ONILEFT,
	ONIRIGHT,
	ONIBOWUP = 20,
	ONIBOWDOWN,
	ONIBOWLEFT,
	ONIBOWRIGHT,
	WOLFUP = 30,
	WOLFDOWN,
	WOLFRIGHT,
	WOLFLEFT,
	BossOni = 40,
	WOLFFLOCK = 50
};

//敵の状態
enum Status
{
	NORMAL, //通常状態
	MOVE,	//動き
	ATTACK,	//攻撃
	ENEMIES,//敵を見つけたら
	NOCKBACK,//ノックバック中
	//ボスの動き
	BOSSATTACK,		//ボス攻撃２
	BOSSATTACK2,	//ボス攻撃１
	SUMMON			//部下召喚
};

struct EnemySupport
{
	//ノックバックステータス
	static const int nockBackTimeMax = 5;
};

//敵の情報
struct EnemyData
{
	Box eBox;							//敵ボックス
	Sphere eSphere;						//敵球
	Vec3 position{ 0.0f,5.0f,0.0f };	//座標
	Vec3 oldPosition{};					//1個前の座標
	Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
	float HPMax = 7;					//最大HP
	float HP = 7;						//HP
	float r = 5;						//大きさ
	int Status = NORMAL;				//状態
	int memoryStatus = NORMAL;
	int StatusTime = 0;					//状態時間
	int direction = Down;				//向いている向き
	int attackDirection = Down;			//攻撃の向き
	int damegeTime = 0;					//点滅時間
	bool DamegeFlag = false;			//ダメージを受けたか
	bool attakFlag = false;				//アタック時の音用フラグ
	Vec3 pDirection = {};				//プレイヤーのいる方向
	int type = 0;						//敵の種類
	bool bossFlag = false;				//ボスかどうか
	//ノックバック数値
	int nockbackTime = 0;				//ノックバック時間
	int nockDirection = 0;				//ノックバックする方向
	bool nockPossibleFlag = true;		//ノックバックが可能かどうか
	//遠距離用数値
	float bowAngle = 0.0f;				//狙う角度
	Vec3 bowPos = {};					//矢の座標
	Vec3 bowOldPos = {};
	bool bowFlag = false;				//弓を撃ったか
	bool bowMove = false;
	int bowTime = 0;					//矢が飛んでいく時間
	bool bowAFlag = false;				//弓の音用フラグ
	//破のやつ
	bool explosionFlag = false;
	int explosionCount = 2;
	bool explosionDelay = 0;

	//描画関係
	int walkNum = 0;					//歩き
	int walkTime = 0;


};