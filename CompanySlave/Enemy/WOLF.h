#pragma once
#include"EnemyHelper.h"

class Player;
/// <summary>
/// 狼
/// </summary>
class Wolf
{
public:
	Wolf();
	~Wolf();

	void Init();

	void Draw(EnemyData* oniData);
	//倒れる描画
	void FallDownDraw(EnemyData* oniData);
	//移動
	void Move(EnemyData* oniData, class  Player* player);
	//索敵
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//攻撃
	void Attack(EnemyData* oniData, class Player* player);

	EnemyData WolfData() { return wolfData; }
private:
	//敵の攻撃エフェクト
	void EffectDraw(EnemyData* oniData);

	//向きによって索敵範囲の決定
	Box SearchField(EnemyData* oniData);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);
private:
	EnemyData wolfData;

	Object::ObjectData enemyObject[5];				//プレイヤーオブジェクト

	Object::ObjectData attackOBJ[3];				//エネミーアタック

	Object::ObjectData nockBackOBJ[2];				//ノックバックオブジェクト

	Object::ObjectData enemyDeadObject;				//死んだとき用オブジェクト

	const Vec2 attackEnemies{ 100.0f,80.0f };		//索敵範囲:width,height

	const Vec2 attackField{ 15.0f,15.0f };			//攻撃範囲

	const int moveTime = 40;						//移動時間

	const int attackMotionTime = 150;				//攻撃モーションの時間
	const int attackHoldTime = 20;					//攻撃を構える時間
	const int attackMoveTime = 30;					//攻撃の移動時間
	const int attackMotionDamege = 10;				//攻撃モーション中のどの時間でダメージを与えるか

	const float player2EnemyLength = 25.0f;			//プレイヤーと敵の距離

	const float moveSpeed = 0.8f;					//敵のスピード
	const float attackMoveSpeed = 1.2f;				//攻撃を行うときのスピード

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

