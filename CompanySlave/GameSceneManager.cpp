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
	//�X�v���C�g�N���X�쐬
	Sprite::Instance()->Init();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();
	//�}�`���f��������
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
	
	//�^�C�g��
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
	//�X�e�[�W�I��
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
				//�v���C�J�n

				title->SetAudioFlag(false);

				play->Init();
				sFadeFlag = true;

			}
		}
		//�t�F�[�h
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

	//�X�e�[�W1
	else if (scene == stage1) {
	

		if (play->GetSceneFlag() == true)
		{
			title->Init();
			title->SetAudioFlag(true);
			scene = titleScene;
			initFlag = true;
		}
					//���̐��l��ς���΃N���A�V�[���ɍs����X�e�[�W��ς����遫
		if (play->GetSceneChangeFlag() == true&&play->GetStageNum()==10) {
			clear->Init();
			clear->SetAudioFlag(true);
			scene = clearScene;
			initFlag = true;
		}
		play->Update();
	}
	//�N���A�V�[��
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
	//���[�h���
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
	


	//�f�o�b�N�p���Z�b�g�{�^��
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
	//�I�u�W�F�N�g�`��O����
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
	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
