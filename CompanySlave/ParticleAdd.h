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
	//�p�[�e�B�N��
	//�G���񂾎��̃p�[�e�B�N��
	ParticleManager* particleMan = nullptr;
	//���s�p�[�e�B�N��	
	ParticleManager* particleMan2 = nullptr;
	ParticleManager* particleMan3 = nullptr;
	ParticleManager* particleMan4 = nullptr;
	ParticleManager* particleMan5 = nullptr;

};

