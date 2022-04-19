#include "PlayScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

PlayScene::PlayScene()
{}
PlayScene::~PlayScene()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(particleMan3);
	safe_delete(particleMan4);
	safe_delete(lightGroup);
	safe_delete(fbxObject1);
	safe_delete(model1);
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
	//FBX������
	FBXObject3d::SetCamera(camera);
	//�p�[�e�B�N��������
	ParticleManager::SetCamera(camera);
	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();
	//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);
}

void PlayScene::Init()
{
	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/i.wav");

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

	//�X�v���C�g�摜�ǂݍ���
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/text2.jpg");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/back.png");
	controlGraph = Sprite::Instance()->SpriteCreate(L"Resources/ControlUI/ControlUI.png");
	GameOverGraph = Sprite::Instance()->SpriteCreate(L"Resources/GameOver.png");
	//3D�I�u�W�F�N�g�摜�ǂݍ���
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	graph1 = Object::Instance()->LoadTexture(L"Resources/texture2.jpg");

	//3Dobj�t�@�C���ǂݍ��݁B

	//Shape�N���X�Ɍ��܂����`�̃I�u�W�F�N�g���쐬	

	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3D�I�u�W�F�N�g�̐����ƃ��f���̃Z�b�g
	fbxObject1 = new FBXObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(model1);

	//�p�[�e�B�N���N���X�쐬

	particleMan = ParticleManager::Create(L"Resources/Eblood/BloodCircle.png", 0);
	particleMan2 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile1.png",0);
	particleMan3 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile3.png",0);
	particleMan4 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile7.png",0);
	particleMan5 = ParticleManager::Create(L"Resources/map/MapGraph/Floor_Tile9.png",0);
	//�}�b�v�`�b�v�̏�����
	mapStage = new MapStage;
	mapStage->Init();
	//�v���C���[�̏�����
	player = new Player;
	player->Init();
	//�G
	enemy = new Enemy;
	enemy->Init();

	//�X�e�[�W������
	stageFlag = true;
	stageNum = 1;
	StageInit();


}

void PlayScene::StageInit()
{
	if (stageFlag == true)
	{
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
		default:
			break;
		}
		stageNum++;
		stageFlag = false;
	}
}

void PlayScene::Update()
{
	//next�X�e�[�W

	StageInit();



	//�v���C���[�̍X�V
	mapStage->Update(enemy);

	player->Update(enemy);

	enemy->Update(player);


	//�}�b�v�`�b�v�ƃv���C���[�̉����߂�����
	PushCollision::Player2Mapchip(player, enemy, mapStage, stageFlag);


	fbxObject1->Update();
	//
	camera->FollowCamera(player->GetPosition(), Vec3{ 0,80,-10 }, 0.0f, -15.0f);

	//�p�[�e�B�N���ǉ�
	if (player->GetMoveFlag() == true)
	{

	
				if (mapStage->GetfloorNum()==0) {
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
			/*	else if (mapStage->GetMap(player->GetPosition().z, player->GetPosition().x) == 2 || mapStage->GetMap(player->GetPosition().z, player->GetPosition().x) == 3)
				{
					particleMan3->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
				}
				else if (mapStage->GetMap(player->GetPosition().z, player->GetPosition().x) == 8 || mapStage->GetMap(player->GetPosition().z, player->GetPosition().x) == 9)
				{
					particleMan5->ParticleAdd3(player->GetPosition(), 0.1f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));

				}*/
	


	//�p�[�e�B�N���ǉ�
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{

		//�_���[�W���󂯂���`��
		if (enemy->GetDamegeFlag(i) == true) {
			if (player->GetDirection() == Right) {
				particleMan->SetParticleDirection(0);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == Left) {
				particleMan->SetParticleDirection(1);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == Up) {
				particleMan->SetParticleDirection(2);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == Down) {
				particleMan->SetParticleDirection(3);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == UpRight) {
				particleMan->SetParticleDirection(4);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == UpLeft) {
				particleMan->SetParticleDirection(5);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, 1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == DownRight) {
				particleMan->SetParticleDirection(6);
				particleMan->ParticleAdd(enemy->GetPosition(i), 1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			if (player->GetDirection() == DownLeft) {
				particleMan->SetParticleDirection(7);
				particleMan->ParticleAdd(enemy->GetPosition(i), -1.0f, -1.0f, Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}

		}


	}

	if (player->GetHP() <= 0 && deadGraphPos.y < 0)
	{
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
	//�I�u�W�F�N�g�`��O����
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);

	//�I�u�W�F�N�g
	//Object::Instance()->Draw(BossPolygon, pPos1, Vec3{ 1.0f,1.0f,1.0f }, angle, Vec4{ 1.0f,1.0f,1.0f ,1.0f });

	//�}�b�v�`�b�v�̕`��
	mapStage->Draw(player->GetPosition());

	//�G�̌���
	enemy->BloodDraw();

	//�v���C���[�̕`��
	player->Draw();

	enemy->Draw();



	particleMan->Draw();
	particleMan2->Draw();
	particleMan3->Draw();
	particleMan4->Draw();
	particleMan5->Draw();

	//�O�i�`��
	player->UIDraw();
	Sprite::Instance()->Draw(controlGraph, Vec2(0, 0), window_width, window_height);

#if _DEBUG
	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "E:button");

	debugText.Print(10, 80, 2, "D:Attack");

	debugText.Print(10, 120, 2, "F:kamikaihi");

	debugText.Print(10, 180, 2, "%d,%d", (int)player->GetPosition().x, (int)player->GetPosition().x);

	Sprite::Instance()->Draw(GameOverGraph, deadGraphPos, window_width, window_height);

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
#endif
}

bool PlayScene::GetSceneFlag()
{
	return sceneFlag;
}
