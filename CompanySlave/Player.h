#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
#include"Sprite.h"
//プレイヤーの向いている向き
enum Direction
{
	Up,
	Down,
	Left,
	Right,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight
};
/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	Player();//コンストラクタ

	~Player();//ですコンストラクタ

	void Init();//初期化

	void Update(class Enemy *enemy);//更新

	void Draw();//描画

	void SetPosition(Vec3 position) { this->position = position; }
private:
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();
	/// <summary>
	/// プレイヤーとエネミーとの最小距離の敵を見つける
	/// </summary>
	/// <param name="enemy">エネミークラス</param>
	void PlayerAttack(class Enemy *enemy);

	/// <summary>
	/// 攻撃を止める
	/// </summary>
	void StopAttack();
	//攻撃角度決定
	float Angle();

	//プレイヤーの向きを決める
	void PDirection();

	//回避
	void Avoidance();

	//デバック描画
	void DebugDraw();
public://取得系
	//座標
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//プレイヤーのBox
	Box GetBox() { return pBox; }

	bool GetAttackFlag() { return attackFlag; }
	bool GetMoveFlag() { return moveFlag; }

	int GetComboNum() { return comboNum; }

	int GetComboTime() { return nowComboTime; }

	int GetCoolTime() { return coolTime; }
private:
	Object::ObjectData playerObject;	//プレイヤーオブジェクト
	Box pBox;							//プレイヤーの長方形
	Vec3 position{ 10.0f,0.0f,0.0f };	//座標
	Vec3 oldPosition{};					//1つ前の座標
	Vec3 speed{ 1.5f,1.5f,1.5f };		//プレイヤースピード

	Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
	Vec3 angle{ 0.0f,0.0f,0.0f };		//角度
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
	float r = 3;

	//プレイヤーHP
	int HP = 10;

	//敵を倒すためのパラメータ
	const float comboMaxTime = 30;	//コンボ時間最初
	float comboTime = 30;			//コンボ時間
	float nowComboTime = 0;			//現在のコンボ時間
	bool comboFlag = false;
	int comboNum = 0;				//コンボ数

	//敵を斬りに行くまでのスピード
	const float attackMaxTime = 10.0f;
	float attackTime = 0;
	Vec3 attackSpeed = { 4.0f,4.0f,4.0f };

	//動いているかどうか
	bool moveFlag = false;

	//連続切りの最中かどうか
	bool attackFlag = false;
	//クールタイム
	const int coolTimeMax = 100;
	int coolTime = 0;

	int  direction = 0;		 //プレイヤーの向き
	float attackAngle = 0.0f;//攻撃向き

	//回避
	bool avoidanceFlag = false;		//回避中か
	const int avoidanceTimeMax = 10;//回避時間
	int avoiDirection = 0;			//回避向き
	int avoidanceTime = 0;			//今回避時間
	float avoiSpeed = 5.0f;			//回避スピード
	const int avoiCoolTimeMax = 20;	//回避クールタイム
	int avoiCoolTime = 0;			//今回避クールタイム

	bool invincibleFlag = false;	//無敵中か



	//UI
	Vec3 UIAngle{ 90.0f,0.0f,0.0f };
	Object::ObjectData comboPolygon;
	Object::ObjectData  comboNumberObj;//１桁目
	Object::ObjectData black;
	int comboGraph;
	int comboNumberGraph[10];
	int yellowColor;

#if _DEBUG
	Object::ObjectData attackField;//攻撃範囲可視化
	int redColor;
#endif
};