#include "Title.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"

Title::Title()
{}
Title::~Title()
{
	safe_delete(particleMan);
	safe_delete(particleMan2);
	safe_delete(lightGroup);
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	Audio::SoundUnload(&sound1);
	safe_delete(audio);
}
void Title::Initialize(_DirectX* directX)
{
	assert(directX);
	this->directX = directX;
	//Audio�N���X�쐬
	audio = Audio::Create();
	//�J�����N���X�쐬
	camera = Camera::Create();
	//FBX�J�����ݒ�
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

void Title::Init()
{
	
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
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");

	//�p�[�e�B�N���N���X�쐬
	particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	particleMan2 = ParticleManager::Create(L"Resources/particle2.png", 1);
}

void Title::Update()
{
	//�p�[�e�B�N���X�V
	particleMan->Update();
	particleMan2->Update();

	//���C�g�X�V
	lightGroup->Update();
}

void Title::Draw()
{
	//�I�u�W�F�N�g�`��O����
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, Vec2(0,0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	
	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "title");

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
