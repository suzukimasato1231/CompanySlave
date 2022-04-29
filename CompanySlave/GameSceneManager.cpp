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
	safe_delete(play);
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
	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList());

	title = new Title();
	title->Initialize(directX);
	select = new SelectScene();
	select->Initialize(directX);
	play = new PlayScene();
	play->Initialize();
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
	scene = titleScene;
}

void GameSceneManager::Update()
{
	//タイトル
	if (scene == titleScene) {
		if (initFlag == true)
		{
			initFlag = false;
			tFadeFlag = false;
			tFade = 1;
		}
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
			initFlag = true;
			tFadeFlag = false;
		}
		title->SetFade(tFade);


		title->Update();

	}
	//ステージ選択
	else if (scene == selectScene) {
		if (initFlag == true)
		{
			sFadeFlag = false;
			sFade = 1;
			LoadCount2 = 0;
			LoadTime = 70;
			initFlag = false;
		}
		if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
		{
			//プレイ開始
			if (select->GetStage() == 1) {
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
		if (play->GetSceneFlag() == true)
		{
			scene = titleScene;
			initFlag = true;
		}
	}
	//ロード画面
	if (LoadFlag == true) {
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
		Sprite::Instance()->Draw(BGGraph, Vec2(0, 0), window_width, window_height);
		Sprite::Instance()->Draw(LoadUIGraph[LoadCount], Vec2(420, 280), 500, 150);
		debugText.Print(10, 380, 2, "%d", LoadTime);
	}
	debugText.Print(10, 260, 2, "R:reset");
	debugText.Print(10, 300, 2, "DebugStageNum %d", stageDebug);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
