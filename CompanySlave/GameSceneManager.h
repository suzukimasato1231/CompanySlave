#pragma once
#include"Title.h"
#include"PlayScene.h"
#include"SelectScene.h"

extern const int window_width;
extern const int window_height;
class GameSceneManager :public Singleton<GameSceneManager>
{
private:
	friend Singleton<GameSceneManager>;
	_DirectX* directX = nullptr;
public:
	GameSceneManager();
	~GameSceneManager();

	void Initialize(_DirectX* directX);

	void Init();

	void Update();

	void Draw();

private://��`
	enum Scene {
		titleSceneInit,
		titleScene,
		selectScene,
		stage1,
		scoreScene
	};
	//�ŏ��̃V�[��
	Scene scene = titleScene;

	Title* title = nullptr;
	PlayScene* play = nullptr;
	SelectScene* select = nullptr;
	DebugText debugText;
	Sprite::SpriteData BGGraph;
	Sprite::SpriteData LoadUIGraph[7];

	//�t�F�[�h�̃t���O(t���^�C�g���̂��s���V�[���`�F���W�̂��)
	bool tFadeFlag = false;
	bool sFadeFlag = false;
	//�t�F�[�h(t���^�C�g���̂��s���V�[���`�F���W�̂��)
	float tFade = 1;
	float sFade = 1;

	//�^�����[�h��ʊ֌W�ϐ�
	bool LoadFlag = false;
	int LoadTime = 70;
	int LoadCount = 0;
	int LoadCount2 = 0;
	//�V�[�����Ƃ̏�����
	bool initFlag = true;
};
