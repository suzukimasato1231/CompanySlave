#include "PlayScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include<time.h>

PlayScene::PlayScene()
{}
PlayScene::~PlayScene()
{
	safe_delete(lightGroup);
	safe_delete(mapStage);
	safe_delete(player);
	safe_delete(enemy);
	safe_delete(particleAdd);
	//XAudio2解放
	audio->xAudio2.Reset();
	//音データ解放
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	Audio::SoundUnload(&sound3);
	safe_delete(audio);
}
void PlayScene::Initialize()
{
	//Audioクラス作成
	audio = Audio::Create();
	//カメラクラス作成
	camera = Camera::Create();
	//ライトグループクラス作成
	lightGroup = LightGroup::Create();
	//デバックテキスト初期化
	debugText.Initialize();

	//音データ読み込み
	sound1 = Audio::SoundLoadWave("Resources/Music/BGM/rain.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/BGM/Rambu.wav");
	sound3 = Audio::SoundLoadWave("Resources/Music/SE/button.wav");


	//スプライト画像読み込み
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/back.png");
	//controlGraph = Sprite::Instance()->SpriteCreate(L"Resources/ControlUI/ControlUI.png");
	//GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver.png");
	GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	GameOverTextGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver2.png");
	SChangeGraph = Sprite::Instance()->SpriteCreate(L"Resources/SceneChange.png");

	for (int i = 0; i < rainMax; i++) {
		RainOBJ[i] = Shape::CreateRect(1, 1);
	}
	rainGraph = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	tutorial = Shape::CreateRect(11.8f, 10);
	tutorial3 = Shape::CreateRect(10, 13.6f);
	tutorial2 = Shape::CreateRect(12, 6);
	AButton[0] = Object::Instance()->LoadTexture(L"Resources/Button/A1.png");
	AButton[1] = Object::Instance()->LoadTexture(L"Resources/Button/A2.png");
	XButton[0] = Object::Instance()->LoadTexture(L"Resources/Button/X1.png");
	XButton[1] = Object::Instance()->LoadTexture(L"Resources/Button/X2.png");
	RBButton[0] = Object::Instance()->LoadTexture(L"Resources/Button/RB1.png");
	RBButton[1] = Object::Instance()->LoadTexture(L"Resources/Button/RB2.png");
	LBButton[0] = Object::Instance()->LoadTexture(L"Resources/Button/LB1.png");
	LBButton[1] = Object::Instance()->LoadTexture(L"Resources/Button/LB2.png");


	//3Dオブジェクト画像読み込み
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	//パーティクル
	particleAdd = new ParticleAdd;
	particleAdd->Init();

	sword = Object::Instance()->CreateOBJ("Effect");
	sword2 = Object::Instance()->CreateOBJ("sword2");


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

void PlayScene::Init()
{
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//カメラ位置をセット
	camera->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//ステージ初期化
	stageFlag = true;
	stageNum = 1;
	//ループ処理での初期化
	player->LoopInit();

	deadGraphPos = { 0.0f,-800.0f };
	nowTime = 0;//ラープ
	endTime = 5;//ラープ
	timeRate = 0;//ラープ

	deathTime = 20;
	swordTime = 10;
	fade = 0.0f;
	fade2 = 0.0f;
	color = 0.0f;
	stageFlag = true;

	sceneFlag = false;
	sceneChangeFlag = false;
	ChangeGraphPosition = { -2000.0f, 0.0f };

	UpdateFlag = false;

	tutorialAFlag = false;
	AButtonTimer = 30;
	AButtonCount = 0;

	tutorialXFlag = false;
	XButtonTimer = 30;
	XButtonCount = 0;

	tutorialRBFlag = false;
	RBButtonTimer = 30;
	RBButtonCount = 0;

	tutorialLBFlag = false;
	LBButtonTimer = 30;
	LBButtonCount = 0;

	audioFlag = false;

	srand(time(NULL));

	for (int i = 0; i < rainMax; i++) {
		//xとzはランダムな位置にしている
		//サイズもランダムにしている
		position[i].x = (float)rand() / 20;
		position[i].z = (float)rand() / -100;
		position[i].y = 100;
		s[i] = (float)rand() / 1000;
	}
}

void PlayScene::StageInit()
{
	if (stageFlag == true)
	{
		audio->SoundBGMPlayLoopWave(sound1, 0);
		switch (stageNum)
		{
		case 1:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 2:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 3:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			audio->SoundBGMPlayLoopWave(sound2, 1);
			break;
		case 4:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 5:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 6:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			audio->SoundBGMPlayLoopWave(sound2, 1);
			break;
		case 7:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 8:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			break;
		case 9:
			player->StageInit(stageNum);
			enemy->StageInit(stageNum);
			mapStage->StageInit(stageNum);
			audio->SoundBGMPlayLoopWave(sound2, 1);
			break;
		default:
			break;
		}
		stageNum++;
		stageFlag = false;
		sceneChangeFlag = false;
	}
}

void PlayScene::Update()
{
	//パーティクル初期化
	ParticleManager::SetCamera(camera);
	//3Dオブジェクト初期化
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	//nextステージ

	StageInit();

	//プレイヤーの更新
	mapStage->Update(player->GetPosition(), enemy);

	if (UpdateFlag == false) {
		if (sceneChangeFlag == false) {
			if (player->GetHP() > 0)
			{
				player->Update(enemy);
				enemy->Update(player);
			}
		}
	}
	//マップチップとプレイヤーの押し戻し処理
	PushCollision::Player2Mapchip(player, enemy, mapStage, sceneChangeFlag);

	//カメラ調整
	camera->FollowCamera(player->GetCameraPos(), Vec3{ 0,80,-10 }, 0.0f, -15.0f);

	//パーティクル追加
	particleAdd->Update(player, enemy, mapStage);

#pragma region 音関係
	//0が無音
	enemy->SetVolume(volume);
	player->SetVolume(volume);
	audio->SetVolume(volume);

	audioFlag = true;

	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			audioFlag = false;
		}
	}

	if (audioFlag == true && stageNum == 4) {
		audio->SoundStop(1);
	}
	else if (audioFlag == true && stageNum == 7) {
		audio->SoundStop(1);
	}
	else if (audioFlag == true && stageNum == 10) {
		audio->SoundStop(1);
	}

	if (Input::Instance()->KeybordTrigger(DIK_R)) {
		audio->SoundStop(0);
		if (stageNum == 4 || stageNum == 7 || stageNum == 10) {
			audio->SoundStop(1);
		}
	}
#pragma endregion
	//if (player->GetHP() <= 0 && deadGraphPos.y < 0)
	//{
	//	audio->SoundStop(0);
	//	nowTime += 0.1;
	//	timeRate = min(nowTime / endTime, 1);
	//	deadGraphPos.y = -800 * (1.0f - (timeRate * timeRate) * (3 - (2 * timeRate))) + 0 * (timeRate * timeRate) * (3 - (2 * timeRate));
	//	if (stageNum == 4 || stageNum == 7 || stageNum == 10) {
	//		audio->SoundStop(1);
	//	}

	//}
	////プレイシーンを抜ける
	//if (Input::Instance()->ControllerDown(ButtonA) && nowTime >= 2.0f)
	//{
	// 	if (stageNum == 4 || stageNum == 7 || stageNum == 10) {
	//	audio->SoundStop(1);
	//	}
	//	deadGraphPos.y = -800;
	//	nowTime = 0;
	//	endTime = 5;
	//	timeRate = 0;
	//	sceneFlag = true;
	//}

	//ゲームオーバー画面deGozaru

	if (player->GetHP() <= 0)
	{
		audio->SoundStop(0);
		if (stageNum == 4 || stageNum == 7 || stageNum == 10) {
			audio->SoundStop(1);
		}
		if (deathTime > 0) {
			deathTime--;
		}
		if (deathTime <= 0) {
			if (fade < 1.0f) {
				fade += 0.01f;
			}
		}
		if (fade >= 1.0f) {
			if (color < 1.0f) {
				color += 0.01f;

			}
		}
		if (color >= 1.0f) {
			if (swordTime > 0) {
				swordTime--;
			}
			if (swordTime <= 0) {

				fade2 += 0.1f;

			}
		}
	}
	if (Input::Instance()->ControllerDown(ButtonA) && fade2 >= 1.0f)
	{
		audio->SoundSEPlayWave(sound3);
		sceneFlag = true;
	}

	if (sceneChangeFlag == true)
	{
		audio->SoundStop(0);
		//audio->SoundStop(1);
		if (ChangeGraphPosition.x < -100)
		{
			ChangeGraphPosition.x += 64;
		}
		if (ChangeGraphPosition.x >= -100)
		{
			stageFlag = true;
		}
	}
	else if (sceneChangeFlag == false)
	{
		ChangeGraphPosition.x -= 64;
		if (ChangeGraphPosition.x <= -2000) {
			ChangeGraphPosition = { -2000.0f, 0.0f };
		}
	}

	rainSlow = player->GetSlow();
	for (int i = 0; i < rainMax; i++)
	{

		if (position[i].y > 0)
		{
			//雨が地面につくまで降る
			position[i].y = position[i].y - g * rainSlow;
		}
		if (position[i].y <= 0)
		{
			//地面についたらまた上に行く
			//xとzはランダムな位置にしている
			//サイズもランダムにしている
			position[i].y = 100;
			position[i].x = rand() % 200 - 100;
			position[i].z = rand() % 220 - 110;
			position[i].x += player->GetPosition().x;
			position[i].z += player->GetPosition().z;
			s[i] = (float)rand() / 1000;
		}
	}
	if (positionS.x < 300) {
		positionS.x += 8;
	}
	else if (positionS.x >= 300) {
		positionS.x = -20;
	}
#pragma region チュートリアルUI
	if (stageNum == 2)
	{
		if (player->GetPosition().x > 125 && player->GetPosition().x < 255 &&
			player->GetPosition().z > -206 && player->GetPosition().z < -108)
		{
			tutorialXFlag = true;
			if (tutorialXFlag = true)
			{
				if (XButtonTimer > 0)
				{
					XButtonTimer--;
				}
				else if (XButtonTimer <= 0)
				{
					XButtonTimer = 30;
					if (XButtonCount < 1)
					{
						XButtonCount++;
					}
					else if (XButtonCount == 1)
					{
						XButtonCount = 0;
					}
				}
			}
		}
		else {
			tutorialXFlag = false;
		}
		if (player->GetPosition().x > 548 && player->GetPosition().x < 662 &&
			player->GetPosition().z > -212 && player->GetPosition().z < -58) {
			tutorialAFlag = true;
			if (tutorialAFlag = true) {
				if (AButtonTimer > 0) {
					AButtonTimer--;
				}
				else if (AButtonTimer <= 0) {
					AButtonTimer = 30;
					if (AButtonCount < 1) {
						AButtonCount++;
					}
					else if (AButtonCount == 1) {
						AButtonCount = 0;
					}
				}
			}
		}
		else {
			tutorialAFlag = false;
		}

	}
	else if (stageNum == 3) {
		if (player->GetPosition().x > 187 && player->GetPosition().x < 285 &&
			player->GetPosition().z > -180 && player->GetPosition().z < -91)
		{
			tutorialRBFlag = true;
			if (tutorialRBFlag = true)
			{
				if (RBButtonTimer > 0)
				{
					RBButtonTimer--;
				}
				else if (RBButtonTimer <= 0)
				{
					RBButtonTimer = 30;
					if (RBButtonCount < 1)
					{
						RBButtonCount++;
					}
					else if (RBButtonCount == 1)
					{
						RBButtonCount = 0;
					}
				}
			}
		}
		else
		{
			tutorialRBFlag = false;
		}
		if (player->GetPosition().x > 732 && player->GetPosition().x < 840 &&
			player->GetPosition().z > -289 && player->GetPosition().z < -188)
		{
			tutorialLBFlag = true;
			if (tutorialLBFlag = true)
			{
				if (LBButtonTimer > 0)
				{
					LBButtonTimer--;
				}
				else if (LBButtonTimer <= 0)
				{
					LBButtonTimer = 30;
					if (LBButtonCount < 1)
					{
						LBButtonCount++;
					}
					else if (LBButtonCount == 1)
					{
						LBButtonCount = 0;
					}
				}
			}
		}
		else
		{
			tutorialLBFlag = false;
		}

	}

#pragma endregion

	//ライト更新
	lightGroup->Update();
}
void PlayScene::Draw()
{
	if (fade <= 1.0f) {

		//背景描画
		//Drawにカーソル合わせればコメントアウトしてあるからなにがどの変数かわかるよ
		Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);

		//オブジェクト

		//マップチップの描画
		mapStage->Draw(camera->GetEye());

		//敵の血痕
		enemy->FirstDraw();

		//パーティクル描画
		mapStage->DrawParticle(camera->GetEye());

		//プレイヤーの描画
		player->Draw(enemy);

		enemy->Draw();

		player->PreDraw();

		particleAdd->Draw();

		enemy->DrawBlood();

		for (int i = 0; i < rainMax; i++) {
			Object::Instance()->Draw(RainOBJ[i], position[i], { 0.1,s[i] ,0.1 }, Vec3{ 1,1,1 }, Vec4{ 1,1,1,1 }, rainGraph);
		}

		//前景描画
		player->UIDraw();
		enemy->DrawUI();

		//回復のチュートリアル
		player->TutorialDraw(tutorialAFlag, tutorialXFlag, tutorialRBFlag, tutorialLBFlag);

		Sprite::Instance()->Draw(SChangeGraph, ChangeGraphPosition, 1980, window_height);
		//}
		if (tutorialAFlag == true) {
			Object::Instance()->Draw(tutorial3, { player->GetPosition().x,player->GetPosition().y + 20, player->GetPosition().z }, Vec3{ 1,1,1 }, Vec3{ 30 ,0, 0 }, Vec4{ 1,1,1,1 }, AButton[AButtonCount]);

		}
		if (tutorialXFlag == true) {
			Object::Instance()->Draw(tutorial, { player->GetPosition().x,player->GetPosition().y + 20, player->GetPosition().z }, Vec3{ 1,1,1 }, Vec3{ 30,0,0 }, Vec4{ 1,1,1,1 }, XButton[XButtonCount]);
		}
		if (tutorialRBFlag == true) {
			Object::Instance()->Draw(tutorial2, { player->GetPosition().x,player->GetPosition().y + 20, player->GetPosition().z }, Vec3{ 1,1,1 }, Vec3{ 30,0,0 }, Vec4{ 1,1,1,1 }, RBButton[RBButtonCount]);
		}
		if (tutorialLBFlag == true) {
			Object::Instance()->Draw(tutorial2, { player->GetPosition().x,player->GetPosition().y + 20, player->GetPosition().z }, Vec3{ 1,1,1 }, Vec3{ 30,0,0 }, Vec4{ 1,1,1,1 }, LBButton[LBButtonCount]);
		}
	}
#if _DEBUG
	debugText.Print(10, 180, 2, "%d", stageNum);
	debugText.Print(10, 500, 2, "ss%d", player->GetSS());
#endif
	//Sprite::Instance()->Draw(GameOverGraph, deadGraphPos, window_width, window_height);
	Sprite::Instance()->Draw(GameOverGraph, Vec2(0, 0), window_width, window_height, Vec2(0, 0), Vec4(color, color, color, fade));
	if (swordTime <= 0) {
		Object::Instance()->Draw(sword2, Vec3(player->GetPosition().x + 41, 7, player->GetPosition().z - 10), Vec3(10, 10, 10), Vec3(0, 90, -20), Vec4(1, 1, 1, 1));
	}
	Sprite::Instance()->Draw(GameOverTextGraph, Vec2(0, 0), window_width, window_height, Vec2(0, 0), Vec4(1, 1, 1, fade2));

#if _DEBUG
	//デバックテキスト描画ここは変わらない
	debugText.DrawAll();
#endif
}
void PlayScene::LoadDraw()
{
	if (LoadFlag == true) {
		Object::Instance()->Draw(sword, positionS, Vec3{ 2,2,4 }, { 0,90,0 }, Vec4{ 1,1,1,1 });
	}
}
bool PlayScene::GetSceneFlag()
{
	return sceneFlag;
}