#pragma once
#include"Title.h"
#include"PlayScene.h"
#include"Clear.h"
#include "Object.h"
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

	Camera* camera = nullptr;
	LightGroup* lightGroup = nullptr;

	enum Scene {
		titleScene,
		stage1,
		clearScene
	};
	//�ŏ��̃V�[��
	Scene scene = titleScene;

	Title* title = nullptr;
	PlayScene* play = nullptr;
	Clear* clear = nullptr;
	DebugText debugText;
	SpriteData BGGraph;
	SpriteData LoadUIGraph[7];



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


	//�f�o�b�N�p
	int stageDebug = 1;

	//����
	float volume;

};