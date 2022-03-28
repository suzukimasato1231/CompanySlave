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
	safe_delete(select);
}

void GameSceneManager::Initialize(_DirectX *directX)
{
	assert(directX);
	this->directX = directX;
	title = new Title();
	title->Initialize(directX);
	select = new SelectScene();
	select->Initialize(directX);
	//play = new PlayScene();
	//play->Initialize(directX);
	ParticleManager::StaticInitialize(directX->GetDevice(), directX->GetCmandList(), window_width, window_height);
	//スプライトクラス作成
	Sprite::Instance()->Init();
	//デバックテキスト初期化
	debugText.Initialize();
	//FBX初期化
	FBXObject3d::SetDevice(directX->GetDevice());
	FBXObject3d::SetCmdList(directX->GetCmandList());
	FBXObject3d::CreateGraphicsPipeline();
	//図形モデル初期化
	Shape::Init(directX->GetDevice());
	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList());
}

void GameSceneManager::Init()
{
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/Loading.png");

	LoadUIGraph[0] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load1.png");
	LoadUIGraph[1] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load2.png");
	LoadUIGraph[2] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load3.png");
	LoadUIGraph[3] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load4.png");
	LoadUIGraph[4] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load5.png");
	LoadUIGraph[5] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load6.png");
	LoadUIGraph[6] = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load7.png");

	title->Init();
	select->Init();
	//play->Init();
	scene = titleScene;
}

void GameSceneManager::Update()
{
	//タイトル
	if (scene == titleScene) {
		if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
		{
			tFadeFlag = true;
			//scene = selectScene;
		}
		//フェード
		if (tFadeFlag == true) {
			tFade -= 0.2f;
		}
		if (tFade < 0) {
			scene = selectScene;
			tFadeFlag = false;
		}
		title->SetFade(tFade);


		title->Update();

	}
	//ステージ選択
	else if (scene == selectScene) {
		if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
		{
			//ステージ1の初期化
			if (select->GetStage() == 1) {
				play = new PlayScene();
				play->Initialize(directX);
				play->Init();
			
				sFadeFlag = true;
			}
		}
		//フェード
		if (sFadeFlag == true) {
			sFade -= 0.2f;
		}
		if (sFade < 0) {
			scene = stage1;
			sFadeFlag = false;
			LoadFlag = true;
		}
		select->SetFade(sFade);

		select->Update();
	}
	//ステージ1
	else if (scene == stage1) {
		play->Update();
	}
	//ロード画面
	if (LoadFlag == true) {
		//play = new PlayScene();
		//play->Initialize(directX);
		//play->Init();
		LoadTime-=1;
		if (LoadTime > 0) {
			
			if (LoadCount < 6) {
				LoadCount++;
			}
			else if (LoadCount == 6) {
				LoadCount = 0;
			}
		}
		if (LoadTime == 0) {
			LoadCount = 0;
			LoadFlag = false;
		}

	}
}

void GameSceneManager::Draw()
{

	if (scene == titleScene)
	{
		title->Draw();
	}
	else if (scene == selectScene)
	{
		select->Draw();
	}
	else if (scene == stage1)
	{
		play->Draw();
	}

	if (LoadFlag == true) {
		Sprite::Instance()->Draw(BGGraph, Vec2(0,0), window_width, window_height);
		Sprite::Instance()->Draw(LoadUIGraph[LoadCount], Vec2(420, 280), 500, 150);
		debugText.Print(10, 380, 2, "%d", LoadTime);
	}
	


	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
