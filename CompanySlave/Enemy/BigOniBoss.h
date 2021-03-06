#pragma once
#pragma once
#include"EnemyHelper.h"
#include<time.h>
class Player;

/// <summary>
/// 鬼近接クラス
/// </summary>
class BigOniBoss
{
private:
	enum BigStatus
	{
		PREOPERATION,//予備動作
		BIGATTACK,	//攻撃
		BIGAFTER,//攻撃後
	};
public:
	BigOniBoss();
	~BigOniBoss();
	//初期化
	void Init();

	void Draw(EnemyData* oniData);
	//倒れる描画
	void FallDownDraw(EnemyData* oniData);
	//プレイヤーより先に描画
	void PreDraw(EnemyData* oniData);
	//索敵
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//打撃小
	void AttackSmall(EnemyData* oniData, class Player* player);
	//打撃大
	void AttackBig(EnemyData* oniData, class Player* player);
	//召喚攻撃
	void AttackSummon(EnemyData* oniData, class Player* player, int eNum);
	//ボス情報獲得
	EnemyData GetBossData() { return bossData; }

	bool GetFirstSummonFlag() { return firstSummonFlag; }
private:
	//敵の攻撃エフェクト
	void EffectDraw(EnemyData* oniData);

	//向きによって索敵範囲の決定
	Box SearchField(EnemyData* oniData);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);

	//向きによって攻撃範囲の決定
	Box AttackField(EnemyData* oniData);

	Sphere AttackBIG(EnemyData* oniData);
private:
	//ボスステータス
	EnemyData bossData;

	//どの攻撃をしているか(大か召喚したあとかどうか)
	bool attackTypeFlag = false;

	time_t start_time, end_time;	//時間計測



	//ボス行動ステータス
	//索敵
	const Vec2 attackEnemies{ 300.0f,110.0f };		//索敵範囲:width,height
	const float player2EnemyLength = 500.0f;		//索敵距離
	//小攻撃
	const float attackSmallLength = 30.0f;			//攻撃範囲
	const Vec2 attackField{ 30.0f,30.0f };			//攻撃範囲
	const float moveSpeed = 0.60f;					//敵のスピード
	const int attackSmallTimeMax = 200;				//攻撃時間最大
	int attackSmallTime = 0;						//攻撃時間
	const int attackMotionDamege = 50;				//攻撃時間中にダメージを与える時間
	int attackSmallNum = 0;							//小攻撃を何回行ったか
	time_t small_start, small_end;
	int smallEffectTime = 0;
	//大攻撃
	time_t big_start, big_end;						//予備動作時間
	float attackBigField = 50.0f;					//攻撃範囲
	int attackBigStatus = 0;						//予備動作開始
	Object::ObjectData BigEffectOBJ;
	int BigEffectGraph;
	bool BigEffect = false;
	float BigEffectSize = 20.0f;
	Vec3 BigScale{ 0.0f,0.0f,5.0f };
	Vec3 BigAngle{ 90.0f,0.0f,0.0f };
	int BigCount = 0;


	//召喚
	bool firstSummonFlag = false;
	Object::ObjectData SummonEffectOBJ;
	int SummonEffectGraph;
	bool SummonEffect = false;
	float SummonEffectSize = 20.0f;
	Vec3 SummonScale{ 5.0f,5.0f,5.0f };
	Vec3 SummonAngle{ 90.0f,0.0f,0.0f };
	int SummonCount = 0;


	//ボスOBJ
	Object::ObjectData enemyObject[3];					//プレイヤーオブジェクト
	Object::ObjectData attackOBJ[2];				//エネミーアタック
	Object::ObjectData nockBackOBJ[2];				//ノックバックオブジェクト
	Object::ObjectData enemyDeadObject;					//死んだときオブジェクト
	//デバック用
	Object::ObjectData debugField;	//索敵
	Object::ObjectData debugField2;//攻撃範囲小
	Object::ObjectData attackBigOBJ;//攻撃大の範囲表示
	int redColor;

	//エネミーアタック
	Object::ObjectData AttackEffectOBJ;//通常攻撃エフェクト
	int AttackEffectGraph[9];
	//エフェクト関係
	//bool AttackEffect = false;
	float AttackEffectSize = 20.0f;
	Vec3 AttackScale{ 2.0f,1.0f,1.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };
	int effectTime = 10;
	//int effectCount = 0;
	float slowValue = 1;

	//音関係
	bool attakAFlag = false;
	int attakCount = 0;
};

