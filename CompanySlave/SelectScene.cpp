#include "SelectScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

SelectScene::SelectScene()
{}
SelectScene::~SelectScene()
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
void SelectScene::Initialize(_DirectX* directX)
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

void SelectScene::Init()
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
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/backgroundA.png");
	Bottan = Sprite::Instance()->SpriteCreate(L"Resources/bottan.png");

	Number[0] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/0.png");
	Number[1] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/1.png");
	Number[2] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/2.png");
	Number[3] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/3.png");
	Number[4] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/4.png");
	Number[5] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/5.png");
	Number[6] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/6.png");
	Number[7] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/7.png");
	Number[8] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/8.png");
	Number[9] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/9.png");

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

void SelectScene::Update()
{
	//キー入力
	//Inputクラスにマウスとコントローラもあるよ
	//インスタンス化してるのでInput/Input.h"を持ってくればどのクラスでも使えるよ
	if (Input::Instance()->KeybordTrigger(DIK_UP))
	{
	
	}
	if (Input::Instance()->KeybordTrigger(DIK_DOWN))
	{
		
	}
	//ステージ選択ボタン
	if (Input::Instance()->KeybordTrigger(DIK_LEFT))
	{
		if (stage > 1) {
			stage--;
			if (nCount == 0) {
				nCount = 10;
				if (nCount2 > 1) {
					nCount2--;
				}
			}
			if (nCount > 0&&nCount2!=0) {
				nCount--;
			}
			if (nCount > 1&&nCount2 == 0) {
				nCount--;
			}
		}
		if (stage < 10) {
			nCount2 = 0;
		}
	}
	if (Input::Instance()->KeybordTrigger(DIK_RIGHT))
	{
		if (stage < 30) {
			stage++;

			if (nCount < 9) {
				nCount++;

			}
			if (stage % 10 == 0) {
				nCount2++;
				nCount = 0;
			}
		}
	}


	fbxObject1->Update();

	//パーティクル更新
	particleMan->Update();
	particleMan2->Update();

	//ライト更新
	lightGroup->Update();
}

void SelectScene::Draw()
{
	//オブジェクト描画前処理
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	
	Sprite::Instance()->Draw(Bottan, Vec2(580, 280), 200, 200, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	
	if (stage < 10) {
		Sprite::Instance()->Draw(Number[nCount], Vec2(630, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	}
	if (stage >= 10) {

		Sprite::Instance()->Draw(Number[nCount2], Vec2(590, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
		Sprite::Instance()->Draw(Number[nCount], Vec2(670, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	}

	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "LeftArrow");
	debugText.Print(10, 80, 2, "RightArrow");


	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
