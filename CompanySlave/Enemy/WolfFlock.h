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
	//������
	void Init();

	void LoopInit();

	void Draw(EnemyData* oniData);
	//���G
	void SearchPlayer(EnemyData* oniData, class Player* player);
	//�����U��
	void AttackSummon(EnemyData* oniData);
	//�{�X���l��
	EnemyData GetBossData() { return bossData; }

	bool GetFlockFlag() { return flockFlag; };
private:
	//�����ɂ���č��G�͈͂̌���
	Box SearchField(EnemyData* oniData);
private:
	//�{�X�X�e�[�^�X
	EnemyData bossData;

	time_t start_time, end_time;	//���Ԍv��

	//�{�X�s���X�e�[�^�X
	//���G
	const Vec2 attackEnemies{ 300.0f,110.0f };		//���G�͈�:width,height
	const float player2EnemyLength = 500.0f;		//���G����
	
	const int wolfNumMax = 50;//�T�ő吔
	int wolfNum = 0;
	//����
	const int wolfTime = 2;//���̘T���o�鎞��

	bool flockFlag = false;//�T���o���Ă�����

	float slowValue = 1;
};

