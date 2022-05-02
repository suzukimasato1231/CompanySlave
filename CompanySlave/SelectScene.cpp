#include "SelectScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"

SelectScene::SelectScene()
{}
SelectScene::~SelectScene()
{
	safe_delete(lightGroup);
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	safe_delete(audio);
}
void SelectScene::Initialize()
{

	//Audio�N���X�쐬
	audio = Audio::Create();
	//�J�����N���X�쐬
	camera = Camera::Create();
	//�p�[�e�B�N��������
	ParticleManager::SetCamera(camera);
	//���C�g�O���[�v�N���X�쐬
	lightGroup = LightGroup::Create();
	//�f�o�b�N�e�L�X�g������
	debugText.Initialize();
	//���f�[�^�ǂݍ���
	sound1 = Audio::SoundLoadWave("Resources/i.wav");

	//�X�v���C�g�摜�ǂݍ���
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/Loading.png");
	Bottan = Sprite::Instance()->SpriteCreate(L"Resources/bottan.png");

	VolumeUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI.png");
	VolumeUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI2.png");
	VolumeUI[2] = Sprite::Instance()->SpriteCreate(L"Resources/Volume.png");

	LoadUIGraph = Sprite::Instance()->SpriteCreate(L"Resources/LoadUI/Load7.png");
	
	//3D�I�u�W�F�N�g�摜�ǂݍ���
	graph3 = Object::Instance()->LoadTexture(L"Resources/white1x1.png");
	//3Dobj�t�@�C���ǂݍ��݁B
	//�p�[�e�B�N���N���X�쐬
	//particleMan = ParticleManager::Create(L"Resources/particle.jpg", 0);

	//particleMan2 = ParticleManager::Create(L"Resources/text2.jpg", 1);
	
}

void SelectScene::Init()
{
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
	camera->SetCamera(Vec3{ 0,0,-10 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });

	//�t�F�[�h
	fade = 1;
	volumeFlag = false;
	volume = 1;
	volume2 = 1;
	volumeB = 500;
	
	//audio->SoundBGMPlayLoopWave(sound1);
	
}

void SelectScene::Update()
{
	//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);


	if(audioFlag==false) {
		//audio->SoundStop();
	}
	audio->SetVolume(volume);

	Direction();

	//���ʒ���
	//if (Input::Instance()->ConLeftInput())
	//{
	//	if (direction == Down) {
	//		volumeFlag = true;
	//	}
	//	if (direction == Up) {
	//		volumeFlag = false;
	//	}
	//	if (volumeFlag == true) {
	//		if (direction == Right) {
	//			if (volume < 1) {
	//				volume += 0.1;
	//				volume2 += 0.1;
	//				volumeB += 50;
	//			}
	//		}
	//		if (direction == Left) {
	//			if (volume >= 0) {
	//				volume -= 0.1;
	//				volume2 -= 0.1;
	//				volumeB -= 50;
	//			}
	//		}
	//	}
	//}



	//���C�g�X�V
	lightGroup->Update();
}
void SelectScene::Direction()
{
	//�E��
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		direction = UpRight;
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownRight;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		direction = DownLeft;
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		direction = UpLeft;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->ControllerPush(LButtonUp)) {
		direction = Up;
	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->ControllerPush(LButtonRight)) {
		direction = Right;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->ControllerPush(LButtonDown)) {
		direction = Down;
	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->ControllerPush(LButtonLeft)) {
		direction = Left;
	}
}
void SelectScene::Draw()
{
	//�I�u�W�F�N�g�`��O����
	Object::Instance()->PreDraw();
	Sprite::Instance()->PreDraw();

	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, pos, window_width, window_height);
	Sprite::Instance()->Draw(LoadUIGraph, Vec2(420, 280), 500, 150);

	Sprite::Instance()->Draw(spriteGraph, Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	
	Sprite::Instance()->Draw(Bottan, Vec2(580, 180), 200, 200, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//���ʒ��߃o�[�̕`��
	//	Sprite::Instance()->Draw(VolumeUI[2], Vec2(380, 480), volumeB, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));

	//if (volumeFlag == false) {
	//	Sprite::Instance()->Draw(VolumeUI[0], Vec2(380, 480), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//}
	//else if (volumeFlag == true) {
	//	Sprite::Instance()->Draw(VolumeUI[1], Vec2(380, 480), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, fade));
	//}

	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "%f",volume);



	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
}