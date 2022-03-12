#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
enum AttackDirection
{
	AttackUp,
	AttackDown,
	AttackLeft,
	AttackRight,
};
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
	/// ダメージ処理
	/// </summary>
	void Damege();

	/// <summary>
	/// 指定した方向に敵がいるかどうか
	/// </summary>
	/// <param name="enemy">エネミークラス</param>
	/// <param name="enemyNumber">どの敵</param>
	/// <returns></returns>
	bool AttackDirection(class Enemy *enemy, int enemyNumber);
public://取得系
	//座標
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//プレイヤーのBox
	Box GetBox() { return pBox; }

	bool GetAttackFlag() { return attackFlag; }

	int GetComboNum() { return comboNum; }
private:
	Object::ObjectData playerObject;	//プレイヤーオブジェクト

	Box pBox;							//プレイヤーの長方形

	Vec3 position{ 10.0f,0.0f,0.0f };	//座標
	Vec3 oldPosition{};					//1つ前の座標
	Vec3 speed{ 2.0f,2.0f,2.0f };		//プレイヤースピード

	Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
	Vec3 angle{ 0.0f,0.0f,0.0f };		//角度
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
	float r = 5;

	//プレイヤーHP
	int HP = 10;

	//敵を倒すためのパラメータ
	const float comboMaxTime = 10;	//コンボ時間最初
	float comboTime = 0;			//コンボ時間
	float nowComboTime = 0;			//現在のコンボ時間

	int attackDirection = 0;//斬りに行く方向
	Vec3 startPos = {};				//斬りに行く前のプレイヤーの座標
	Vec3 enemyPos = {};				//斬りに行く敵の座標
	int enemyNum = 0;				//どの敵かの情報

	//連続切りの最中かどうか
	bool attackFlag = false;
	//コンボ数
	int comboNum = 0;

	//攻撃範囲
	Vec2 attackPos = {};//攻撃開始位置
	float circleMain = 30.0f;
	float ciecleSub = 30.0f;
#if _DEBUG
	Object::ObjectData circleM;
	Object::ObjectData circleS;
	int white;
	int blue;
	int yellow;
#endif
};