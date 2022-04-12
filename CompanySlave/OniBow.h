#pragma once
#include"EnemyHelper.h"
class Player;
/// <summary>
/// 鬼遠距離クラス
/// </summary>
class OniBow
{
public:
	OniBow();
	~OniBow();

	void Init();

	void Draw(EnemyData *oniData);
	//移動
	void Move(EnemyData *oniData, class  Player *player);
	//索敵
	void SearchPlayer(EnemyData *oniData, class Player *player);
	//攻撃
	void Attack(EnemyData *oniData, class Player *player);
	//矢のアップデート
	void BowUpdate(EnemyData *oniData, Player *player);
private:
	//向きによって索敵範囲の決定
	Box SearchField(EnemyData *oniData);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);
private:

	Object::ObjectData enemyObject;					//プレイヤーオブジェクト

	Object::ObjectData attackOBJ[2];				//エネミーアタック

	const Vec2 attackEnemies{ 100.0f,80.0f };		//索敵範囲:width,height

	const int moveTime = 40;						//移動時間

	const int attackMotionTime = 200;				//攻撃モーションの時間
	const int attackMotionDamege = 100;				//攻撃モーション中のどの時間でダメージを与えるか

	const float player2EnemyLength = 50.0f;			//プレイヤーと敵の距離


	const float bowSpeed = 2.0f;					//矢が飛んでいくスピード
	const int bowTimeMax = 100;
	const Vec3 bowSize{ 4.0f,1.0f,8.0f };			//矢のサイズ
	Object::ObjectData bowOBJ;//矢の描画
	Object::ObjectData bowRaysOBJ;//矢の射線


	//デバック用
	Object::ObjectData debugField;	//索敵
	Object::ObjectData debugField2;//攻撃範囲
	int redColor;

	float slowValue= 1;
};

