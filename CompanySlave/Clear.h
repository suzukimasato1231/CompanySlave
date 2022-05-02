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

	bool GetBottanFlag() { return bottanFlag; }
	float GetVolume() { return volume; }
	float SetVolume(float volume) { return this->volume = volume; }
	bool SetAudioFlag(bool audioFlag) { return this->audioFlag = audioFlag; }

private:
	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;
	DebugText debugText;
	Audio* audio = nullptr;
private://定義

	//音データ
	SoundData sound1;
	SoundData sound2;



	//スプライトデータ
	SpriteData spriteGraph;
	SpriteData BGGraph;

	SpriteData textGraph;
	SpriteData title;

	//オブジェクトデータ
	Object::ObjectData PlayerObject;
	Vec3 position{ 0,0,0 };



	float volume = 1.0f;

	int colorTimer = 60;

	bool colorFlag = false;
	float color = 1;
	bool TextFlag = false;
	bool bottanFlag = false;
	bool audioFlag = true;
};


