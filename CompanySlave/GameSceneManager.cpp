#include "GameSceneManager.h"
#include"Input.h"
#include<sstream>
#include<iomanip>
GameSceneManager::GameSceneManager()
{
}

GameSceneManager::~GameSceneManager()
{


}

void GameSceneManager::Initialize(_DirectX* directX)
{
	assert(directX);
	this->directX = directX;
	title = new Title();
	title->Initialize(directX);
	select = new SelectScene();
	select->Initialize(directX);
	play = new PlayScene();
	play->Initialize(directX);

}

void GameSceneManager::Init()
{
	title->Init();
	select->Init();
	play->Init();

	scene = titleScene;
}

void GameSceneManager::Update()
{
	if (scene == titleScene) {
		if (Input::Instance()->KeybordTrigger(DIK_SPACE))
		{
			tFadeFlag = true;
			//scene = selectScene;
		}

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
	else if (scene == selectScene) {
	if (Input::Instance()->KeybordTrigger(DIK_SPACE)&& select->GetStage() == 1)
	{
		sFadeFlag = true;
	}

	if (sFadeFlag == true) {
		sFade -= 0.2f;
	}
	if (sFade < 0) {
		scene = stage1;
		sFadeFlag = false;
	}
	select->SetFade(sFade);

		select->Update();
	}

	else if (scene == stage1) {
		play->Update();
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


}
