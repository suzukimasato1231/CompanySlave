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
	//���G�͈͓��Ƀv���C���[��������
	if (Collision::CheckBox2Box(player->GetBox(), enemiesBox))
	{
		//�v���C���[���U�������ɂ�����
		oniData->Status = SUMMON;//���Ō�
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
	//���^�[��HP��߂����ƂŌ��炷����
	oniData->HP = oniData->HPMax;
	if (wolfNum > 0)
	{
		end_time = time(NULL);

		int nextTime = end_time - start_time;
		//���̘T���o�����ԂT�b����
		if (nextTime > wolfTime)
		{
			wolfNum -= 5;			//1�x�ɂT�̏o��
			flockFlag = true;
			start_time = time(NULL);//���ԃ��Z�b�g
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
