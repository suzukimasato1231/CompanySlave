#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"

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
	Vec3 speed{ 2.0f,2.0f,2.0f };		//プレイヤースピード

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
	//コンボ数
	int comboNum = 0;

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

	//攻撃向き
	float attackAngle = 0.0f;
};