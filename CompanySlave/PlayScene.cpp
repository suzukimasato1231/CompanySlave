#include "PlayScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

PlayScene::PlayScene()
{}
PlayScene::~PlayScene()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(particleMan4);
	safe_delete(lightGroup);
	safe_delete(fbxObject1);
	safe_delete(model1);
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
	//FBX初期化
	FBXObject3d::SetCamera(camera);
	//パーティクル初期化
	ParticleManager::SetCamera(camera);
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);
}

void PlayScene::Init()
{
	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/i.wav");

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

	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/text2.jpg");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/back.png");
	Parent = Sprite::Instance()->SpriteCreate(L"Resources/text2.jpg");
	controlGraph = Sprite::Instance()->SpriteCreate(L"Resources/ControlUI/ControlUI.png");
	GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver.png");
	//3Dオブジェクト画像読み込み
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	graph1 = Object::Instance()->LoadTexture(L"Resources/texture2.jpg");

	//3Dobjファイル読み込み。

	//Shapeクラスに決まった形のオブジェクトを作成	

	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3Dオブジェクトの生成とモデルのセット
	fbxObject1 = new FBXObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(model1);

	//パーティクルクラス作成

	particleMan = ParticleManager::Create(L"Resources/particle.jpg");

	particleMan4 = ParticleManager::Create(L"Resources/particle.jpg");

	particleMan2 = ParticleManager::Create(L"Resources/Blood/Blood.png");

	//マップチップの初期化
	mapStage = new MapStage;
	mapStage->Init();
	//プレイヤーの初期化
	player = new Player;
	player->Init();
	//敵
	enemy = new Enemy;
	enemy->Init();

	//ステージ初期化
	stageFlag = true;
	stageNum = 1;
	StageInit();


}

void PlayScene::StageInit()
{
	if (stageFlag == true)
	{
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
		default:
			break;
		}
		stageNum++;
		stageFlag = false;
	}
}

void PlayScene::Update()
{
	//nextステージ

	StageInit();



	//プレイヤーの更新
	mapStage->Update(enemy);

	player->Update(enemy);

	enemy->Update(player);


	//マップチップとプレイヤーの押し戻し処理
	PushCollision::Player2Mapchip(player, enemy, mapStage, stageFlag);


	fbxObject1->Update();
	//
	camera->FollowCamera(player->GetPosition(), Vec3{ 0,100,-10 }, 0.0f, -15.0f);

	//パーティクル追加
	if (player->GetMoveFlag() == true)
	{
		particleMan4->ParticleAdd(player->GetPosition(), Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));
	}


	//パーティクル追加
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{

		if (enemy->GetParticleFlag(i) == true) {

			if (enemy->GetParticleTime(i) > 0) {

				particleMan2->ParticleAdd(enemy->GetPosition(i), Vec4(0.2f, 0.2f, 0.2f, 1.0f), Vec4(0.2f, 0.2f, 0.2f, 1.0f));
			}
		}

	}
	particleMan->ParticleAdd2(Vec3{ 0 + 74 * 10,2, 0 + 6 * (-10) }, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));
	particleMan->ParticleAdd2(Vec3{ 0 + 74 * 10,2, 0 + 5 * (-10) }, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));

	if (player->GetHP() <= 0 && deadGraphPos.y < 0)
	{
		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		deadGraphPos.y = -800 * (1.0f - (timeRate * timeRate) * (3 - (2 * timeRate))) + 0 * (timeRate * timeRate) * (3 - (2 * timeRate));
	}

	//プレイシーンを抜ける
	if (Input::Instance()->ControllerDown(ButtonA) && timeRate == 1)
	{
		deadGraphPos.y = -800;
		nowTime = 0;
		endTime = 5;
		timeRate = 0;
		sceneFlag = true;
	}

	particleMan4->Update();

	particleMan2->Update();
	//パーティクル更新
	particleMan->Update();




	//ライト更新
	lightGroup->Update();
}

void PlayScene::Draw()
{
	//オブジェクト描画前処理
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);

	//オブジェクト
	//Object::Instance()->Draw(BossPolygon, pPos1, Vec3{ 1.0f,1.0f,1.0f }, angle, Vec4{ 1.0f,1.0f,1.0f ,1.0f });

	//マップチップの描画
	mapStage->Draw(player->GetPosition());

	//敵の血痕
	enemy->BloodDraw();

	//プレイヤーの描画
	player->Draw();

	enemy->Draw();

	//パーティクル描画

	if (mapStage->GetMap(74, 6) == 5)
	{
		particleMan->Draw();
	}

	particleMan4->Draw();

	particleMan2->Draw();
	//particleMan3->Draw();

	//前景描画
	player->UIDraw();
	Sprite::Instance()->Draw(controlGraph, Vec2(0, 0), window_width, window_height);
#if _DEBUG
	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "E:button");

	debugText.Print(10, 80, 2, "D:Attack");

	debugText.Print(10, 120, 2, "F:kamikaihi");

	debugText.Print(10, 180, 2, "%d", enemy->GetParticleTime(0));

	Sprite::Instance()->Draw(GameOverGraph, deadGraphPos, window_width, window_height);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
#endif
}

bool PlayScene::GetSceneFlag()
{
	return sceneFlag;
}
