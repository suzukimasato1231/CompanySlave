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
	sound1 = Audio::SoundLoadWave("Resources/Music/BGM/heiannoyoi.wav");
	//スプライト画像読み込み
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	for (int i = 0; i < rainMax; i++) {
		rain[i] = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	}
	titleGraph = Sprite::Instance()->SpriteCreate(L"Resources/TitleText.png");
	startGraph = Sprite::Instance()->SpriteCreate(L"Resources/Start.png");
	
	Button[0] = Sprite::Instance()->SpriteCreate(L"Resources/SelectB.png");
	Button[1] = Sprite::Instance()->SpriteCreate(L"Resources/SelectB2.png");

	VolumeUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI.png");
	VolumeUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/Volume.png");
	VolumeUI[2] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeArrow1.png");
	VolumeUI[3] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeArrow2.png");


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
	ButtonFlag = false;
	sceneChangeFlag = false;
	//フェード
	//fade = 1;
	volumeFlag = 0;
	volumeArrowFlag = false;
	volume = 1;
	volume2 = 1;
	volumeB = 500;

	scene = 0;

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
	Direction();



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
		if (scene == 0) {
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
	}
	if (scene == 0) {
		if (titleTextFlag == true) {
			if (fade < 1) {
				fade += 0.01f;
			}
			if (fade >= 1) {
				fade2 += 0.01f;
			}
		}
		if (fade2 >= 1) {
			ButtonFlag = true;
		}
		if (ButtonFlag == true) {
			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				volume2 = 1;
				scene = 1;
				sceneChangeFlag = true;
			}
		}
	}

	else if (scene == 1) {
		//音量調節
		if (Input::Instance()->ControllerDown(ButtonB)) {
			volumeFlag++;
		}
		if (volumeFlag == 2) {
			volumeFlag = 0;
		}
		if (Input::Instance()->ConLeftInput())
		{


			if (direction == Right) {
				volumeArrowFlag = true;
			}
			else if (direction == Left) {
				volumeArrowFlag = false;
			}
		}
		if (volumeFlag == 1) {
			if (volumeArrowFlag == true) {
				if (Input::Instance()->ControllerDown(ButtonA)) {

					if (volume < 1) {
						volume += 0.1;
						volume2 += 0.1;
						volumeB += 50;
					}
				}
			}

			if (volumeArrowFlag == false) {
				if (Input::Instance()->ControllerDown(ButtonA)) {

					if (volume >= 0) {
						volume -= 0.1;
						volume2 -= 0.1;
						volumeB -= 50;
					}
				}
			}
		}
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

void Title::Direction()
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
	Object::Instance()->Draw(swordObject, position, Vec3{ 1,1,1 }, Vec3{ 0,-90,-90 }, Vec4{ 1,1,1,1 });

	if (scene == 0) {
		if (titleTextFlag == true) {
			Sprite::Instance()->Draw(titleGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade });
		}

		Sprite::Instance()->Draw(startGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade2 });
	}
	
	else if (scene == 1) {
		if (volumeFlag == false) {
			Sprite::Instance()->Draw(Button[0], Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
		}
		//音量調節バーの描画
		if (volumeFlag == 1) {
			Sprite::Instance()->Draw(VolumeUI[1], Vec2(420, 340), volumeB, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));
			Sprite::Instance()->Draw(VolumeUI[0], Vec2(420, 340), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));
			if (volumeArrowFlag == false) {
				Sprite::Instance()->Draw(VolumeUI[2], Vec2(370, 340), 600, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));
			}
			if (volumeArrowFlag == true) {
				Sprite::Instance()->Draw(VolumeUI[3], Vec2(370, 340), 600, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));
			}
			Sprite::Instance()->Draw(Button[1], Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));

		}
	}

	//デバックテキスト%dと%f対応
	debugText.Print(10, 40, 2, "%d",volumeArrowFlag);
	debugText.Print(10, 80, 2, "%d", volumeFlag);

	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
}
