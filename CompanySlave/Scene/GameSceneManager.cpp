#include "GameSceneManager.h"
#include"Input.h"
#include"Shape.h"
#include<sstream>
#include<iomanip>
GameSceneManager::GameSceneManager()
{
}

GameSceneManager::~GameSceneManager()
{
	safe_delete(title);
	safe_delete(play);
	safe_delete(clear);
}

void GameSceneManager::Initialize(_DirectX* directX)
{
	assert(directX);
	this->directX = directX;
	ParticleManager::StaticInitialize(directX->GetDevice(), directX->GetCmandList(), window_width, window_height);
	//スプライトクラス作成
	Sprite::Instance()->Init();
	//デバックテキスト初期化
	debugText.Initialize();
	//図形モデル初期化
	Shape::Init(directX->GetDevice());

	//カメラクラス作成
	camera = Camera::Create();
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();


	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList());
	Object::Instance()->LoadTexture(L"Resources/white1x1.png");



	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/Loading.png");
	LoadUIGraph[0] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load1.png");
	LoadUIGraph[1] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load2.png");
	LoadUIGraph[2] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load3.png");
	LoadUIGraph[3] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load4.png");
	LoadUIGraph[4] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load5.png");
	LoadUIGraph[5] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load6.png");
	LoadUIGraph[6] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load7.png");
	//sword = Object::Instance()->CreateOBJ("Effect");

	title = new Title();
	title->Initialize();

	play = new PlayScene();
	play->Initialize();

	clear = new Clear();
	clear->Initialize();
}

void GameSceneManager::Init()
{
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);


	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });


	title->Init();

	scene = titleScene;
}

void GameSceneManager::Update()
{
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);


	//タイトル
	if (scene == titleScene) {

		if (initFlag == true)
		{
			initFlag = false;
			tFadeFlag = false;
			tFade = 1;
		
			LoadCount2 = 0;
			LoadTime = 70;
		}
		volume = title->GetVolume();
		if (title->GetVolumeFlag() == 0 && title->GetSceneChangeFlag() == true) {

			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				initFlag = true;
				title->SetAudioFlag(false);

				scene = stage1;
				play->Init();
				LoadFlag = true;
			}
		}
		//title->SetVolume(volume);
		play->SetVolume(volume);
		clear->SetVolume(volume);

		title->AudioUpdate();
		title->Update();
	}

	//ステージ1
	else if (scene == stage1) {

		if (play->GetSceneFlag() == true)
		{
			title->Init();
			title->SetAudioFlag(true);
			scene = titleScene;
			initFlag = true;
		}
		//この数値を変えればクリアシーンに行けるステージを変えられる↓
		if (play->GetSceneChangeFlag() == true && play->GetStageNum() == 10) {
			clear->Init();
			clear->SetAudioFlag(true);
			scene = clearScene;
			initFlag = true;
		}

		play->Update();
		if (LoadFlag == false) {
			play->SetUpdateFlag(false);
		}
	}
	//クリアシーン
	else if (scene == clearScene) {

		if (clear->GetButtonFlag() == true) {

			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				clear->SetAudioFlag(false);
				title->SetAudioFlag(true);
				initFlag = true;
				title->Init();
				scene = titleScene;
			}
		}
		clear->Update();
	}
	//ロード画面
	if (LoadFlag == true) {
		play->SetUpdateFlag(true);
		play->SetLoadFlag(true);
		LoadTime -= 1;
		if (LoadTime > 0) {
			LoadCount2++;
			if (LoadCount < 6 && LoadCount2 >= 10) {
				LoadCount++;
				LoadCount2 = 0;
			}
			else if (LoadCount == 6) {
				LoadCount = 0;
			}
		}
		if (LoadTime <= 0) {
			LoadCount = 0;
			play->SetLoadFlag(false);
			LoadFlag = false;
		}
	}

	//デバック用リセットボタン
	if (Input::Instance()->KeybordTrigger(DIK_R))
	{
		play->Init();

		play->SetStageDebug(stageDebug);
	}

	if (Input::Instance()->KeybordTrigger(DIK_1))
	{
		stageDebug = 1;
	}
	if (Input::Instance()->KeybordTrigger(DIK_2))
	{
		stageDebug = 2;
	}
	if (Input::Instance()->KeybordTrigger(DIK_3))
	{
		stageDebug = 3;
	}
	if (Input::Instance()->KeybordTrigger(DIK_4))
	{
		stageDebug = 4;
	}
	if (Input::Instance()->KeybordTrigger(DIK_5))
	{
		stageDebug = 5;
	}
	if (Input::Instance()->KeybordTrigger(DIK_6))
	{
		stageDebug = 6;
	}
	if (Input::Instance()->KeybordTrigger(DIK_7))
	{
		stageDebug = 7;
	}
	if (Input::Instance()->KeybordTrigger(DIK_8))
	{
		stageDebug = 8;
	}
	if (Input::Instance()->KeybordTrigger(DIK_9))
	{
		stageDebug = 9;
	}
}

void GameSceneManager::Draw()
{
	//オブジェクト描画前処理
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();
	if (scene == titleScene)
	{
		title->Draw();
	}
	else if (scene == stage1)
	{
		play->Draw();
	}
	else if (scene == clearScene)
	{
		clear->Draw();
	}

	if (LoadFlag == true) {
		Sprite::Instance()->Draw(BGGraph, Vec2(0, 0), window_width, window_height);
		Sprite::Instance()->Draw(LoadUIGraph[LoadCount], Vec2(420, 280), 500, 150);

	}
	play->LoadDraw();
#if _DEBUG
	//debugText.Print(10, 380, 2, "%d", LoadTime);
	debugText.Print(10, 260, 2, "R:reset");
	debugText.Print(10, 300, 2, "DebugStageNum %d", stageDebug);
	debugText.Print(10, 360, 2, "%f", volume);
	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
#endif
}
