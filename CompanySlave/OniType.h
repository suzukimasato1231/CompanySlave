#pragma once
#include"EnemyHelper.h"

class Player;
/// <summary>
/// 鬼近接クラス
/// </summary>
class OniType
{
public:
	OniType();
	~OniType();

	void Init();

	void Draw(EnemyData *oniData);
	//移動
	void Move(EnemyData *oniData, class  Player *player);
	//索敵
	void SearchPlayer(EnemyData *oniData, class Player *player);
	//攻撃
	void Attack(EnemyData *oniData, class Player *player);
private:
	//敵の攻撃エフェクト
	void EffectDraw(EnemyData *oniData);

	//向きによって索敵範囲の決定
	Box SearchField(EnemyData *oniData);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);

	//向きによって攻撃範囲の決定
	Box AttackField(EnemyData *oniData);
private:

	Object::ObjectData enemyObject;					//プレイヤーオブジェクト

	Object::ObjectData attackOBJ[2];				//エネミーアタック

	Object::ObjectData nockBackOBJ[2];				//ノックバックオブジェクト

	const Vec2 attackEnemies{ 100.0f,80.0f };		//索敵範囲:width,height

	const Vec2 attackField{ 15.0f,15.0f };			//攻撃範囲

	const int moveTime = 40;						//移動時間

	const int attackMotionTime = 100;				//攻撃モーションの時間
	const int attackMotionDamege = 10;				//攻撃モーション中のどの時間でダメージを与えるか

	const float player2EnemyLength = 15.0f;			//プレイヤーと敵の距離

	const float moveSpeed = 0.3f;					//敵のスピード


	//デバック用
	Object::ObjectData debugField;	//索敵
	Object::ObjectData debugField2;//攻撃範囲
	int redColor;

	//エネミーアタック
	Object::ObjectData AttackEffectOBJ;//通常攻撃エフェクト
	int AttackEffectGraph[9];
	//エフェクト関係
	bool AttackEffect = false;
	float AttackEffectSize = 5.0f;
	Vec3 AttackScale{ 2.0f,2.0f,2.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };
	int effectTime = 10;
	int effectCount = 0;


	float slowValue = 1;
};

