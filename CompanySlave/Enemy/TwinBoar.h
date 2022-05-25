#pragma once
#pragma once
#include"EnemyHelper.h"
#include<time.h>
class Player;

/// <summary>
/// イノシシ
/// </summary>
class TwinBoar
{
private:
	enum BigStatus
	{
		PREOPERATION,//予備動作
		DOUBLEATTACK,	//攻撃
		DOUBLEAFTER,//攻撃後
	};
public:
	TwinBoar();
	~TwinBoar();
	//初期化
	void Init();

	//倒れる描画
	void FallDownDraw(EnemyData* oniData);

	void Draw(EnemyData* oniData,int i);

	void PreDraw(EnemyData* oniData, int i);

	void LoopInit();
	//索敵
	void SearchPlayer(EnemyData* oniData, class Player* player);

	int  SearchStatus() { return BOSSATTACK; }
	//突進（交互）
	void AttackRush(EnemyData* oniData, class Player* player, int num);
	//突進（２体同時）
	void AttackDoubleRush(EnemyData* oniData, class Player* player);
	//短い突進
	void AttackShortRush(EnemyData* oniData, class Player* player, int ty);
	//ボス情報獲得
	EnemyData GetBossData() { return bossData; }

	bool GetSerachEnemyFlag() { return searchEnemyFlag; }
private:
	//向きによって索敵範囲の決定
	Box SearchField(EnemyData* oniData);
	//向きによって敵の描画角度を取得
	Vec3 DirectionAngle(int direction);
private:
	//ボスステータス
	EnemyData bossData;

	const float moveSpeed = 0.60f;					//敵のスピード

	time_t start_time, end_time;					//時間計測

	const int boarNumMax = 2;						//イノシシの数
	int boarNum = 2;

	//ボス行動ステータス
	//索敵
	const Vec2 attackEnemies{ 300.0f,110.0f };		//索敵範囲:width,height
	const float player2EnemyLength = 500.0f;		//索敵距離
	bool searchEnemyFlag = false;					//片方でも見つかったかどうか
	const Vec2 attackField{ 15.0f,15.0f };
	//行動を行う所定位置
	const Vec3 fixedPosition[2] = { {250.0f,0.0f,-200.0f} ,{750.0f,0.0f,-200.0f} };
	float moveFixedSpeed = 1.2f;
	//小突進
	const float sPlayer2EnemyLength = 50.0f;		//小突進を行う距離
	const int sAttackMotionTime = 120;				//攻撃モーション全体
	const int sAttackHoldTime = 50;					//攻撃構える時間
	const float sAttackSpeed = 1.3f;				//攻撃スピード
	bool attackFlag = false;
	const int sTimeMax = 300;						//小攻撃をしていい時間/2
	int sTime = 0;
	bool sAttack[2] = { false,false };
	//2体同時
	const int doublePreTimeMax = 100;				//予備動作時間
	int doublePreTime = 0;
	const int doubleTimeMax = 500;					//同時突進を行う時間
	int doubleTime = 0;
	const int doubleAfterTimeMax = 50;				//攻撃後の硬直時間
	int doubleAfterTime = 0;
	int attackStats = PREOPERATION;
	const float dAttackSpeed = 4.5f;				//攻撃スピード
	const int dAttackTimeMax = 100;					//攻撃時間
	int dAttackTime = 0;
	//交互突進
	const int bPreTimeMax = 50;
	int bPreTime = 0;
	const int bTimeMax = 300;						//交互突進を行う時間
	int bTime = 0;
	const Vec2 attackAltField{ 40.0f,40.0f };		//攻撃範囲
	int attackAltStatus = 0;						//予備動作開始
	bool altFlag = false;							//2体のうちどちらかが突進状態
	int altAttackNum = 0;							//交互突進を連続で何回やったか
	bool bFlag[2] = { false,false };
	bool bFinishFlag[2]{ false,false };

	//ボスOBJ
	Object::ObjectData enemyObject[5];					//プレイヤーオブジェクト
	Object::ObjectData attackOBJ[5];					//エネミーアタック
	//デバック用
	Object::ObjectData debugField;	//索敵
	Object::ObjectData debugField2;//攻撃範囲小
	Object::ObjectData attackBigOBJ;//攻撃大の範囲表示
	int redColor;
	//エフェクト関係

	bool AttackSound = false;


	int SoundCount = 0;

	float slowValue = 1;
};

