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
	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList());
	Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	title = new Title();
	title->Initialize();

	select = new SelectScene();
	select->Initialize();
	
	play = new PlayScene();
	play->Initialize();
	
	clear = new Clear();
	clear->Initialize();
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
	//clear->Init();
	//play->Init();
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
		if (title->GetBottanFlag() == true) {

			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{


				volume = 1;
				initFlag = true;
				select->Init();
				scene = selectScene;
			}
		}
		title->AudioUpdate();
		title->Update();
	}
	//ステージ選択
	else if (scene == selectScene) {
		//play->SetVolume(0);
		
		volume = select->GetVolume();
		if (initFlag == true)
		{
			sFadeFlag = false;
			sFade = 1;
			LoadCount2 = 0;
			LoadTime = 70;
			initFlag = false;

		}
		if (select->GetVolumeFlag() == 0) {
			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				//プレイ開始

				title->SetAudioFlag(false);

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
		title->SetVolume(volume);
		play->SetVolume(volume);
		clear->SetVolume(volume);
		select->SetFade(sFade);
		title->AudioUpdate();
		select->Update();
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
		if (play->GetSceneChangeFlag() == true&&play->GetStageNum()==10) {
			clear->Init();
			clear->SetAudioFlag(true);
			scene = clearScene;
			initFlag = true;
		}
		play->Update();
	}
	//クリアシーン
	else if (scene == clearScene) {
		
		if (clear->GetBottanFlag() == true) {

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
	else if (scene == selectScene)
	{
		select->Draw();
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
		debugText.Print(10, 380, 2, "%d", LoadTime);
	}
	debugText.Print(10, 260, 2, "R:reset");
	debugText.Print(10, 300, 2, "DebugStageNum %d", stageDebug);
	debugText.Print(10, 360, 2, "%f", volume);
	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
