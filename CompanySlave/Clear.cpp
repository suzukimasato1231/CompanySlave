#include "Clear.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

Clear::Clear()
{}
Clear::~Clear()
{
	safe_delete(lightGroup);
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	Audio::SoundUnload(&sound1);
	safe_delete(audio);
}
void Clear::Initialize()
{
	//Audioクラス作成
	audio = Audio::Create();
	//カメラクラス作成
	camera = Camera::Create();

	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();

	////3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);


	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/i.wav");
	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");

	textGraph = Sprite::Instance()->SpriteCreate(L"Resources/Text.png");
	title = Sprite::Instance()->SpriteCreate(L"Resources/clearTitle.png");

	
	PlayerObject = Object::Instance()->CreateOBJ("playerKari1");

}

void Clear::Init()
{

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//カメラ位置をセット
	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	color = 1;
	colorTimer = 60;
	TextFlag = false;
	bottanFlag = false;

	//audio->SoundBGMPlayLoopWave(sound1);
}

void Clear::Update()
{
	//
	//// 	//パーティクル初期化
	//ParticleManager::SetCamera(camera);
	//////3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	if (audioFlag == false) {
		//audio->SoundStop();
	}
	audio->SetVolume(volume);

	if (colorTimer > 0) {
		colorTimer--;
	}
	else if (colorTimer <= 0) {
		colorFlag = true;
	}
	if (colorFlag == true) {
		if (color > 0.1)
		{
			color -= 0.01f;
		}
		else if (color <= 0.1) {
			TextFlag = true;
			bottanFlag = true;
		}

	}

	//パーティクル更新
	//particleMan->Update();
	//particleMan2->Update();

	//ライト更新
	lightGroup->Update();
}

void Clear ::Draw()
{
	//オブジェクト描画前処理

	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, { 0,0 }, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { color, color,color ,1 });


	if (colorFlag == true) {
		Object::Instance()->Draw(PlayerObject, position, Vec3{ 1,1,1 }, Vec3{ 0,-90,40 }, Vec4{ 1,1,1,1});
	}
	if (TextFlag == true) {

		Sprite::Instance()->Draw(textGraph, { 440,200 }, 400, 400, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
		Sprite::Instance()->Draw(title, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });

	}
	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "%f", color);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
