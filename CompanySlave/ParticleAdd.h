#pragma once
#include <ParticleManager.h>
#include<Player.h>
#include<MapStage.h>
#include<Enemy.h>
class ParticleAdd
{
public:
	ParticleAdd();

	~ParticleAdd();

	void Init();

	void Update(class Player* player, class Enemy* enemy, class MapStage* mapStage);

	void Draw();
private:
	//パーティクル
	//敵死んだ時のパーティクル
	ParticleManager* particleMan = nullptr;
	//歩行パーティクル	
	ParticleManager* particleMan2 = nullptr;
	ParticleManager* particleMan3 = nullptr;
	ParticleManager* particleMan4 = nullptr;
	ParticleManager* particleMan5 = nullptr;

};

