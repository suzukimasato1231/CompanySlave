#include "SelectScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

SelectScene::SelectScene()
{}
SelectScene::~SelectScene()
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
void SelectScene::Initialize(_DirectX* directX)
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

void SelectScene::Init()
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
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/backgroundA.png");
	Bottan = Sprite::Instance()->SpriteCreate(L"Resources/bottan.png");

	Number[0] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/0.png");
	Number[1] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/1.png");
	Number[2] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/2.png");
	Number[3] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/3.png");
	Number[4] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/4.png");
	Number[5] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/5.png");
	Number[6] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/6.png");
	Number[7] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/7.png");
	Number[8] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/8.png");
	Number[9] = Sprite::Instance()->SpriteCreate(L"Resources/ComboUI/9.png");

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

void SelectScene::Update()
{
	//�L�[����
	//Input�N���X�Ƀ}�E�X�ƃR���g���[���������
	//�C���X�^���X�����Ă�̂�Input/Input.h"�������Ă���΂ǂ̃N���X�ł��g�����
	if (Input::Instance()->KeybordTrigger(DIK_UP))
	{
	
	}
	if (Input::Instance()->KeybordTrigger(DIK_DOWN))
	{
		
	}
	//�X�e�[�W�I���{�^��
	if (Input::Instance()->KeybordTrigger(DIK_LEFT))
	{
		if (stage > 1) {
			stage--;
			if (nCount == 0) {
				nCount = 10;
				if (nCount2 > 1) {
					nCount2--;
				}
			}
			if (nCount > 0&&nCount2!=0) {
				nCount--;
			}
			if (nCount > 1&&nCount2 == 0) {
				nCount--;
			}
		}
		if (stage < 10) {
			nCount2 = 0;
		}
	}
	if (Input::Instance()->KeybordTrigger(DIK_RIGHT))
	{
		if (stage < 30) {
			stage++;

			if (nCount < 9) {
				nCount++;

			}
			if (stage % 10 == 0) {
				nCount2++;
				nCount = 0;
			}
		}
	}


	fbxObject1->Update();

	//�p�[�e�B�N���X�V
	particleMan->Update();
	particleMan2->Update();

	//���C�g�X�V
	lightGroup->Update();
}

void SelectScene::Draw()
{
	//�I�u�W�F�N�g�`��O����
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	
	Sprite::Instance()->Draw(Bottan, Vec2(580, 280), 200, 200, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	
	if (stage < 10) {
		Sprite::Instance()->Draw(Number[nCount], Vec2(630, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	}
	if (stage >= 10) {

		Sprite::Instance()->Draw(Number[nCount2], Vec2(590, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
		Sprite::Instance()->Draw(Number[nCount], Vec2(670, 320), 100, 100, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	}

	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "LeftArrow");
	debugText.Print(10, 80, 2, "RightArrow");


	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
