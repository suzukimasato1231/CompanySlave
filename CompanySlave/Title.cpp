#include "Title.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

Title::Title()
{}
Title::~Title()
{
	safe_delete(lightGroup);
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	Audio::SoundUnload(&sound1);
	safe_delete(audio);
}
void Title::Initialize()
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

	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/Music/heiannoyoi.wav");
	//�X�v���C�g�摜�ǂݍ���
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	for (int i = 0; i < rainMax; i++) {
		rain[i] = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	}
	titleGraph = Sprite::Instance()->SpriteCreate(L"Resources/TitleText.png");
	startGraph = Sprite::Instance()->SpriteCreate(L"Resources/Start.png");
	swordObject = Object::Instance()->CreateOBJ("sword");

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
	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	srand(time(NULL));
	fade = 0;
	fade2 = 0;
	titleTextFlag = false;
	rainFlag = false;
	bottanFlag = false;
	for (int i = 0; i < rainMax; i++) {
		//x�̓����_���Ȉʒu�ɂ��Ă���
		//�T�C�Y�������_���ɂ��Ă���
		pos[i].y = rand()%-100;
		pos[i].x = rand()%1300;
		s[i] = rand() % 500-400;
	}
	audio->SoundBGMPlayLoopWave(sound1);
}

void Title::Update()
{
	//
	// 	//�p�[�e�B�N��������
	ParticleManager::SetCamera(camera);
		//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);

	if (rainFlag == false) {
		if (rainTime < 30) {
			rainTime++;
		}
		if (rainTime >= 30)
		{
			rainFlag = true;
			rainTime = 0;
		}
	}

	if (rainFlag == true) {
		for (int i = 0; i < rainMax; i++) {

			if (pos[i].y < 720) {
				//�J���n�ʂɂ��܂ō~��
				pos[i].y = pos[i].y + v;
				v = g + v;
			}
			if (pos[i].y >= 720) {
				//�n�ʂɂ�����܂���ɍs��
				pos[i].y = rand() % -100;
				pos[i].x = rand() % 1300;
				v = 0;
				g = 9.8f;
				s[i] = rand() % 500 - 400;
			}
		}
		if (titleTextFlag == false) {
			if (titleTime < 60) {
				titleTime++;
			}
			if (titleTime >= 60)
			{
				titleTextFlag = true;
				titleTime = 0;
			}
		}
	}
		if (titleTextFlag == true) {
			if (fade < 1) {
				fade+=0.01f;
			}
			if (fade >= 1) {
				fade2 += 0.01f;
			}
		}
		if (fade2 >= 1) {
			bottanFlag = true;
		}
	//�p�[�e�B�N���X�V
	//particleMan->Update();
	//particleMan2->Update();
	
	//���C�g�X�V
	lightGroup->Update();
}

void Title::AudioUpdate()
{

	if (audioFlag == false) {
		audio->SoundStop();
	}
	audio->SetVolume(volume);

}

void Title::Draw()
{
	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, {0,0}, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1});
	for (int i = 0; i < rainMax; i++) {
		if (rainFlag == true) {
			Sprite::Instance()->Draw(rain[i], pos[i], 1, s[i], { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
		}
	}
	if (titleTextFlag == true) {
		Sprite::Instance()->Draw(titleGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade });
	}

		Sprite::Instance()->Draw(startGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade2 });
	
	Object::Instance()->Draw(swordObject, position, Vec3{1,1,1 }, Vec3{ 0,-90,-90 }, Vec4{1,1,1,1});

	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "%f",fade);

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}
