#include "Score.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"


Score::Score()
{}
Score::~Score()
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
void Score::Initialize(_DirectX* directX)
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
	ParticleManager::SetCamera(camera);	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();
	//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);
}

void Score::Init()
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
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/Score.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
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
	//particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	//particleMan2 = ParticleManager::Create(L"Resources/particle2.png", 1);



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

void Score::Update()
{

	fbxObject1->Update();

	//�p�[�e�B�N���X�V
	//particleMan->Update();
	//particleMan2->Update();

	//���C�g�X�V
	lightGroup->Update();
}

void Score::Draw()
{
	//�I�u�W�F�N�g�`��O����
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));


	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "Score");



	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
