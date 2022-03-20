#include "GameSceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

GameSceneManager::GameSceneManager()
{}
GameSceneManager::~GameSceneManager()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
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
void GameSceneManager::Initialize(_DirectX *directX)
{
	assert(directX);
	this->directX = directX;
	//Audioクラス作成
	audio = Audio::Create();
	//カメラクラス作成
	camera = Camera::Create();
	//スプライトクラス作成
	Sprite::Instance()->Init();
	//FBX初期化
	FBXObject3d::SetDevice(directX->GetDevice());
	FBXObject3d::SetCmdList(directX->GetCmandList());
	FBXObject3d::SetCamera(camera);
	FBXObject3d::CreateGraphicsPipeline();
	//図形モデル初期化
	Shape::Init(directX->GetDevice());
	//パーティクル初期化
	ParticleManager::StaticInitialize(directX->GetDevice(), directX->GetCmandList(), this->camera, window_width, window_height);
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();
	//3Dオブジェクト初期化
	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList(), camera, lightGroup);
}

void GameSceneManager::Init()
{
	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/i.wav");
	//sound2 = Audio::SoundLoadWave("Resources/BGM.wav");

	//読み込んだ音データを1回だけ流す
	//sound->SoundSEPlayWave(sound1);

	//読み込んだ音データをループで流す
	//sound->SoundBGMPlayLoopWave(sound2, sound->BGM);

	// 3Dオブエクトにライトをセット
	//Object3d::SetLightGroup(lightGroup);
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
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/backgroundA.png");
	Parent = Sprite::Instance()->SpriteCreate(L"Resources/text2.jpg");

	//3Dオブジェクト画像読み込み
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	graph1 = Object::Instance()->LoadTexture(L"Resources/texture2.jpg");

	//3Dobjファイル読み込み。
	//Polygon = Object::Instance()->CreateOBJ("Boss");
	BossPolygon = Object::Instance()->CreateOBJ("sphere", true);

	//Shapeクラスに決まった形のオブジェクトを作成	
	Polygon = Shape::CreateSquare(20.0f, 20.0f, 20.0f);

	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3Dオブジェクトの生成とモデルのセット
	fbxObject1 = new FBXObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(model1);

	//パーティクルクラス作成
	particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	particleMan2 = ParticleManager::Create(L"Resources/text2.jpg", 1);

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

void GameSceneManager::Update()
{
	//キー入力
	//Inputクラスにマウスとコントローラもあるよ
	//インスタンス化してるのでInput/Input.h"を持ってくればどのクラスでも使えるよ
	if (Input::Instance()->KeybordPush(DIK_UP))
	{
		//	fbxObject1->PlayAnimation();
	}
	if (Input::Instance()->KeybordPush(DIK_DOWN))
	{
	}
	if (Input::Instance()->KeybordPush(DIK_LEFT))
	{
	}
	if (Input::Instance()->KeybordPush(DIK_RIGHT))
	{
	}
	//プレイヤーの更新
	player->Update(enemy);

	enemy->Update(player);

	//マップチップとプレイヤーの押し戻し処理
	PushCollision::Player2Mapchip(player, enemy, mapStage);


	fbxObject1->Update();
	//
	camera->FollowCamera(player->GetPosition(), Vec3{ 0,100,-10 });

	//パーティクル追加
	if (player->GetMoveFlag() == true)
	{
		particleMan->ParticleAdd(player->GetPosition(), Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));
	}

	//パーティクル追加
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		if (enemy->GetWasAttackFlag(i) == true)
		{
			particleMan->ParticleAdd(enemy->GetPosition(i), Vec4(1.0f, 0.0f, 0.0f, 1.0f), Vec4(1.0f, 0.4f, 0.0f, 1.0f));
		}
	}
	//パーティクル更新
	particleMan->Update();

	particleMan2->Update();
	//ライト更新
	lightGroup->Update();
}

void GameSceneManager::Draw()
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
	mapStage->Draw();
	//プレイヤーの描画
	player->Draw();

	enemy->Draw();


	//fbxObject1->Draw();


	//パーティクル描画
	particleMan->Draw();

	particleMan2->Draw();

	//前景描画
	//Sprite::Instance()->Draw(spriteGraph, Vec2(400, 400), 100, 100, Vec2(0.5f, 0.5f));
#if _DEBUG
	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "E:button");

	debugText.Print(10, 80, 2, "WASD+C:Attack");

	debugText.Print(10, 120, 2, "SPACE:noutou");

	debugText.Print(10, 160, 2, "comboNum:%d", player->GetComboNum());

	debugText.Print(10, 200, 2, "comboTime:%d", player->GetComboTime());

	debugText.Print(10, 240, 2, "coolTime:%d", player->GetCoolTime());


	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
#endif
}
