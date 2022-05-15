#include "WolfFlock.h"
#include"Player.h"
#include"Shape.h"
WolfFlock::WolfFlock()
{}

WolfFlock::~WolfFlock()
{}

void WolfFlock::Init()
{
	bossData.HP = 50.0f;
	bossData.HPMax = 50.0f;
	bossData.scale = { 1.0f,1.0f,1.0f };
	bossData.bossFlag = true;
}

void WolfFlock::LoopInit()
{
	wolfNum = wolfNumMax;
	flockFlag = false;
	start_time = time(NULL);
}

void WolfFlock::Draw(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
}

void WolfFlock::SearchPlayer(EnemyData* oniData, Player* player)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}
	Box enemiesBox = SearchField(oniData);
	//索敵範囲内にプレイヤーがいたら
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//プレイヤーが攻撃距離にいたら
		oniData->Status = SUMMON;//小打撃
		start_time = time(NULL);
	}
}

void WolfFlock::AttackSummon(EnemyData* oniData)
{
	if (oniData == nullptr)
	{
		assert(oniData);
	}

	if (flockFlag == true)
	{
		flockFlag = false;
	}
	//毎ターンHPを戻すあとで減らすため
	oniData->HP = oniData->HPMax;
	if (wolfNum > 0)
	{
		end_time = time(NULL);

		int nextTime = end_time - start_time;
		//次の狼を出す時間５秒ごと
		if (nextTime > wolfTime)
		{
			wolfNum -= 5;			//1度に５体出す
			flockFlag = true;
			start_time = time(NULL);//時間リセット
		}
	}
}

Box WolfFlock::SearchField(EnemyData* oniData)
{
	Box enemiesBox;
	enemiesBox.maxPosition = XMVectorSet(1000.0f, 30.0f, 0.0f, 1.0f);
	enemiesBox.minPosition = XMVectorSet(-1000.0f, -30.0f, -250.0f, 1.0f);

	return enemiesBox;
}
