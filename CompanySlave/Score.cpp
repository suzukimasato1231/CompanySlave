#include "Score.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"


Score::Score()
{}
Score::~Score()
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
void Score::Initialize(_DirectX* directX)
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
	ParticleManager::SetCamera(camera);	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);
}

void Score::Init()
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
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/Score.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
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
	//particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	//particleMan2 = ParticleManager::Create(L"Resources/particle2.png", 1);



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

void Score::Update()
{

	fbxObject1->Update();

	//パーティクル更新
	//particleMan->Update();
	//particleMan2->Update();

	//ライト更新
	lightGroup->Update();
}

void Score::Draw()
{
	//オブジェクト描画前処理
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));


	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "Score");



	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
