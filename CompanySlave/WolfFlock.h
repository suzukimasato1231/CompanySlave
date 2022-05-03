#pragma once
#pragma once
#pragma once
#include"EnemyHelper.h"
#include<time.h>
class Player;
class WolfFlock
{
public:
	WolfFlock();
	~WolfFlock();
	//初期化
	void Init();

	void LoopInit();

	void Draw(EnemyData* oniData);
	//索敵
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//召喚攻撃
	void AttackSummon(EnemyData* oniData);
	//ボス情報獲得
	EnemyData GetBossData() { return bossData; }

	bool GetFlockFlag() { return flockFlag; };
private:
	//向きによって索敵範囲の決定
	Box SearchField(EnemyData* oniData);
private:
	//ボスステータス
	EnemyData bossData;

	time_t start_time, end_time;	//時間計測

	//ボス行動ステータス
	//索敵
	const Vec2 attackEnemies{ 300.0f,110.0f };		//索敵範囲:width,height
	const float player2EnemyLength = 500.0f;		//索敵距離
	
	const int wolfNumMax = 50;//狼最大数
	int wolfNum = 0;
	//召喚
	const int wolfTime = 2;//次の狼が出る時間

	bool flockFlag = false;//狼を出していいか

	float slowValue = 1;
};

