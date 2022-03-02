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
	safe_delete(lightGroup);
	safe_delete(fbxObject1);
	safe_delete(model1);
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
	//パーティクルクラス作成
	particleMan = ParticleManager::Create();
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
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//カメラ位置をセット
	camera->SetCamera(Vec3{ 0,20,-50 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()-> SpriteCreate(L"Resources/text2.jpg");
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
	
}

void GameSceneManager::Update()
{
	//キー入力
	//Inputクラスにマウスとコントローラもあるよ
	//インスタンス化してるのでInput/Input.h"を持ってくればどのクラスでも使えるよ
	if (Input::Instance()->KeybordPush(DIK_UP))
	{		
		fbxObject1->PlayAnimation();
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
	fbxObject1->Update();

	//パーティクル追加
	particleMan->ParticleAdd(pPos1);
	//パーティクル更新
	particleMan->Update();
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
	Sprite::Instance()->Draw(BGGraph,pos, window_width, window_height);

	//オブジェクト
	//Object::Instance()->Draw(BossPolygon, pPos1, Vec3{ 1.0f,1.0f,1.0f }, angle, Vec4{ 1.0f,1.0f,1.0f ,1.0f });


	fbxObject1->Draw();


	//パーティクル描画
	particleMan->Draw();

	//前景描画
	//Sprite::Instance()->Draw(spriteGraph, Vec2(400, 400), 100, 100, Vec2(0.5f, 0.5f));

	//デバックテキスト%dと%f対応
	debugText.Print(1, 120, 2, "%d", 1891);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
