#include "SelectScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

SelectScene::SelectScene()
{}
SelectScene::~SelectScene()
{
	safe_delete(lightGroup);
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	safe_delete(audio);
}
void SelectScene::Initialize()
{

	//Audioクラス作成
	audio = Audio::Create();
	//カメラクラス作成
	camera = Camera::Create();
	//パーティクル初期化
	ParticleManager::SetCamera(camera);
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();
	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/i.wav");

	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/Loading.png");
	Bottan = Sprite::Instance()->SpriteCreate(L"Resources/bottan.png");

	VolumeUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI.png");
	VolumeUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI2.png");
	VolumeUI[2] = Sprite::Instance()->SpriteCreate(L"Resources/Volume.png");

	LoadUIGraph = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load7.png");
	
	//3Dオブジェクト画像読み込み
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	//3Dobjファイル読み込み。
	//パーティクルクラス作成
	//particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	//particleMan2 = ParticleManager::Create(L"Resources/text2.jpg", 1);
	
}

void SelectScene::Init()
{
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
	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//フェード
	fade = 1;
	volumeFlag = false;
	volume = 1;
	volume2 = 1;
	volumeB = 500;
	
	//audio->SoundBGMPlayLoopWave(sound1);
	
}

void SelectScene::Update()
{
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);


	if(audioFlag==false) {
		//audio->SoundStop();
	}
	audio->SetVolume(volume);

	Direction();

	//音量調節
	//if (Input::Instance()->ConLeftInput())
	//{
	//	if (direction == Down) {
	//		volumeFlag = true;
	//	}
	//	if (direction == Up) {
	//		volumeFlag = false;
	//	}
	//	if (volumeFlag == true) {
	//		if (direction == Right) {
	//			if (volume < 1) {
	//				volume += 0.1;
	//				volume2 += 0.1;
	//				volumeB += 50;
	//			}
	//		}
	//		if (direction == Left) {
	//			if (volume >= 0) {
	//				volume -= 0.1;
	//				volume2 -= 0.1;
	//				volumeB -= 50;
	//			}
	//		}
	//	}
	//}



	//ライト更新
	lightGroup->Update();
}
void SelectScene::Direction()
{
	//右上
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		direction = UpRight;
	}//右下
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownRight;
	}//左下
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownLeft;
	}//左上
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		direction = UpLeft;
	}//上
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		direction = Up;
	}//右
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		direction = Right;
	}//下
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		direction = Down;
	}//左
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		direction = Left;
	}
}
void SelectScene::Draw()
{
	//オブジェクト描画前処理
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(LoadUIGraph, Vec2(420, 280), 500, 150);

	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	
	Sprite::Instance()->Draw(Bottan, Vec2(580, 180), 200, 200, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//音量調節バーの描画
	//	Sprite::Instance()->Draw(VolumeUI[2], Vec2(380, 480), volumeB, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	//if (volumeFlag == false) {
	//	Sprite::Instance()->Draw(VolumeUI[0], Vec2(380, 480), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//}
	//else if (volumeFlag == true) {
	//	Sprite::Instance()->Draw(VolumeUI[1], Vec2(380, 480), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//}

	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "%f",volume);



	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}