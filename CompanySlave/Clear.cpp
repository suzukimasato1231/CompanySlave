#include "Clear.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

Clear::Clear()
{}
Clear::~Clear()
{
	safe_delete(lightGroup);
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	Audio::SoundUnload(&sound1);
	safe_delete(audio);
}
void Clear::Initialize()
{
	//Audio�N���X�쐬
	audio = Audio::Create();
	//�J�����N���X�쐬
	camera = Camera::Create();

	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();

	////3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);


	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/i.wav");
	//�X�v���C�g�摜�ǂݍ���
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");

	textGraph = Sprite::Instance()->SpriteCreate(L"Resources/Text.png");
	title = Sprite::Instance()->SpriteCreate(L"Resources/clearTitle.png");

	
	PlayerObject = Object::Instance()->CreateOBJ("playerKari1");

}

void Clear::Init()
{

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightDir(1, XMVECTOR{ 0,-1,0,0 });

	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetSpotLightActive(0, false);
	lightGroup->SetCircleShadowActive(0, false);

	//�J�����ʒu���Z�b�g
	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	color = 1;
	colorTimer = 60;
	TextFlag = false;
	bottanFlag = false;

	//audio->SoundBGMPlayLoopWave(sound1);
}

void Clear::Update()
{
	//
	//// 	//�p�[�e�B�N��������
	//ParticleManager::SetCamera(camera);
	//////3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	if (audioFlag == false) {
		//audio->SoundStop();
	}
	audio->SetVolume(volume);

	if (colorTimer > 0) {
		colorTimer--;
	}
	else if (colorTimer <= 0) {
		colorFlag = true;
	}
	if (colorFlag == true) {
		if (color > 0.1)
		{
			color -= 0.01f;
		}
		else if (color <= 0.1) {
			TextFlag = true;
			bottanFlag = true;
		}

	}

	//�p�[�e�B�N���X�V
	//particleMan->Update();
	//particleMan2->Update();

	//���C�g�X�V
	lightGroup->Update();
}

void Clear ::Draw()
{
	//�I�u�W�F�N�g�`��O����

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, { 0,0 }, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { color, color,color ,1 });


	if (colorFlag == true) {
		Object::Instance()->Draw(PlayerObject, position, Vec3{ 1,1,1 }, Vec3{ 0,-90,40 }, Vec4{ 1,1,1,1});
	}
	if (TextFlag == true) {

		Sprite::Instance()->Draw(textGraph, { 440,200 }, 400, 400, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
		Sprite::Instance()->Draw(title, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });

	}
	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "%f", color);

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
