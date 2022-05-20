#include "_DirectX.h"
#include "Safe_delete.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include"Collision.h"
#include"Audio.h"
#include"Singleton.h"
#include"Sprite.h"
#include "Object.h"
#include"MapStage.h"
#include"Player.h"
#include"PushCollision.h"
#include"Enemy.h"
#include<array>

extern const int window_width;
extern const int window_height;
class Clear :public Singleton<Clear>
{
private:
	friend Singleton<Clear>;

public:
	Clear();
	~Clear();

	void Initialize();

	void Init();

	void Update();

	void Draw();

	bool GetButtonFlag() { return buttonFlag; }
	float GetVolume() { return volume; }
	float SetVolume(float volume) { return this->volume = volume; }
	bool SetAudioFlag(bool audioFlag) { return this->audioFlag = audioFlag; }

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://��`

	//���f�[�^
	SoundData sound1;
	SoundData sound2;



	//�X�v���C�g�f�[�^
	SpriteData spriteGraph;
	SpriteData BGGraph;

	SpriteData textGraph;
	SpriteData title;

	//�I�u�W�F�N�g�f�[�^
	Object::ObjectData PlayerObject;
	Vec3 position{ 0,-0.5,0 };



	float volume = 1.0f;
	//�F�֌W
	int colorTimer = 60;//�t���O��ON�ɂȂ�܂ł̎���
	bool colorFlag = false;//�F�ύX�t���O
	float color = 1;//�F

	bool TextFlag = false;//���̕����̃t���O
	bool buttonFlag = false;//�{�^���������邩�̃t���O
	bool audioFlag = true;
};


