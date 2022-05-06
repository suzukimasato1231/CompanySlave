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
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(particleMan3);
	safe_delete(particleMan4);
	safe_delete(particleMan5);
	safe_delete(lightGroup);
	safe_delete(mapStage);
	safe_delete(player);
	safe_delete(enemy);
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	safe_delete(audio);
}
void PlayScene::Initialize()
{
	//Audio�N���X�쐬
	audio = Audio::Create();
	//�J�����N���X�쐬
	camera = Camera::Create();
	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();

	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/Music/BGM/rain.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/BGM/battle.wav");


	//�X�v���C�g�摜�ǂݍ���
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/back.png");
	controlGraph = Sprite::Instance()->SpriteCreate(L"Resources/ControlUI/ControlUI.png");
	GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver.png");
	SChangeGraph = Sprite::Instance()->SpriteCreate(L"Resources/SceneChange.png");

	for (int i = 0; i < rainMax; i++) {
		RainOBJ[i] = Shape::CreateRect(1, 1);
	}
	rainGraph = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	//3D�I�u�W�F�N�g�摜�ǂݍ���
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");

	//�p�[�e�B�N���N���X�쐬
	particleMan = ParticleManager::Create(L"Resources/Eblood/BloodCircle.png", 0);
	particleMan2 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile1.png", 0);
	particleMan3 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile3.png", 0);
	particleMan4 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile7.png", 0);
	particleMan5 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile9.png", 0);



	//�}�b�v�`�b�v�̏�����
	mapStage = new MapStage;
	mapStage->Init();
	//�v���C���[�̏�����
	player = new Player;
	player->Init();
	//�G
	enemy = new Enemy;
	enemy->Init();
}

void PlayScene::Init()
{
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//�J�����ʒu���Z�b�g
	camera->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//�X�e�[�W������
	stageFlag = true;
	stageNum = 1;
	//���[�v�����ł̏�����
	player->LoopInit();

	deadGraphPos = { 0.0f,-800.0f };
	nowTime = 0;//���[�v
	endTime = 5;//���[�v
	timeRate = 0;//���[�v

	fade = 1.0f;
	stageFlag = true;

	sceneFlag = false;
	sceneChangeFlag = false;
	ChangeGraphPosition = { -1600.0f, 0.0f };
	srand(time(NULL));

	for (int i = 0; i < rainMax; i++) {
		//x��z�̓����_���Ȉʒu�ɂ��Ă���
		//�T�C�Y�������_���ɂ��Ă���
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
		//audio->SoundBGMPlayLoopWave(sound1);
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
	//�p�[�e�B�N��������
	ParticleManager::SetCamera(camera);
	//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	//next�X�e�[�W

	StageInit();



	//�v���C���[�̍X�V
	mapStage->Update(player->GetPosition(), enemy);

	if (sceneChangeFlag == false) {
		player->Update(enemy);

		enemy->Update(player);
	}

	//if (Input::Instance()->ControllerDown(ButtonX))
	//{
	//	audio->SoundSEPlayWave(sound2);
	//}

	//0������
	enemy->SetVolume(volume);
	player->SetVolume(volume);
	audio->SetVolume(volume);


	//�}�b�v�`�b�v�ƃv���C���[�̉����߂�����
	PushCollision::Player2Mapchip(player, enemy, mapStage, sceneChangeFlag);


	//
	//�J��������
	camera->FollowCamera(player->GetCameraPos(), Vec3{0,80,-10}, 0.0f, -15.0f);



	//�p�[�e�B�N���ǉ�
	if (player->GetMoveFlag() == true)
	{
		if (mapStage->GetfloorNum() == 0) {
			particleMan2->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 1) {
			particleMan3->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 2) {
			particleMan4->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}
		if (mapStage->GetfloorNum() == 3) {
			particleMan5->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

	}

	//�p�[�e�B�N���ǉ�
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{

		//�_���[�W���󂯂���`��
		if (enemy->GetDamegeFlag(i) == true) {
			if (player->GetDirection() == Right) {
				particleMan->SetParticleDirection(0);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Left) {
				particleMan->SetParticleDirection(1);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Up) {
				particleMan->SetParticleDirection(2);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == Down) {
				particleMan->SetParticleDirection(3);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == UpRight) {
				particleMan->SetParticleDirection(4);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == UpLeft) {
				particleMan->SetParticleDirection(5);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == DownRight) {
				particleMan->SetParticleDirection(6);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else if (player->GetDirection() == DownLeft) {
				particleMan->SetParticleDirection(7);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	if (player->GetHP() <= 0 && deadGraphPos.y < 0)
	{
		//audio->SoundStop();

		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		deadGraphPos.y = -800 * (1.0f - (timeRate * timeRate) * (3 - (2 * timeRate))) + 0 * (timeRate * timeRate) * (3 - (2 * timeRate));
	}

	//�v���C�V�[���𔲂���
	if (Input::Instance()->ControllerDown(ButtonA) && nowTime >= 2.0f)
	{

		deadGraphPos.y = -800;
		nowTime = 0;
		endTime = 5;
		timeRate = 0;
		sceneFlag = true;
	}

	if (sceneChangeFlag == true) {
		//audio->SoundStop();
		if (ChangeGraphPosition.x < 0) {
			ChangeGraphPosition.x += 22;
		}
		if (ChangeGraphPosition.x >= 0) {

			stageFlag = true;
		}
	}
	else if (sceneChangeFlag == false) {
		ChangeGraphPosition = { -1600.0f, 0.0f };
	}




	rainSlow = player->GetSlow();
	for (int i = 0; i < rainMax; i++) {

		if (position[i].y > 0) {
			//�J���n�ʂɂ��܂ō~��
			position[i].y = position[i].y - g * rainSlow;
		}
		if (position[i].y <= 0) {
			//�n�ʂɂ�����܂���ɍs��
			//x��z�̓����_���Ȉʒu�ɂ��Ă���
			//�T�C�Y�������_���ɂ��Ă���
			position[i].y = 100;
			position[i].x = rand() % 200 - 100;
			position[i].z = rand() % 220 - 110;
			position[i].x += player->GetPosition().x;
			position[i].z += player->GetPosition().z;
			s[i] = (float)rand() / 1000;
		}
	}
	particleMan->Update();
	particleMan2->Update();
	particleMan3->Update();
	particleMan4->Update();
	particleMan5->Update();

	//���C�g�X�V
	lightGroup->Update();
}
void PlayScene::Draw()
{
	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);

	//�I�u�W�F�N�g

	//�}�b�v�`�b�v�̕`��
	mapStage->Draw(camera->GetEye());

	//�G�̌���
	enemy->BloodDraw();

	//�v���C���[�̕`��
	player->Draw();

	enemy->Draw();

	//�p�[�e�B�N���`��
	mapStage->DrawParticle(camera->GetEye());

	particleMan->Draw();
	particleMan2->Draw();
	particleMan3->Draw();
	particleMan4->Draw();
	particleMan5->Draw();

	enemy->DrawBlood();


	for (int i = 0; i < rainMax; i++) {
		Object::Instance()->Draw(RainOBJ[i], position[i], { 0.1,s[i] ,0.1 }, Vec3{ 1,1,1 }, Vec4{ 1,1,1,1 }, rainGraph);
	}
	//�O�i�`��
	player->UIDraw();
	enemy->DrawUI();
	Sprite::Instance()->Draw(controlGraph, Vec2(0, 0), window_width, window_height);

	if (sceneChangeFlag == true) {
		Sprite::Instance()->Draw(SChangeGraph, ChangeGraphPosition, 1980, window_height);
	}

#if _DEBUG
	debugText.Print(10, 180, 2, "%d", stageNum);

	Sprite::Instance()->Draw(GameOverGraph, deadGraphPos, window_width, window_height);

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
#endif
}

bool PlayScene::GetSceneFlag()
{
	return sceneFlag;
}
