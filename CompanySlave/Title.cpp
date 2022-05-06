#include "Title.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

Title::Title()
{}
Title::~Title()
{
	safe_delete(lightGroup);
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	Audio::SoundUnload(&sound1);
	safe_delete(audio);
}
void Title::Initialize()
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

	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/Music/heiannoyoi.wav");
	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	for (int i = 0; i < rainMax; i++) {
		rain[i] = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	}
	titleGraph = Sprite::Instance()->SpriteCreate(L"Resources/TitleText.png");
	startGraph = Sprite::Instance()->SpriteCreate(L"Resources/Start.png");
	swordObject = Object::Instance()->CreateOBJ("sword");

}

void Title::Init()
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
	srand(time(NULL));
	fade = 0;
	fade2 = 0;
	titleTextFlag = false;
	rainFlag = false;
	bottanFlag = false;
	for (int i = 0; i < rainMax; i++) {
		//xはランダムな位置にしている
		//サイズもランダムにしている
		pos[i].y = rand()%-100;
		pos[i].x = rand()%1300;
		s[i] = rand() % 500-400;
	}
	audio->SoundBGMPlayLoopWave(sound1);
}

void Title::Update()
{
	//
	// 	//パーティクル初期化
	ParticleManager::SetCamera(camera);
		//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	if (rainFlag == false) {
		if (rainTime < 30) {
			rainTime++;
		}
		if (rainTime >= 30)
		{
			rainFlag = true;
			rainTime = 0;
		}
	}

	if (rainFlag == true) {
		for (int i = 0; i < rainMax; i++) {

			if (pos[i].y < 720) {
				//雨が地面につくまで降る
				pos[i].y = pos[i].y + v;
				v = g + v;
			}
			if (pos[i].y >= 720) {
				//地面についたらまた上に行く
				pos[i].y = rand() % -100;
				pos[i].x = rand() % 1300;
				v = 0;
				g = 9.8f;
				s[i] = rand() % 500 - 400;
			}
		}
		if (titleTextFlag == false) {
			if (titleTime < 60) {
				titleTime++;
			}
			if (titleTime >= 60)
			{
				titleTextFlag = true;
				titleTime = 0;
			}
		}
	}
		if (titleTextFlag == true) {
			if (fade < 1) {
				fade+=0.01f;
			}
			if (fade >= 1) {
				fade2 += 0.01f;
			}
		}
		if (fade2 >= 1) {
			bottanFlag = true;
		}
	//パーティクル更新
	//particleMan->Update();
	//particleMan2->Update();
	
	//ライト更新
	lightGroup->Update();
}

void Title::AudioUpdate()
{

	if (audioFlag == false) {
		audio->SoundStop();
	}
	audio->SetVolume(volume);

}

void Title::Draw()
{
	//背景描画
	//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
	Sprite::Instance()->Draw(BGGraph, {0,0}, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1});
	for (int i = 0; i < rainMax; i++) {
		if (rainFlag == true) {
			Sprite::Instance()->Draw(rain[i], pos[i], 1, s[i], { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
		}
	}
	if (titleTextFlag == true) {
		Sprite::Instance()->Draw(titleGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade });
	}

		Sprite::Instance()->Draw(startGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade2 });
	
	Object::Instance()->Draw(swordObject, position, Vec3{1,1,1 }, Vec3{ 0,-90,-90 }, Vec4{1,1,1,1});

	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "%f",fade);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
