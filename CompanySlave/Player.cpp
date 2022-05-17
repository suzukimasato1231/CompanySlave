#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
#include<time.h>
#include<random>

Player::Player()
{}

Player::~Player()
{
	//XAudio2���
	audio->xAudio2.Reset();
	//���f�[�^���
	Audio::SoundUnload(&sound1);
	Audio::SoundUnload(&sound2);
	Audio::SoundUnload(&sound3);
	safe_delete(audio);
}

void Player::Init()
{
	//Audio�N���X�쐬
	audio = Audio::Create();
	sound1 = Audio::SoundLoadWave("Resources/Music/SE/katana.wav");
	sound2 = Audio::SoundLoadWave("Resources/Music/SE/hiken.wav");
	sound3 = Audio::SoundLoadWave("Resources/Music/SE/return.wav");

	playerSwordWalkObject[0] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[1] = Object::Instance()->CreateOBJ("playerKari2-2", "playerOBJ/");
	playerSwordWalkObject[2] = Object::Instance()->CreateOBJ("playerKari2-1", "playerOBJ/");
	playerSwordWalkObject[3] = Object::Instance()->CreateOBJ("playerKari2-3", "playerOBJ/");
	playerDamageObject = Object::Instance()->CreateOBJ("playerDead", "playerOBJ/");

	playerAttackObject[0] = Object::Instance()->CreateOBJ("playerAttack1-1", "playerOBJ/");
	playerAttackObject[1] = Object::Instance()->CreateOBJ("playerAttack1-2", "playerOBJ/");
	playerAttackObject[2] = Object::Instance()->CreateOBJ("playerAttack1-3", "playerOBJ/");
	playerAttackObject[3] = Object::Instance()->CreateOBJ("playerAttack2-1", "playerOBJ/");
	playerAttackObject[4] = Object::Instance()->CreateOBJ("playerAttack2-2", "playerOBJ/");
	playerAttackObject[5] = Object::Instance()->CreateOBJ("playerAttack2-3", "playerOBJ/");
	playerAttackObject[6] = Object::Instance()->CreateOBJ("playerAttack3-1", "playerOBJ/");
	playerAttackObject[7] = Object::Instance()->CreateOBJ("playerAttack3-2", "playerOBJ/");
	playerAttackObject[8] = Object::Instance()->CreateOBJ("playerAttack3-3", "playerOBJ/");

	playerLifeObject[0] = Object::Instance()->CreateOBJ("playerKari4-1", "playerOBJ/");
	playerLifeObject[1] = Object::Instance()->CreateOBJ("playerKari4-2", "playerOBJ/");

	swordObject = Object::Instance()->CreateOBJ("sword");
	tornadoObject = Object::Instance()->CreateOBJ("tornado");
	swordEffectObject = Object::Instance()->CreateOBJ("Effect");
	cursorGraph = Object::Instance()->LoadTexture(L"Resources/Effect/Line.png");
	cursorObject = Shape::CreateRect(5, 0.5);

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;

	//�v���C���[HP
	HPGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/HPGauge.png");
	HPGaugeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	HPGaugeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/yellow.png");
	//�\�[�h�Q�[�WUI
	swordGraph[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge15.png");
	swordGraph[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge14.png");
	swordGraph[2] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge13.png");
	swordGraph[3] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge12.png");
	swordGraph[4] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge11.png");
	swordGraph[5] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge10.png");
	swordGraph[6] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge9.png");
	swordGraph[7] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge8.png");
	swordGraph[8] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge7.png");
	swordGraph[9] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge6.png");
	swordGraph[10] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge5.png");
	swordGraph[11] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge4.png");
	swordGraph[12] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge3.png");
	swordGraph[13] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge2.png");
	swordGraph[14] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge1.png");

	swordGargeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	swordGargeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/blue.png");
#if _DEBUG
	attackField = Shape::CreateRect(10.0, 20.0f);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");
	normalFieldOBJ = Shape::CreateRect(normalLength, normalLengthSub);
	normalFieldOBJ2 = Shape::CreateRect(normalLength, normalLength);
#endif
	AttackEffectOBJ = Shape::CreateRect(AttackEffectSize, AttackEffectSize);
	AttackEffectGraph[0] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect1.png");
	AttackEffectGraph[1] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect2.png");
	AttackEffectGraph[2] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect3.png");
	AttackEffectGraph[3] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect4.png");
	AttackEffectGraph[4] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect5.png");
	AttackEffectGraph[5] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect6.png");
	AttackEffectGraph[6] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect7.png");
	AttackEffectGraph[7] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect8.png");
	AttackEffectGraph[8] = Object::Instance()->LoadTexture(L"Resources/Effect/2effect9.png");

	AttackEffectGraph2[0] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect1.png");
	AttackEffectGraph2[1] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect2.png");
	AttackEffectGraph2[2] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect3.png");
	AttackEffectGraph2[3] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect4.png");
	AttackEffectGraph2[4] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect5.png");
	AttackEffectGraph2[5] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect6.png");
	AttackEffectGraph2[6] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect7.png");
	AttackEffectGraph2[7] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect8.png");
	AttackEffectGraph2[8] = Object::Instance()->LoadTexture(L"Resources/Effect/4effect9.png");

	swordUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/0White.png");
	swordUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/1White.png");
	swordUI[2] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/2White.png");
	swordUI[3] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/3White.png");
	swordUI[4] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/4White.png");
	swordUI[5] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/5White.png");
	swordUI[6] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/6White.png");
	swordUI[7] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/7White.png");
	swordUI[8] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/8White.png");
	swordUI[9] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/9White.png");

	skillUI[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill1.png");
	skillUI[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/swordSkill2.png");

	swordNot = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordNot.png");

	swordRotationGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordRotion.png");

	portionSprite[0] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");
	portionSprite[1] = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/Recovery.png");

	BlackGraph = Sprite::Instance()->SpriteCreate(L"Resources/BlackOut.png");
}

void Player::LoopInit()
{
	HP = HPMAX;
	//�|�[�V����
	portion = portionMax;
}

void Player::StageInit(int stageNum)
{
	//��
	char* Filepath = (char*)"";
	switch (stageNum)
	{
	case 1:
		//�|�[�V����
		portion = portionMax;
		HP = HPMAX;
		Filepath = (char*)"Resources/map/Enemy_Tile1.csv";
		break;
	case 2:
		Filepath = (char*)"Resources/map/Enemy_Tile2.csv";
		break;
	case 3:
		Filepath = (char*)"Resources/map/Enemy_Tile3.csv";
		break;
	case 4:
		//�|�[�V����
		portion = portionMax;
		HP = HPMAX;
		Filepath = (char*)"Resources/map/Enemy_Tile4.csv";
		break;
	case 5:
		Filepath = (char*)"Resources/map/Enemy_Tile5.csv";
		break;
	case 6:
		Filepath = (char*)"Resources/map/Enemy_Tile6.csv";
		break;
	case 7:
		//�|�[�V����
		portion = portionMax;
		HP = HPMAX;
		Filepath = (char*)"Resources/map/Enemy_Tile7.csv";
		break;
	case 8:
		Filepath = (char*)"Resources/map/Enemy_Tile8.csv";
		break;
	case 9:
		Filepath = (char*)"Resources/map/Enemy_Tile9.csv";
		break;
	default:
		break;
	}
	int spawnMap[MAP_HEIGHT][MAP_WIDTH] = {};
	const float mapSize = 10.0f;
	LoadCSV(spawnMap, Filepath);

	Vec3 memoryPos = {};
	for (size_t j = 0; j < MAP_HEIGHT; j++)
	{
		for (size_t i = 0; i < MAP_WIDTH; i++)
		{
			if (spawnMap[j][i] == 1)
			{
				memoryPos = { i * mapSize, 5, j * (-mapSize) };
				break;
			}
		}
	}
	position = memoryPos;				//���W
	oldPosition = position;				//1�O�̍��W
	//���W�����킹��
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
	direction = Right;					//�v���C���[�̌���

	//�X�e�[�W�ς�������̏�����
	walkCount = 0;						//�`��p�J�E���g
	walkNo = 0;							//�`�悷��NO
	attackCount = 0;					//�`��p�J�E���g
	attackNo = 0;						//�`�悷��NO
	attackMode = false;
	damageTime = 0;						//�_���[�W�H��������̌����ڗp
	moveFlag = false;					//�����Ă��邩�ǂ���
	//���
	avoidanceFlag = false;				//��𒆂�
	avoidanceTime = 0;					//���������
	avoiCoolTime = 0;					//������N�[���^�C��
	//�ʏ�U��
	normalAttackFlag.fill(false);//�ʏ�U���\��

	enemyDamegeTime.fill(60.0f);

	normalAttackTime = 0;							//�U���ƍU���̊�
	normalDirection = 0;							//�U���̌���
	normalAttackCount = 0;							//�ʏ�U���̉���ڂ�
	normalGraceTime = 0;							//�A���؂�܂ł̗P�\

   //���U��
	for (int i = 0; i < 7; i++)
	{
		swordPosition[i] = position;	//���W
		swordAngle[i] = { 0.0f,0.0f,0.0f };		//�p�x
		swordAngleVec[i] = 0;//��΂�����
		isSwordAttack[i] = false;//�A�^�b�N�t���O
		stingCnt[i] = 0;//�h����܂ł̎���
		haveSword[i] = true;//�����Ă錕
	}
	shotNo = 0;//�ǂ̌���
	returnFlag = false;//�����߂鎞�̃t���O
	nowTime = 0;//�����߂鎞�̃��[�v
	timeRate = 0;//�����߂鎞�̃��[�v
	srand(time(NULL));
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < eNumMax; j++)
		{
			isEnemySting[i][j] = false;
		}
	}
	swordCoolTimeFlag = false;
	swordCoolTime = swordCoolTimeMax;
	//�G�t�F�N�g�֌W
	AttackEffect = false;
	effectTime = 10;
	effectCount = 0;

	eslowTime = 120;

	//�|�[�V����
	portionFlag = false;
	portionTime = 0;
}

void Player::Update(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	//�E�X�e�B�b�N�ɓ��͂����邩�ǂ���
	if (!Input::Instance()->ConRightInput()) {
		homingFlag = true;
	}
	else {
		homingFlag = false;
	}

	Angle();

	SwordAngle();
	//���G���Ԃ̍X�V
	InvincivleUpdate();

	//�ړ�
	Move();
	//�v���C���[�̌��������߂�
	PDirection();
	//�ʏ�U��
	NormalAttack(enemy);
	//���U��
	SwordAttack(enemy);
	//��
	LifePortion();
	//���
	Avoidance();
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > 990)
	{
		position.x = 990;
	}
	if (position.z > 0)
	{
		position.z = 0;
	}
	if (position.z < -390)
	{
		position.z = -390;
	}
	//���W�����킹��
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);


	audio->SetVolume(volume);
}

void Player::Draw()
{
	//�v���C���[
	if (damageTime % 2 == 0)
	{

		if (invincivleTime != 0) { Object::Instance()->Draw(playerDamageObject, position, scale, angle, color); }
		else if (portionFlag == true) { Object::Instance()->Draw(playerLifeObject[portionNo], position, scale, angle, color); }
		else if (attackMode == false) { Object::Instance()->Draw(playerSwordWalkObject[walkNo], position, scale, angle, color); }
		else if (attackMode == true)
		{
			Object::Instance()->Draw(playerAttackObject[attackNo], position, scale, angle, color);
#if _DEBUG
			if (direction == Up) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == Down) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == Left) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x - normalLength / 2, position.y, position.z), scale, Vec3(90.0f, 0.0f, 0.0f), color, redColor);
			}
			else if (direction == Right) {
				Object::Instance()->Draw(normalFieldOBJ, Vec3(position.x + normalLength / 2, position.y, position.z), scale, Vec3(90.0f, 0.0f, 0.0f), color, redColor);
			}
			else if (direction == UpLeft) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x - normalLength / 2, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == UpRight) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x + normalLength / 2, position.y, position.z + normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == DownLeft)
			{
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x - normalLength / 2, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
			else if (direction == DownRight) {
				Object::Instance()->Draw(normalFieldOBJ2, Vec3(position.x + normalLength / 2, position.y, position.z - normalLength / 2), scale, Vec3(90.0f, 90.0f, 0.0f), color, redColor);
			}
#endif
		}
	}
	if (homingFlag == false)
	{//���̔�ԕ����̕`��
		Object::Instance()->Draw(cursorObject, { position.x,0,position.z, }, { 10,2,1 }, { 90,Rangle.y,0 }, color, cursorGraph);
	}

	for (int i = 0; i < 7; i++)
	{
		if (returnFlag == true && haveSword[i] == false)
		{
			Object::Instance()->Draw(swordEffectObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 0.5f,0.5f ,2.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i] + 90, swordAngle[i].z }, color);
		}
		else if (haveSword[i] == true || isSwordAttack[i] == false)
		{
			Object::Instance()->Draw(swordObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 1.5f,1.5f ,3.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i], swordAngle[i].z }, color);
		}
		else
		{
			Object::Instance()->Draw(swordEffectObject, { swordPosition[i].x,swordPosition[i].y,swordPosition[i].z }, { 0.5f,0.5f ,2.0f }, { swordAngle[i].x,swordAngle[i].y + reverseAngle[i] + 90, swordAngle[i].z }, color);
		}
	}
	if (returnFlag)
	{
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle + 120,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale,tornadoScale + 0.2f,tornadoScale }, { 0,tornadoAngle + 240,0 }, color);

		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 285,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 45,0 }, color);
		Object::Instance()->Draw(tornadoObject, { havePosition.x,havePosition.y,havePosition.z }, { tornadoScale - 0.2f,tornadoScale - 0.2f,tornadoScale - 0.2f }, { 0,tornadoAngle + 165,0 }, color);
	}
	if (BlackFlag == true)
	{
		Sprite::Instance()->Draw(BlackGraph, { 0,0 }, window_width, window_height, { 0.0f, 0.0f }, { 1, 1, 1 ,1 });
	}
	EffectDraw();
}

//�ړ�
void Player::Move()
{
	oldPosition = position;
	//�����A�j���[�V�����p
	if (walkCount >= 5)
	{
		walkCount = 0;
		walkNo++;
	}
	if (walkNo >= 4)
	{
		walkNo = 0;
	}
	//�ړ�
	if ((Input::Instance()->KeybordInputArrow() || Input::Instance()->ConLeftInput()) && portionFlag == false)
	{
		//�����ύX
		if (Input::Instance()->KeybordPush(DIK_RIGHT)) { angle.y = 0; }
		else if (Input::Instance()->KeybordPush(DIK_LEFT)) { angle.y = 180; }
		else if (Input::Instance()->KeybordPush(DIK_UP)) { angle.y = 270; }
		else if (Input::Instance()->KeybordPush(DIK_DOWN)) { angle.y = 90; }
		if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
			angle.y = 315;
		}
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
			angle.y = 225;
		}
		else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN))
		{
			angle.y = 45;
		}
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN))
		{
			angle.y = 135;
		}

		if (Input::Instance()->ConLeftInput() && portionFlag == false)
		{
			angle.y = XMConvertToDegrees(atan2(sinRad, cosRad)) - 90;
		}
		walkCount++;//�A�j���[�V�����̃^�C�}�[
		moveFlag = true;
	}
	else {
		walkNo = 0;
		moveFlag = false;
	}
	if (avoidanceTime <= 0 && normalAttackTime <= 0 && portionFlag == false)
	{
		//�ړ�
		//�L�[�{�[�h
		if (Input::Instance()->KeybordPush(DIK_RIGHT))
		{
			position.x += speed.x * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_LEFT))
		{
			position.x -= speed.x * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_UP))
		{
			position.z += speed.z * slowValue;
		}
		if (Input::Instance()->KeybordPush(DIK_DOWN))
		{
			position.z -= speed.z * slowValue;
		}
		//�R���g���[���[
		if (Input::Instance()->ConLeftInput())
		{
			position.x += speed.x * sinRad * slowValue;
			position.z += speed.z * cosRad * slowValue;
		}
	}
}

void Player::NormalAttack(Enemy* enemy)
{
	if (attackMode == true) { attackCount++; }//�A�j���[�V�����̃J�E���g
	if ((Input::Instance()->KeybordTrigger(DIK_D) || Input::Instance()->ControllerDown(ButtonX)) && avoidanceTime <= 0 && portionFlag == false)
	{
		attackMode = true;
		//�A���U���𐔂���
		if (normalAttackCount < 3)
		{
			audio->SoundSEPlayWave(sound1);
			normalAttackFlag[normalAttackCount] = true;
			normalAttackCount++;
			effectCount2 = 0;
			effectCount = 0;
		}
		AttackEffect = true;
	}
	if (normalAttackCount == 2)
	{
		attackCombo2check = true;
	}
	else
	{
		attackCombo2check = false;
	}
	//�`�悷��NO
	//if (normalAttackTime == normalAttackTimeMax)
	//{
	//	/*if (normalAttackFlag[0]) { attackNo = 0; }
	//	else if (normalAttackFlag[1]) { attackNo = 3; }
	//	else if (normalAttackFlag[2]) { attackNo = 6; }*/
	//	//attackCount = 0;//�J�E���g���Z�b�g
	//}

	//�U�����Ă��鎞��
	if (normalAttackTime > 0)
	{
		normalAttackTime--;
		if (normalAttackFlag[0] == false && normalAttackFlag[1] == false && normalAttackFlag[2] == false && normalAttackCount == 3)
		{
			position.x += attackMoveHighSpeed * sinRad * slowValue;
			position.z += attackMoveHighSpeed * cosRad * slowValue;
		}
		else
		{
			position.x += attackMoveSpeed * sinRad * slowValue;
			position.z += attackMoveSpeed * cosRad * slowValue;
		}
	}

	for (int j = 0; j < 3; j++)
	{//�U������͈�ԍŏ�
		if (normalAttackFlag[j] == true && normalAttackTime <= 0)
		{
			normalAttackFlag[j] = false;
			NormalFieldDirection();
			bool swordCoolTimeFlag = false;
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if ((enemy->GetHP(i) > 0 && enemy->GetType(i) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(i) == BossTwinBoar)) {
					if (Collision::CheckBox2Box(enemy->GetBox(i), normalAttackBox))
					{
						enemy->DamegeNormal(i, direction);
						enemy->SetDamegeFlag(i, true);
						swordCoolTimeFlag = true;
					}
				}
			}
			//20���̊m���łT�b�������N�[���^�C������
			if (swordCoolTimeFlag == true)
			{
				std::random_device rnd;
				std::mt19937 mt(rnd());
				std::uniform_int_distribution<>rand100(0, 99);//0~99�͈̔�
				int num = rand100(mt);
				if (num < 20)
				{//�Q�O�����Ȃ�N�[���^�C���T�b����
					swordCoolTimePlas += 5;
				}
			}

			normalAttackTime = normalAttackTimeMax;
			normalGraceTime = normalGraceTimeMax;
			break;
		}
	}
	//�A���؂�܂ł̗P�\
	if (normalGraceTime >= 0 && normalAttackFlag[0] == false && normalAttackFlag[1] == false && normalAttackFlag[2] == false)
	{
		normalGraceTime--;
		//���ԏI�������A���؂胊�Z�b�g
		if (normalGraceTime <= 0)
		{
			normalAttackCount = 0;
			attackMode = false;
			attackNo = 0;
			AttackEffect = false;
			effectCount2 = 0;
			effectCount = 0;
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				enemy->SetDamegeFlag(i, false);
			}
		}
	}
	if (normalAttackCount == 0) {
		AttackScale = { 3.0f,3.0f,3.0f };
	}
	if (normalAttackCount == 1) {
		AttackScale = { 3.0f,3.0f,3.0f };
	}
	if (normalAttackCount == 2) {
		AttackScale = { 4.0f,4.0f,4.0f };
	}
	if (normalAttackCount == 3) {
		AttackScale = { 5.0f,5.0f,5.0f };
	}
	if (normalAttackTime == normalAttackTimeMax / 2) { AttackEffect = true; }

	if (AttackEffect == true) {
		if (effectCount2 != 8)
		{
			effectCount++;
		}
		if (effectCount > 1) {
			effectCount2++;
			effectCount = 0;
		}
		if (effectCount2 == 8) {
			AttackEffect = false;
		}
	}
	eslowFlag = true;
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			eslowFlag = false;
		}
	}
	if (eslowFlag == true) {
		if (eslowTime > 0) {
			eslowTime--;
			slowValue = 0.15;
		}
		else if (eslowTime <= 0) {
			slowValue = 1;
			eslowFlag = false;
		}
	}

	//�A�j���[�V�����p
	if (effectCount2 == 0 || effectCount2 == 1 || effectCount2 == 2)
	{
		attackNo = 1;
	}
	if (effectCount2 == 3 || effectCount2 == 4 || effectCount2 == 5)
	{
		attackNo = 2;
	}
	if (effectCount2 == 6 || effectCount2 == 7 || effectCount2 == 8)
	{
		attackNo = 2;
	}
	if (normalAttackCount == 2)
	{
		if (effectCount2 == 0 || effectCount2 == 1 || effectCount2 == 2)
		{
			attackNo = 4;
		}
		if (effectCount2 == 3 || effectCount2 == 4 || effectCount2 == 5)
		{
			attackNo = 5;
		}
		if (effectCount2 == 6 || effectCount2 == 7 || effectCount2 == 8)
		{
			attackNo = 6;
		}
	}

}

//������
void Player::SwordAttack(Enemy* enemy)
{

	//����
	if (Input::Instance()->ControllerDown(ButtonRB) && haveSword[shotNo] && !returnFlag && portionFlag == false)
	{
		audio->SoundSEPlayWave(sound2);
		swordPosition[shotNo] = position;
		swordAngle[shotNo].z = 0;
		swordAngle[shotNo] = Rangle;
		if (homingFlag && enemy->GetEnemySize() > 0)
		{
			swordAngle[shotNo] = Vec3{ 0,(XMConvertToDegrees(atan2(enemy->GetPosition(EnemyNeedNumber(enemy)).x - position.x, enemy->GetPosition(EnemyNeedNumber(enemy)).z - position.z)) + 270),0 };
		}
		isSwordAttack[shotNo] = true;
		haveSword[shotNo] = false;
		holdingFlag[shotNo] = false;
	}

	//���������ւ�
	if (!haveSword[shotNo])
	{
		shotNo++;
	}
	if (shotNo >= 7)
	{
		shotNo = 0;
	}

	//���߂��Ă�������
	if (Input::Instance()->ControllerDown(ButtonLB) && swordCoolTimeFlag == false && portionFlag == false)
	{

		if (IsSwordALLHave() == true)
		{//����S�������Ă����ꍇ
			swordCoolTimeFlag = true;
			start_time = time(NULL);//�N�[���^�C���v���J�n
			swordCoolTimePlas = 0;
			slowValue = 0.15;
			slowFlag = true;
			BlackFlag = true;
			for (int i = 0; i < 7; i++)
			{
				reverseAngle[i] = 0;
				reverseValue[i] = 0;
				isSwordAttack[i] = false;
				swordStop[i] = false;
				stingCnt[i] = 0;
				holdingFlag[i] = true;
				swordAngle[i].z = 0;
			}
			returnFlag = true;
			tornadoScale = 0.5f;
			audio->SoundSEPlayWave(sound3);
		}
		else
		{//����ł��Ȃ����̕`�ʂ̎��Ԑݒ�
			swordNotTime = swordNotTimeMax;
		}

	}
	for (int i = 0; i < 7; i++)
	{
		for (int k = 0; k < 25; k++)
		{
			if (position.x + 6.0 < havePosition.x)
			{
				havePosition.x -= 0.1f;
			}
			else if (position.x - 6.0 > havePosition.x)
			{
				havePosition.x += 0.1f;
			}
		}
		for (int k = 0; k < 25; k++)
		{
			if (position.z + 6.0 < havePosition.z)
			{
				havePosition.z -= 0.1f;
			}
			else if (position.z - 6.0 > havePosition.z)
			{
				havePosition.z += 0.1f;
			}
		}

		if (haveSword[i])
		{
			swordAngle[i].z = -90;
			swordAngle[i].y = 51.4 * i;

			swordPosition[i] = havePosition;
		}
	}
	//���߂��Ă�������
	if (returnFlag)
	{

		tornadoScale -= 0.01 * slowValue;
		tornadoAngle += 24 * slowValue;
		if (slowFlag)
		{
			slowCount++;
		}
		if (slowCount > 10)
		{
			slowValue = 1;
			slowCount = 0;
			slowFlag = false;
		}
		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		for (int i = 0; i < 7; i++)
		{
			if (!haveSword[i])
			{
				//�����̕�����
				swordAngle[i].y = XMConvertToDegrees(atan2(havePosition.x - swordPosition[i].x, havePosition.z - swordPosition[i].z)) + 270;
				//�h�������t���O����
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					isEnemySting[i][j] = false;
					enemy->SetDamegeFlag(j, false);
				}

				swordPosition[i].x += cos((swordAngle[i].y * 3.14) / -180) * swordSpeed * slowValue;      // x���W���X�V
				swordPosition[i].z += sin((swordAngle[i].y * 3.14) / -180) * swordSpeed * slowValue;      // z���W���X�V

				//�߂��Ă�Ƃ��̓����蔻��
				for (size_t j = 0; j < enemy->GetEnemySize(); j++)
				{
					if ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)) {
						if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
						{
							enemy->DamegeSword(j);
							if (enemyDamegeTime[j] > 0) {
								enemy->SetDamegeFlag(j, true);
							}
						}
					}
				}
				//�����v���C���[�ɖ߂��Ă��铖���蔻��
				if (Collision::CheckBox2Box(haveBox, swordAttackBox[i]) && holdingFlag[i])
				{
					haveSword[i] = true;
				}
			}
		}
		shotNo = 0;
		if (haveSword[0] && haveSword[1] && haveSword[2] && haveSword[3] && haveSword[4] && haveSword[5] && haveSword[6])
		{
			tornadoAngle = 0;
			shotNo = 0;
			timeRate = 0;
			nowTime = 0;
			slowValue = 1;
			returnFlag = false;
			BlackFlag = false;
		}
	}
	haveBox.minPosition = XMVectorSet(havePosition.x - (r + 5), havePosition.y - r, havePosition.z - (r + 5), 1);
	haveBox.maxPosition = XMVectorSet(havePosition.x + (r + 5), havePosition.y + r, havePosition.z + (r + 5), 1);

	for (int i = 0; i < 7; i++)
	{
		//�����蔻��̃{�b�N�X�̈ʒu��
		swordAttackBox[i].maxPosition = XMVectorSet(swordPosition[i].x + 2.0f, swordPosition[i].y, swordPosition[i].z + 2.0f, 1);
		swordAttackBox[i].minPosition = XMVectorSet(swordPosition[i].x - 2.0f, swordPosition[i].y, swordPosition[i].z - 2.0f, 1);

		//���̔�ԕ����ƌ����ւ�
		if (haveSword[i] && holdingFlag[i])
		{
			reverseAngle[i] = 0;
			reverseValue[i] = 0;
			stingCnt[i] = 0;
			swordAngleVec[i] = (Rangle.y * 3.14) / -180;
			if (homingFlag && enemy->GetEnemySize() > 0)
			{
				swordAngleVec[i] = ((XMConvertToDegrees(atan2(enemy->GetPosition(EnemyNeedNumber(enemy)).x - position.x, enemy->GetPosition(EnemyNeedNumber(enemy)).z - position.z)) + 270) * 3.14) / -180;
			}
			swordStop[i] = false;
			stingCnt[i] = 0;
		}

		//�U��(���ł���~�܂�܂�)
		if (isSwordAttack[i] == true)
		{
			//�G�Ƃ̓����蔻��
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)))
				{
					isSwordAttack[i] = false;
					isEnemySting[i][j] = true;
					enemy->DamegeThrowSword(j);
					if (enemyDamegeTime[j] > 0) {
						enemy->SetDamegeFlag(j, true);
					}
				}
			}
			//�p�x�Ői�߂Ă�
			for (int s = 0; s < swordSpeed; s++)
			{
				swordPosition[i].x += cos(swordAngleVec[i] + reverseValue[i]) * 1 * slowValue;      // x���W���X�V
				swordPosition[i].z += sin(swordAngleVec[i] + reverseValue[i]) * 1 * slowValue;      // z���W���X�V
			}
		}
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (enemy->GetDamegeFlag(j) == true) {
				enemyDamegeTime[j] -= 0.1f;
			}
			if (enemy->GetDamegeFlag(j) == false) {
				enemyDamegeTime[j] = 60.0f;
			}
			if (enemyDamegeTime[j] <= 0) {
				enemy->SetDamegeFlag(j, false);
			}
		}
		//�������Ď��Ƃ��̓����蔻�肽��
		if (isSwordAttack[i] == false && !returnFlag)
		{
			if (Collision::CheckBox2Box(pBox, swordAttackBox[i]) && holdingFlag[i])
			{
				haveSword[i] = true;
			}
		}

		//�h�������G�Ɍ����ǂ�������
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (isEnemySting[i][j])
			{
				swordPosition[i] = enemy->GetPosition(j);
			}
		}

		if (swordStop[i] == true)
		{
			stingCnt[i]++;//�h����J�E���g	
			reverseAngle[i] = 180;
		}

		//�~�܂�
		if (stingCnt[i] >= 2 + rand() % 20)
		{
			swordAngle[i].z = -90;
			isSwordAttack[i] = false;
			swordStop[i] = false;
			stingCnt[i] = 0;
			holdingFlag[i] = true;
		}

		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (enemy->GetHP(j) <= 0 && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
				isEnemySting[i][j] = false;
				enemy->SetExplosionFlag(j);
			}

			if (enemy->GetExplosionFlag(j) == true && isEnemySting[i][j] == true)
			{
				explosion[i] = true;
				isEnemySting[i][j] = false;
				explosionAngle[i] = rand() % 360;
				enemy->SetExplosionFlag(j);
				enemy->SetExplosionCount(j);
			}
		}

		if (explosion[i])
		{
			explosionCount[i]++;
			swordAngle[i].z = 0;
			swordAngle[i].y = -explosionAngle[i] - 180;
			swordPosition[i].x += cos(XMConvertToRadians(explosionAngle[i])) * 5 * slowValue;      // x���W���X�V
			swordPosition[i].z += sin(XMConvertToRadians(explosionAngle[i])) * 5 * slowValue;      // z���W���X�V
			holdingFlag[i] = true;

			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (explosionCount[i] >= 3 && isEnemySting[i][j] == false)
				{
					//�G�Ƃ̓����蔻��

					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && ((enemy->GetHP(j) > 0 && enemy->GetType(j) != BossWolfFlock) || (enemy->TwinBoarHP() > 0 && enemy->GetType(j) == BossTwinBoar)))
					{
						isSwordAttack[i] = false;
						isEnemySting[i][j] = true;
						enemy->DamegeThrowSword(j);
						if (enemyDamegeTime[j] > 0) {
							enemy->SetDamegeFlag(j, true);
						}
					}
					if (enemy->GetDamegeFlag(j) == true) {
						enemyDamegeTime[j] -= 0.1f;
					}
					if (enemy->GetDamegeFlag(j) == false) {
						enemyDamegeTime[j] = 60.0f;
					}
					if (enemyDamegeTime[j] <= 0) {
						enemy->SetDamegeFlag(j, false);
					}
				}
			}
			if (explosionCount[i] >= 6)
			{
				swordAngle[i].z = -90;
				explosionCount[i] = 0;
				explosion[i] = false;
			}
		}
	}
	//���̉���N�[���^�C��
	if (swordCoolTimeFlag == true)
	{
		end_time = time(NULL);
		swordCoolTime = end_time - start_time;//���Ԍv�Z
		if (swordCoolTime + swordCoolTimePlas >= swordCoolTimeMax)
		{
			swordCoolTimePlas = 0;
			swordCoolTime = swordCoolTimeMax;
			swordCoolTimeFlag = false;
		}
	}
	//����s�����̉��o
	if (swordNotTime > 0)
	{
		swordNotTime--;
		if (swordNotTime < 0)
		{
			swordNotTime = 0;
		}
	}
	eslowFlag = true;
	for (int e = 0; e < enemy->GetEnemySize(); e++)
	{
		if (enemy->GetHP(e) > 0)
		{
			eslowFlag = false;
		}
	}
	if (eslowFlag == true)
	{
		if (eslowTime > 0)
		{
			eslowTime--;
			slowValue = 0.15;
		}
		else if (eslowTime <= 0)
		{
			slowValue = 1;
			eslowFlag = false;
		}
	}
	/*else if (eslowFlag == false)
	{
		slowValue = 1;
	}*/
}
void Player::LifePortion()
{
	//HP�������Ă��Ċ��|�[�V�����������Ă����ꍇ
	if ((Input::Instance()->KeybordTrigger(DIK_Q) || Input::Instance()->ControllerDown(ButtonY))
		&& portion > 0 && HP < HPMAX && portionFlag == false && avoidanceTime <= 0 && normalAttackTime <= 0)
	{
		portion--;
		HP += 4;
		if (HP > HPMAX)
		{
			HP = HPMAX;
		}
		//�񕜎����P������
		portionTime = portionTimeMax;
		portionFlag = true;
		portionNo = 0;
	}
	//�P�����Ԍ���
	if (portionFlag == true)
	{
		portionTime--;
		if (portionTime < 25)
		{
			portionNo = 1;
		}
		if (portionTime <= 0)
		{
			portionFlag = false;
		}
	}
}
void   Player::Angle()
{
	float rad = 0.0f;
	if (Input::Instance()->KeybordInputArrow())
	{
		//�E��
		if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
			rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		}//�E��
		else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
			rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		}//����
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
			rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		}//����
		else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
			rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		}//��
		else if (Input::Instance()->KeybordPush(DIK_UP)) {
			rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		}//�E
		else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
			rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		}//��
		else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
			rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		}//��
		else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
			rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		}
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}
	if (Input::Instance()->ConLeftInput())
	{
		rad = Input::Instance()->GetLeftAngle();
		sinRad = sinf(-rad);
		cosRad = cosf(rad);
	}
}

void Player::SwordAngle()
{
	//�E�R���g���[��
	if (Input::Instance()->ConRightInput())
	{
		float rad = Input::Instance()->GetRightAngle();
		RsinRad = sinf(-rad);
		RcosRad = cosf(rad);
		Rangle.y = XMConvertToDegrees(atan2(RsinRad, RcosRad)) - 90;
	}
}

void Player::Avoidance()
{
	//����N�[���^�C������
	if (avoiCoolTime > 0)
	{
		avoiCoolTime--;
		if (avoiCoolTime <= 0)
		{
			avoidanceFlag = false;
		}
	}
	//����J�n
	if ((Input::Instance()->KeybordTrigger(DIK_F) || Input::Instance()->ControllerDown(ButtonA))
		&& avoidanceFlag == false && normalAttackTime <= 0)
	{
		avoidanceFlag = true;
		avoidanceTime = avoidanceTimeMax;
		avoiDirection = direction;
	}
	//���
	if (avoidanceTime > 0)
	{
		avoidanceTime--;
		if (avoidanceTime <= 0)
		{
			avoiCoolTime = avoiCoolTimeMax;
		}
		position.x += avoiSpeed * sinRad * slowValue;
		position.z += avoiSpeed * cosRad * slowValue;
	}
}

void Player::PDirection()
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

void Player::Damage(int damegeNum)
{
	//�_���[�W��H�炤
	damageTime = damageTimeMax;
	HP -= damegeNum;

	invincivleTime = invincibleTimeMax;
}

void Player::UIDraw()
{
	//HP
	if (HP > 0)
	{
		Sprite::Instance()->Draw(HPGaugeSub, Vec2(130.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
		Sprite::Instance()->Draw(HPGaugeMain, Vec2(130.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
	}
	Sprite::Instance()->Draw(HPGraph, Vec2(80.0f, 30.0f), 500.0f, 30.0f);

	//�|�[�V����
	if (portion >= 2)
	{
		Sprite::Instance()->Draw(portionSprite[0], Vec2(160.0f, 70.0f), 40.0f, 40.0f);
	}
	if (portion >= 1)
	{
		Sprite::Instance()->Draw(portionSprite[1], Vec2(120.0f, 70.0f), 40.0f, 40.0f);
	}

	//�\�[�h�Q�[�W
	//���܂������̉��o
	if (swordCoolTimeFlag == false)
	{
		swordRotationGraph.rotation -= 2.0f;
		if (swordRotationGraph.rotation < -360)
		{
			swordRotationGraph.rotation = 0.0f;
		}
		Sprite::Instance()->Draw(swordRotationGraph, Vec2(65.0f, 65.0f), 120.0f, 120.0f, Vec2(0.5f, 0.5f));
	}
	//�ԍ�
	//�N�[���^�C�����Ԃ𑫂�
	int coolTime = (int)swordCoolTime + swordCoolTimePlas;
	if (swordCoolTimeMax <= swordCoolTime + swordCoolTimePlas)
	{
		coolTime = swordCoolTimeMax;
	}

	int number = (int)(swordCoolTimeMax - coolTime) % 10;
	int number2 = (int)(swordCoolTimeMax - coolTime) / 10;
	int swordCoolTimeNum = 15 - coolTime;
	if (swordCoolTimeNum >= 15)
	{
		swordCoolTimeNum = 14;
	}
	Sprite::Instance()->Draw(swordGraph[swordCoolTimeNum], Vec2(5.0f, 5.0f), 120.0f, 120.0f);
	if (coolTime != swordCoolTimeMax)
	{
		//���N�[���^�C������
		Sprite::Instance()->Draw(swordUI[number2], Vec2(20.0f, 36.0f), 45.0f, 45.0f);
		Sprite::Instance()->Draw(swordUI[number], Vec2(65.0f, 36.0f), 45.0f, 45.0f);
	}

	//����s����
	if (swordNotTime > 0)
	{
		Sprite::Instance()->Draw(swordNot, Vec2(5.0f, 5.0f), 120.0f, 120.0f);
	}
}

void Player::EffectDraw()
{
	if (AttackEffect == true)
	{
		switch (direction)
		{
		case Up:
			AttackAngle.y = 90.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z + r + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z + r + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}

			break;
		case Down:
			AttackAngle.y = 270.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z - r - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z - r - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case Left:
			AttackAngle.y = 0.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - r - AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - r - AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case Right:
			AttackAngle.y = 180.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + r + AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + r + AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			if (attackCombo2check == false)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount2]);
			}
			else if (attackCombo2check == true)
			{
				Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph2[effectCount2]);
			}
			break;
		}
	}
}

int Player::EnemyNeedNumber(Enemy* enemy)
{
	//�v���C���[�Ɉ�ԋ߂��G�����߂�
	float pNeedLength = 9999.9f;
	int number = 0;
	for (size_t i = 0; i < enemy->GetEnemySize(); i++)
	{
		if (enemy->GetHP(i) > 0 && enemy->GetType(i) != BossWolfFlock)
		{
			//�v���C���[�ƓG�Ƃ̋���
			float length = Vec3(position - enemy->GetPosition(i)).length();
			if (pNeedLength > length)
			{
				pNeedLength = length;
				number = i;
			}
		}
	}
	return number;
}

bool Player::IsSwordALLHave()
{
	bool Flag = false;
	for (int i = 0; i < 7; i++)
	{
		if (haveSword[i] == false)
		{
			Flag = true;
			break;
		}
	}
	return Flag;
}

Vec3 Player::GetCameraPos()
{
	Vec3 cameraPos = position;
	if (position.x < 110.0f)
	{
		cameraPos.x = 110.0f;

	}
	else if (position.x > 890.0f)
	{
		cameraPos.x = 890.0f;
	}
	if (position.z > -65.0f)
	{
		cameraPos.z = -65.0f;

	}
	else if (position.z < -350.0f)
	{
		cameraPos.z = -350.0f;
	}
	if (BlackFlag == true)
	{
		collectCount -= 0.25f;
		cameraPos.y += collectCount;
		if (cameraPos.y < -3.0f)
		{
			cameraPos.y = -3.0f;
			collectCount = -8.0f;
		}
	}
	else
	{
		collectCount += 0.1f;
		if (collectCount > 0.0f)
		{
			collectCount = 0.0f;
		}
	}
	return cameraPos;
}

void Player::NormalFieldDirection()
{
	switch (direction)
	{
	case Up:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLengthSub / 2, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLengthSub / 2, position.y, position.z, 1);
		break;
	case Down:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLengthSub / 2, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLengthSub / 2, position.y, position.z - normalLength, 1);
		break;
	case Left:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z + normalLengthSub / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLengthSub / 2, 1);
		break;
	case Right:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z + normalLengthSub / 2, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalLengthSub / 2, 1);
		break;
	case UpRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case UpLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z, 1);
		break;
	case DownRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z - normalLength, 1);
		break;
	case DownLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x, position.y, position.z, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLength, 1);
		break;
	}
}

void Player::InvincivleUpdate()
{
	if (invincivleTime > 0)
	{
		invincivleTime--;
		if (invincivleTime < 0)
		{
			invincivleTime = 0;
		}
	}
}
