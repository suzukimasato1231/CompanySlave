#include "PlayScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include<time.h>

PlayScene::PlayScene()
{}
PlayScene::~PlayScene()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(particleMan3);
	safe_delete(particleMan4);
	safe_delete(particleMan5);
	safe_delete(lightGroup);
	safe_delete(mapStage);
	safe_delete(player);
	safe_delete(enemy);
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	safe_delete(audio);
}
void PlayScene::Initialize()
{
	//Audioクラス作成
	audio = Audio::Create();
	//カメラクラス作成
	camera = Camera::Create();
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();

	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/Music/BGM/rain.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/BGM/battle.wav");


	//スプライト画像読み込み
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/back.png");
	controlGraph = Sprite::Instance()->SpriteCreate(L"Resources/ControlUI/ControlUI.png");
	GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver.png");
	SChangeGraph = Sprite::Instance()->SpriteCreate(L"Resources/SceneChange.png");

	for (int i = 0; i < rainMax; i++) {
		RainOBJ[i] = Shape::CreateRect(1, 1);
	}
	rainGraph = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	//3Dオブジェクト画像読み込み
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	//パーティクルクラス作成
	particleMan = ParticleManager::Create(L"Resources/Eblood/BloodCircle.png", 0);
	particleMan2 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile1.png", 0);
	particleMan3 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile3.png", 0);
	particleMan4 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile7.png", 0);
	particleMan5 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile9.png", 0);



	//マップチップの初期化
	mapStage = new MapStage;
	mapStage->Init();
	//プレイヤーの初期化
	player = new Player;
	player->Init();
	//敵
	enemy = new Enemy;
	enemy->Init();
}

void PlayScene::Init()
{
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//カメラ位置をセット
	camera->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//ステージ初期化
	stageFlag = true;
	stageNum = 1;
	//ループ処理での初期化
	player->LoopInit();

	deadGraphPos = { 0.0f,-800.0f };
	nowTime = 0;//ラープ
	endTime = 5;//ラープ
	timeRate = 0;//ラープ

	fade = 1.0f;
	stageFlag = true;

	sceneFlag = false;
	sceneChangeFlag = false;
	ChangeGraphPosition = { -1600.0f, 0.0f };
	srand(time(NULL));

	for (int i = 0; i < rainMax; i++) {
		//xとzはランダムな位置にしている
		//サイズもランダムにしている
		position[i].x = (float)rand() / 20;
		position[i].z = (float)rand() / -100;
		position[i].y = 100;
		s[i] = (float)rand() / 1000;
	}
}

void PlayScene::StageInit()
{
	if (stageFlag == true)
	{
		//audio->SoundBGMPlayLoopWave(sound1);
		switch (stageNum)
		{
		case 1:

			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 2:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 3:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 4:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 5:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 6:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 7:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 8:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		default:
			break;
		}
		stageNum++;
		stageFlag = false;
		sceneChangeFlag = false;
	}
}

void PlayScene::Update()
{
	//パーティクル初期化
	ParticleManager::SetCamera(camera);
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	//nextステージ

	StageInit();



	//プレイヤーの更新
	mapStage->Update(player->GetPosition(), enemy);

	if (sceneChangeFlag == false) {
		player->Update(enemy);

		enemy->Update(player);
	}

	//if (Input::Instance()->ControllerDown(ButtonX))
	//{
	//	audio->SoundSEPlayWave(sound2);
	//}

	//0が無音
	enemy->SetVolume(volume);
	player->SetVolume(volume);
	audio->SetVolume(volume);


	//マップチップとプレイヤーの押し戻し処理
	PushCollision::Player2Mapchip(player, enemy, mapStage, sceneChangeFlag);


	//
	//カメラ調整
	camera->FollowCamera(player->GetCameraPos(), Vec3{0,80,-10}, 0.0f, -15.0f);



	//パーティクル追加
	if (player->GetMoveFlag() == true)
	{
		if (mapStage->GetfloorNum() == 0) {
			particleMan2->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 1) {
			particleMan3->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 2) {
			particleMan4->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 3) {
			particleMan5->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

	}

	//パーティクル追加
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{

		//ダメージを受けたら描画
		if (enemy->GetDamegeFlag(i) == true) {
			if (player->GetDirection() == Right) {
				particleMan->SetParticleDirection(0);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Left) {
				particleMan->SetParticleDirection(1);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Up) {
				particleMan->SetParticleDirection(2);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Down) {
				particleMan->SetParticleDirection(3);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == UpRight) {
				particleMan->SetParticleDirection(4);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == UpLeft) {
				particleMan->SetParticleDirection(5);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == DownRight) {
				particleMan->SetParticleDirection(6);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == DownLeft) {
				particleMan->SetParticleDirection(7);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	if (player->GetHP() <= 0 && deadGraphPos.y < 0)
	{
		//audio->SoundStop();

		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		deadGraphPos.y = -800 * (1.0f - (timeRate * timeRate) * (3 - (2 * timeRate))) + 0 * (timeRate * timeRate) * (3 - (2 * timeRate));
	}

	//プレイシーンを抜ける
	if (Input::Instance()->ControllerDown(ButtonA) && nowTime >= 2.0f)
	{

		deadGraphPos.y = -800;
		nowTime = 0;
		endTime = 5;
		timeRate = 0;
		sceneFlag = true;
	}

	if (sceneChangeFlag == true) {
		//audio->SoundStop();
		if (ChangeGraphPosition.x < 0) {
			ChangeGraphPosition.x += 22;
		}
		if (ChangeGraphPosition.x >= 0) {

			stageFlag = true;
		}
	}
	else if (sceneChangeFlag == false) {
		ChangeGraphPosition = { -1600.0f, 0.0f };
	}




	rainSlow = player->GetSlow();
	for (int i = 0; i < rainMax; i++) {

		if (position[i].y > 0) {
			//雨が地面につくまで降る
			position[i].y = position[i].y - g * rainSlow;
		}
		if (position[i].y <= 0) {
			//地面についたらまた上に行く
			//xとzはランダムな位置にしている
			//サイズもランダムにしている
			position[i].y = 100;
			position[i].x = rand() % 200 - 100;
			position[i].z = rand() % 220 - 110;
			position[i].x += player->GetPosition().x;
			position[i].z += player->GetPosition().z;
			s[i] = (float)rand() / 1000;
		}
	}
	particleMan->Update();
	particleMan2->Update();
	particleMan3->Update();
	particleMan4->Update();
	particleMan5->Update();

	//ライト更新
	lightGroup->Update();
}
void PlayScene::Draw()
{
	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);

	//オブジェクト

	//マップチップの描画
	mapStage->Draw(camera->GetEye());

	//敵の血痕
	enemy->BloodDraw();

	//プレイヤーの描画
	player->Draw();

	enemy->Draw();

	//パーティクル描画
	mapStage->DrawParticle(camera->GetEye());

	particleMan->Draw();
	particleMan2->Draw();
	particleMan3->Draw();
	particleMan4->Draw();
	particleMan5->Draw();

	enemy->DrawBlood();


	for (int i = 0; i < rainMax; i++) {
		Object::Instance()->Draw(RainOBJ[i], position[i], { 0.1,s[i] ,0.1 }, Vec3{ 1,1,1 }, Vec4{ 1,1,1,1 }, rainGraph);
	}
	//前景描画
	player->UIDraw();
	enemy->DrawUI();
	Sprite::Instance()->Draw(controlGraph, Vec2(0, 0), window_width, window_height);

	if (sceneChangeFlag == true) {
		Sprite::Instance()->Draw(SChangeGraph, ChangeGraphPosition, 1980, window_height);
	}

#if _DEBUG
	debugText.Print(10, 180, 2, "%d", stageNum);

	Sprite::Instance()->Draw(GameOverGraph, deadGraphPos, window_width, window_height);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
#endif
}

bool PlayScene::GetSceneFlag()
{
	return sceneFlag;
}
