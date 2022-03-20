#include "GameSceneManager.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

GameSceneManager::GameSceneManager()
{}
GameSceneManager::~GameSceneManager()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
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
void GameSceneManager::Initialize(_DirectX *directX)
{
	assert(directX);
	this->directX = directX;
	//Audio�N���X�쐬
	audio = Audio::Create();
	//�J�����N���X�쐬
	camera = Camera::Create();
	//�X�v���C�g�N���X�쐬
	Sprite::Instance()->Init();
	//FBX������
	FBXObject3d::SetDevice(directX->GetDevice());
	FBXObject3d::SetCmdList(directX->GetCmandList());
	FBXObject3d::SetCamera(camera);
	FBXObject3d::CreateGraphicsPipeline();
	//�}�`���f��������
	Shape::Init(directX->GetDevice());
	//�p�[�e�B�N��������
	ParticleManager::StaticInitialize(directX->GetDevice(), directX->GetCmandList(), this->camera, window_width, window_height);
	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();
	//3D�I�u�W�F�N�g������
	Object::Instance()->Init(directX->GetDevice(), directX->GetCmandList(), camera, lightGroup);
}

void GameSceneManager::Init()
{
	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/i.wav");
	//sound2 = Audio::SoundLoadWave("Resources/BGM.wav");

	//�ǂݍ��񂾉��f�[�^��1�񂾂�����
	//sound->SoundSEPlayWave(sound1);

	//�ǂݍ��񂾉��f�[�^�����[�v�ŗ���
	//sound->SoundBGMPlayLoopWave(sound2, sound->BGM);

	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	//Object3d::SetLightGroup(lightGroup);
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
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/backgroundA.png");
	Parent = Sprite::Instance()->SpriteCreate(L"Resources/text2.jpg");

	//3D�I�u�W�F�N�g�摜�ǂݍ���
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	graph1 = Object::Instance()->LoadTexture(L"Resources/texture2.jpg");

	//3Dobj�t�@�C���ǂݍ��݁B
	//Polygon = Object::Instance()->CreateOBJ("Boss");
	BossPolygon = Object::Instance()->CreateOBJ("sphere", true);

	//Shape�N���X�Ɍ��܂����`�̃I�u�W�F�N�g���쐬	
	Polygon = Shape::CreateSquare(20.0f, 20.0f, 20.0f);

	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	//3D�I�u�W�F�N�g�̐����ƃ��f���̃Z�b�g
	fbxObject1 = new FBXObject3d;
	fbxObject1->Initialize();
	fbxObject1->SetModel(model1);

	//�p�[�e�B�N���N���X�쐬
	particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	particleMan2 = ParticleManager::Create(L"Resources/text2.jpg", 1);

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

void GameSceneManager::Update()
{
	//�L�[����
	//Input�N���X�Ƀ}�E�X�ƃR���g���[���������
	//�C���X�^���X�����Ă�̂�Input/Input.h"�������Ă���΂ǂ̃N���X�ł��g�����
	if (Input::Instance()->KeybordPush(DIK_UP))
	{
		//	fbxObject1->PlayAnimation();
	}
	if (Input::Instance()->KeybordPush(DIK_DOWN))
	{
	}
	if (Input::Instance()->KeybordPush(DIK_LEFT))
	{
	}
	if (Input::Instance()->KeybordPush(DIK_RIGHT))
	{
	}
	//�v���C���[�̍X�V
	player->Update(enemy);

	enemy->Update(player);

	//�}�b�v�`�b�v�ƃv���C���[�̉����߂�����
	PushCollision::Player2Mapchip(player, enemy, mapStage);


	fbxObject1->Update();
	//
	camera->FollowCamera(player->GetPosition(), Vec3{ 0,100,-10 });

	//�p�[�e�B�N���ǉ�
	if (player->GetMoveFlag() == true)
	{
		particleMan->ParticleAdd(player->GetPosition(), Vec4(1.0f, 1.0f, 1.0f, 1.0f), Vec4(1.0f, 0.8f, 1.0f, 1.0f));
	}

	//�p�[�e�B�N���ǉ�
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		if (enemy->GetWasAttackFlag(i) == true)
		{
			particleMan->ParticleAdd(enemy->GetPosition(i), Vec4(1.0f, 0.0f, 0.0f, 1.0f), Vec4(1.0f, 0.4f, 0.0f, 1.0f));
		}
	}
	//�p�[�e�B�N���X�V
	particleMan->Update();

	particleMan2->Update();
	//���C�g�X�V
	lightGroup->Update();
}

void GameSceneManager::Draw()
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
	mapStage->Draw();
	//�v���C���[�̕`��
	player->Draw();

	enemy->Draw();


	//fbxObject1->Draw();


	//�p�[�e�B�N���`��
	particleMan->Draw();

	particleMan2->Draw();

	//�O�i�`��
	//Sprite::Instance()->Draw(spriteGraph, Vec2(400, 400), 100, 100, Vec2(0.5f, 0.5f));
#if _DEBUG
	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "E:button");

	debugText.Print(10, 80, 2, "WASD+C:Attack");

	debugText.Print(10, 120, 2, "SPACE:noutou");

	debugText.Print(10, 160, 2, "comboNum:%d", player->GetComboNum());

	debugText.Print(10, 200, 2, "comboTime:%d", player->GetComboTime());

	debugText.Print(10, 240, 2, "coolTime:%d", player->GetCoolTime());


	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
#endif
}
