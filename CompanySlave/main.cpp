#include<Windows.h>
#include"_Window.h"
#include"_DirectX.h"
#include"Safe_delete.h"
#include"Input.h"
#include"GameSceneManager.h"
#include"Sprite.h"
#include"DebugText.h"
const int window_width = 1280;//横幅
const int window_height = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	//汎用機能
	_DirectX *directX = nullptr;
	Sprite *sprite = nullptr;
	DebugText *debugText = nullptr;
	directX = new _DirectX();
	//ゲームウィンドウの作成
	_Window::Instance()->CreateGameWindow();
	// DirectX初期化処理
	directX->Initilize();
	//入力の初期化
	Input::Instance()->Initialize();	
	//ライト静的初期化
	LightGroup::StaticInitialize(directX->GetDevice());
	//スプライト静的初期化
	Sprite::StaticInit(directX->GetDevice(), directX->GetCmandList());
	//ゲームシーン
	GameSceneManager::Instance()->Initialize(directX);
	GameSceneManager::Instance()->Init();

	while (true)  // ゲームループ
	{
		//メッセージ処理
		if (_Window::Instance()->ProcessMessage()) { break; }

		Input::Instance()->Update();

		GameSceneManager::Instance()->Update();

		//描画開始
		directX->PreDraw();

		GameSceneManager::Instance()->Draw();

		directX->ResourceBarrier();
		//描画コマンドはここまで
	}
#pragma region	//データ解放
	safe_delete(directX);
	//ゲームウィンドウの破棄
	_Window::Instance()->TerminateGameWindow();

	return 0;
}
