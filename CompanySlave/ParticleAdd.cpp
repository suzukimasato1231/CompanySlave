#include "ParticleAdd.h"

ParticleAdd::ParticleAdd()
{
}

ParticleAdd::~ParticleAdd()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(particleMan3);
	safe_delete(particleMan4);
	safe_delete(particleMan5);
}

void ParticleAdd::Init()
{
	//パーティクルクラス作成
	particleMan = ParticleManager::Create(L"Resources/Eblood/BloodCircle.png", 0);
	particleMan2 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile1.png", 0);
	particleMan3 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile3.png", 0);
	particleMan4 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile7.png", 0);
	particleMan5 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile9.png", 0);
}

void ParticleAdd::Update(class Player* player, class Enemy* enemy, class MapStage* mapStage)
{
	if (player->GetMoveFlag() == true)
	{
		if (mapStage->GetfloorNum() == 0) {
			particleMan2->ParticleAdd3(player->GetPosition(), 0.1f, 1, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 1) {
			particleMan3->ParticleAdd3(player->GetPosition(), 0.1f, 1, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 2) {
			particleMan4->ParticleAdd3(player->GetPosition(), 0.1f, 1, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 3) {
			particleMan5->ParticleAdd3(player->GetPosition(), 0.1f, 1, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}

	//パーティクル追加
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		float speed = 1.0f;
		int addNum = 15;
		float startScale = 4.0f, endScale = 0.3f;
		if (enemy->GetBossFlag(i) == true) {
			speed = 3.0f;
			addNum = 20;
			startScale = 6.0f, endScale = 1.0f;
		}
		//ダメージを受けたら描画
		if (enemy->GetDamegeFlag(i) == true) {
			if (player->GetDirection() == Right) {
				particleMan->SetParticleDirection(0);
				particleMan->ParticleAdd(enemy->GetPosition(i), speed, speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == Left) {
				particleMan->SetParticleDirection(1);
				particleMan->ParticleAdd(enemy->GetPosition(i), -speed, speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == Up) {
				particleMan->SetParticleDirection(2);
				particleMan->ParticleAdd(enemy->GetPosition(i), speed, -speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == Down) {
				particleMan->SetParticleDirection(3);
				particleMan->ParticleAdd(enemy->GetPosition(i), -speed, -speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == UpRight) {
				particleMan->SetParticleDirection(4);
				particleMan->ParticleAdd(enemy->GetPosition(i), speed, speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == UpLeft) {
				particleMan->SetParticleDirection(5);
				particleMan->ParticleAdd(enemy->GetPosition(i), -speed, speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == DownRight) {
				particleMan->SetParticleDirection(6);
				particleMan->ParticleAdd(enemy->GetPosition(i), speed, -speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
			else if (player->GetDirection() == DownLeft) {
				particleMan->SetParticleDirection(7);
				particleMan->ParticleAdd(enemy->GetPosition(i), -speed, -speed, startScale, endScale, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f), addNum);
			}
		}
	}

	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		//狼の突進時のパーティクル
		if (enemy->GetRushFlag(i) == true && enemy->GetType(i) == WolfType && enemy->GetHP(i) > 0) {
			if (mapStage->GetfloorNum() == 0) {
				particleMan2->ParticleAdd3(enemy->GetPosition(i), 0.1f, 2.0, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 1) {
				particleMan3->ParticleAdd3(enemy->GetPosition(i), 0.1f, 2.0, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 2) {
				particleMan4->ParticleAdd3(enemy->GetPosition(i), 0.1f, 2.0, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 3) {
				particleMan5->ParticleAdd3(enemy->GetPosition(i), 0.1f, 2.0, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		//猪の突進時のパーティクル
		if (enemy->GetRushFlag(i) == true && enemy->GetType(i) == BoarType && enemy->GetHP(i) > 0) {
			if (mapStage->GetfloorNum() == 0) {
				particleMan2->ParticleAdd3(enemy->GetPosition(i), 0.1f, 3, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 1) {
				particleMan3->ParticleAdd3(enemy->GetPosition(i), 0.1f, 3, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 2) {
				particleMan4->ParticleAdd3(enemy->GetPosition(i), 0.1f, 3, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (mapStage->GetfloorNum() == 3) {
				particleMan5->ParticleAdd3(enemy->GetPosition(i), 0.1f, 3, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	particleMan->Update();
	particleMan2->Update();
	particleMan3->Update();
	particleMan4->Update();
	particleMan5->Update();
}

void ParticleAdd::Draw()
{
	particleMan->Draw();
	particleMan2->Draw();
	particleMan3->Draw();
	particleMan4->Draw();
	particleMan5->Draw();
}
