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
	Audio::SoundUnload(&sound2);
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
	sound1 = Audio::SoundLoadWave("Resources/Music/BGM/heiannoyoi.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/SE/button.wav");

	//�X�v���C�g�摜�ǂݍ���
	spriteGraph = Sprite::Instance()->SpriteCreate(L"Resources/title.png");
	BGGraph = Sprite::Instance()->SpriteCreate(L"Resources/select.png");
	for (int i = 0; i < rainMax; i++) {
		rain[i] = Sprite::Instance()->SpriteCreate(L"Resources/white1x1.png");
	}
	titleGraph = Sprite::Instance()->SpriteCreate(L"Resources/TitleText.png");
	startGraph = Sprite::Instance()->SpriteCreate(L"Resources/Start.png");

	Button[0] = Sprite::Instance()->SpriteCreate(L"Resources/SelectB.png");
	Button[1] = Sprite::Instance()->SpriteCreate(L"Resources/SelectB2.png");

	VolumeUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/VolumeUI.png");
	VolumeUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/Volume.png");


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
	ButtonFlag = false;
	sceneChangeFlag = false;
	//�t�F�[�h
	//fade = 1;
	volumeFlag = 0;
	volumeArrowFlag = false;
	volume = 1;
	volume2 = 1;
	volumeB = 500;

	volumeFadeFlag = false;
	volumeFade = 0;
	scene = 0;

	position = { 5.0f,0.0f,0.0f };
	rotation = { 0,90,-90 };
	size = { 1,1,1 };
	for (int i = 0; i < rainMax; i++) {
		//x�̓����_���Ȉʒu�ɂ��Ă���
		//�T�C�Y�������_���ɂ��Ă���
		pos[i].y = rand() % -100;
		pos[i].x = rand() % 1300;
		s[i] = rand() % 500 - 400;
	}
	ControllerLFlag = false;
	ControllerRFlag = false;
	audio->SoundBGMPlayLoopWave(sound1,0);
}

void Title::Update()
{
	//�p�[�e�B�N��������
	ParticleManager::SetCamera(camera);
	//3D�I�u�W�F�N�g������
	Object::Instance()->SetCamera(camera);
	Object::Instance()->SetLight(lightGroup);
	Direction();

	//�J�̏���
	if (rainFlag == false)
	{
		if (rainTime < 30)
		{
			rainTime++;
		}
		if (rainTime >= 30)
		{
			rainFlag = true;
			rainTime = 0;
		}
	}
	if (rainFlag == true)
	{
		for (int i = 0; i < rainMax; i++)
		{
			if (pos[i].y < 720)
			{
				//�J���n�ʂɂ��܂ō~��
				pos[i].y = pos[i].y + v;
				v = g + v;
			}
			if (pos[i].y >= 720)
			{
				//�n�ʂɂ�����܂���ɍs��
				pos[i].y = rand() % -100;
				pos[i].x = rand() % 1300;
				v = 0;
				g = 9.8f;
				s[i] = rand() % 500 - 400;
			}
		}
		if (scene == 0)
		{
			if (titleTextFlag == false)
			{
				if (titleTime < 60)
				{
					titleTime++;
				}
				if (titleTime >= 60)
				{
					titleTextFlag = true;
					titleTime = 0;
				}
			}
		}
	}
	if (scene == 0)
	{
		if (titleTextFlag == true)
		{
			if (fade < 1)
			{
				fade += 0.01f;
			}
			if (fade >= 1)
			{
				fade2 += 0.01f;
			}
		}
		if (fade2 >= 1)
		{
			ButtonFlag = true;
		}
		if (ButtonFlag == true)
		{
			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				audio->SoundSEPlayWave(sound2);
				volume2 = 1;
				scene = 1;
				sceneChangeFlag = true;
			}
		}
	}
	else if (scene == 1)
	{
		//���ʒ���
		if (Input::Instance()->ControllerDown(ButtonB))
		{
			audio->SoundSEPlayWave(sound2);
			volumeFlag++;
			position = { -50.0f,0.0f,0.0f };
			rotation = { 90,0,0 };
			size = { 6,4,4 };
		}
		if (volumeFlag == 2)
		{
			volumeFlag = 0;
		}

		if (volumeFlag == 0)
		{
			position = { 5.0f,0.0f,0.0f };
			rotation = { 0,90,-90 };
			size = { 1,1,1 };
			volumeFade = 0;
			volumeFadeFlag = false;
			rainFlag = true;
			if (Input::Instance()->KeybordTrigger(DIK_SPACE) || Input::Instance()->ControllerDown(ButtonA))
			{
				audio->SoundSEPlayWave(sound2);
			}
		}
		else if (volumeFlag == 1)
		{
			rainFlag = false;

			if (position.x < 4)
			{
				position.x += 2;
			}
			else if (position.x >= 4)
			{
				volumeFadeFlag = true;
			}
			if (volumeFadeFlag == true)
			{
				volumeFade += 0.1;
			}
		
			if (Input::Instance()->ControllerDown(LButtonRight))
			{
				if (volume < 1)
				{
					volume += 0.1;
					volume2 += 0.1;
					volumeB += 50;
				}
			}
			else if (Input::Instance()->ControllerDown(LButtonLeft))
			{
				if (volume > 0)
				{
					volume -= 0.1;
					volume2 -= 0.1;
					volumeB -= 50;
				}
			}

		}
	}

	//���C�g�X�V
	lightGroup->Update();
}

void Title::AudioUpdate()
{

	if (audioFlag == false) {
		audio->SoundStop(0);
	}
	audio->SetVolume(volume);

}

void Title::Direction()
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

void Title::Draw()
{
	//�w�i�`��
	//Draw�ɃJ�[�\�����킹��΃R�����g�A�E�g���Ă��邩��Ȃɂ��ǂ̕ϐ����킩���
	Sprite::Instance()->Draw(BGGraph, { 0,0 }, window_width, window_height);
	Sprite::Instance()->Draw(spriteGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
	for (int i = 0; i < rainMax; i++) {
		if (rainFlag == true) {
			Sprite::Instance()->Draw(rain[i], pos[i], 1, s[i], { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
		}
	}
	Object::Instance()->Draw(swordObject, position, size, rotation, Vec4{ 1,1,1,1 });

	if (scene == 0) {
		if (titleTextFlag == true) {
			Sprite::Instance()->Draw(titleGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade });
		}

		Sprite::Instance()->Draw(startGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,fade2 });
	}

	else if (scene == 1) {
		if (volumeFlag == 0) {
			Sprite::Instance()->Draw(Button[0], Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));
		}
		//���ʒ��߃o�[�̕`��
		else if (volumeFlag == 1) {
			Sprite::Instance()->Draw(VolumeUI[1], Vec2(420, 350), volumeB, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, volumeFade));
			Sprite::Instance()->Draw(VolumeUI[0], Vec2(420, 350), 500, 40, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, volumeFade));
		
			Sprite::Instance()->Draw(Button[1], Vec2(0, 0), window_width, window_height, Vec2(0.0f, 0.0f), Vec4(1, 1, 1, 1));

		}
	}
#if _DEBUG
	//�f�o�b�N�e�L�X�g%d��%f�Ή�
	debugText.Print(10, 40, 2, "%d", volumeArrowFlag);
	debugText.Print(10, 80, 2, "%d", volumeFlag);

	//�f�o�b�N�e�L�X�g�`�悱���͕ς��Ȃ�
	debugText.DrawAll();
#endif
}
