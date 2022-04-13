#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
#include<time.h>

Player::Player()
{}

Player::~Player()
{}

void Player::Init()
{
	playerSwordWalkObject[0] = Object::Instance()->CreateOBJ("playerKari2-1");
	playerSwordWalkObject[1] = Object::Instance()->CreateOBJ("playerKari2-2");
	playerSwordWalkObject[2] = Object::Instance()->CreateOBJ("playerKari2-1");
	playerSwordWalkObject[3] = Object::Instance()->CreateOBJ("playerKari2-3");

	playerAttackObject[0] = Object::Instance()->CreateOBJ("playerAttack1-1");
	playerAttackObject[1] = Object::Instance()->CreateOBJ("playerAttack1-2");
	playerAttackObject[2] = Object::Instance()->CreateOBJ("playerAttack1-3");
	playerAttackObject[3] = Object::Instance()->CreateOBJ("playerAttack2-1");
	playerAttackObject[4] = Object::Instance()->CreateOBJ("playerAttack2-2");
	playerAttackObject[5] = Object::Instance()->CreateOBJ("playerAttack2-3");
	playerAttackObject[6] = Object::Instance()->CreateOBJ("playerAttack3-1");
	playerAttackObject[7] = Object::Instance()->CreateOBJ("playerAttack3-2");
	playerAttackObject[8] = Object::Instance()->CreateOBJ("playerAttack3-3");

	swordObject = Object::Instance()->CreateOBJ("sword");
	swordEffectObject = Object::Instance()->CreateOBJ("Effect");
	cursorGraph = Object::Instance()->LoadTexture(L"Resources/Effect/Line.png");
	cursorObject = Shape::CreateRect(5, 0.5);

	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	pSphere.radius = r;
	pSphere.center = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;


	yellowColor = Object::Instance()->LoadTexture(L"Resources/color/yellow.png");

	//�v���C���[HP
	HPGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/HPGauge.png");
	HPGaugeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	HPGaugeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/yellow.png");
	//�\�[�h�Q�[�WUI
	swordGraph = Sprite::Instance()->SpriteCreate(L"Resources/playerUI/SwordGauge.png");
	swordGargeMain = Sprite::Instance()->SpriteCreate(L"Resources/color/red.png");
	swordGargeSub = Sprite::Instance()->SpriteCreate(L"Resources/color/blue.png");
#if _DEBUG
	attackField = Shape::CreateRect(10.0f, 20.0f);
	redColor = Object::Instance()->LoadTexture(L"Resources/color/red.png");

	normalFieldOBJ = Shape::CreateRect(normalLength, normalLength);
	//normalGraph = Object::Instance()->LoadTexture(L"Resources/kengeki.png");

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
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			isEnemySting[i][j] = false;
		}
	}
}

void Player::StageInit(int stageNum)
{
	//�X�e�[�W����
	switch (stageNum)
	{
	case 1:
		position = { 100.0f,0.0f,-120.0f };	//���W
		oldPosition = position;				//1�O�̍��W
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
		pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
		direction = Right;					//�v���C���[�̌���
		break;
	case 2:
		position = { 100.0f,0.0f,-120.0f };	//���W
		oldPosition = position;				//1�O�̍��W
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
		pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
		direction = Right;
		break;
	}
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
	for (int i = 0; i < 3; i++)
	{
		normalAttackFlag[i] = false;				//�ʏ�U���\��
	}
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

	//�G�t�F�N�g�֌W
	AttackEffect = false;
	effectTime = 10;
	effectCount = 0;

}

void Player::Update(Enemy* enemy)
{
	if (enemy == nullptr)
	{
		return;
	}

	Angle();

	SwordAngle();
	//�ړ�
	Move();
	//�v���C���[�̌��������߂�
	PDirection();
	//�ʏ�U��
	NormalAttack(enemy);
	//���U��
	SwordAttack(enemy);
	//���
	Avoidance();

	if (damageTime > 0)
	{
		damageTime--;
	}

	//���W�����킹��
	pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
	pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	pSphere.center = XMVectorSet(position.x, position.y, position.z, 1);
}

void Player::Draw()
{
#if _DEBUG
	DebugDraw();
#endif
	//�v���C���[
	if (damageTime % 2 == 0)
	{
		if (attackMode == false) { Object::Instance()->Draw(playerSwordWalkObject[walkNo], position, scale, angle, color); }
		if (attackMode == true) { Object::Instance()->Draw(playerAttackObject[attackNo], position, scale, angle, color); }
	}
	Object::Instance()->Draw(cursorObject, { position.x,-2,position.z, }, { 10,2,1 }, { 90,Rangle.y,0 }, color, cursorGraph);
	for (int i = 0; i < 7; i++)
	{
		if (returnFlag == true)
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
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
		|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
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

		if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft)
			|| Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
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
	if (avoidanceTime <= 0 && normalAttackTime <= 0)
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
		if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft) ||
			Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
		{
			position.x += speed.x * sinRad * slowValue;
			position.z += speed.z * cosRad * slowValue;
		}
		//���W�����킹��
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

void Player::NormalAttack(Enemy* enemy)
{//�����A�j���[�V�����p
	if (attackCount >= 10 && attackMode == true)
	{
		attackCount = 0;
		attackNo++;
		if (attackNo >= 9)
		{
			attackNo = 8;
		}
	}
	if (normalAttackFlag[2] && attackNo >= 9) { attackNo = 8; }
	if (normalAttackFlag[1] && attackNo >= 6) { attackNo = 5; }
	if (normalAttackFlag[0] && attackNo >= 3) { attackNo = 2; }


	if (attackMode == true) { attackCount++; }//�A�j���[�V�����̃J�E���g
	if ((Input::Instance()->KeybordTrigger(DIK_D) || Input::Instance()->ControllerDown(ButtonX)) && avoidanceTime <= 0)
	{
		attackMode = true;
		//�A���U���𐔂���
		if (normalAttackCount < 3)
		{
			normalAttackFlag[normalAttackCount] = true;
			normalAttackCount++;
		}
	}
	//�`�悷��NO
	if (normalAttackTime == normalAttackTimeMax)
	{
		if (normalAttackFlag[0]) { attackNo = 0; }
		else if (normalAttackFlag[1]) { attackNo = 3; }
		else if (normalAttackFlag[2]) { attackNo = 6; }
		attackCount = 0;//�J�E���g���Z�b�g
	}

	//�U�����Ă��鎞��
	if (normalAttackTime > 0)
	{
		normalAttackTime--;
		position.x += attackMoveSpeed * sinRad * slowValue;
		position.z += attackMoveSpeed * cosRad * slowValue;
	}

	for (int j = 0; j < 3; j++)
	{//�U������͈�ԍŏ�
		if (normalAttackFlag[j] == true && normalAttackTime <= 0)
		{
			normalAttackFlag[j] = false;
			NormalFieldDirection();
			for (size_t i = 0; i < enemy->GetEnemySize(); i++)
			{
				if (enemy->GetHP(i) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(i), normalAttackBox))
					{
						enemy->DamegeNormal(i, direction);

						enemy->SetDamegeFlag(i, true);

					}
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

		if (effectCount < 8) {
			effectCount++;
		}
		else if (effectCount == 8) {
			effectCount = 0;
			AttackEffect = false;
		}
	}
}

//������
void Player::SwordAttack(Enemy* enemy)
{
	if (Input::Instance()->KeybordPush(DIK_U))
	{

	}
	//����
	if (Input::Instance()->ControllerDown(ButtonRB) && haveSword[shotNo] && !returnFlag)
	{
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
	if (Input::Instance()->ControllerDown(ButtonLB))
	{
		slowValue = 0.25;
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
	}

	//���߂��Ă�������
	if (returnFlag)
	{

		nowTime += 0.1;
		timeRate = min(nowTime / endTime, 1);
		for (int i = 0; i < 7; i++)
		{
			//�����̕�����
			swordAngle[i].y = XMConvertToDegrees(atan2(position.x - swordPosition[i].x, position.z - swordPosition[i].z)) + 270;
			//�h�������t���O����
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				isEnemySting[i][j] = false;
				enemy->SetDamegeFlag(j, false);
			}

			//���[�v
			swordPosition[i] = Easing::easeIn(swordPosition[i], position, timeRate);

			//�߂��Ă�Ƃ��̓����蔻��
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (enemy->GetHP(j) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]))
					{
						enemy->DamegeSword(j);
						enemy->SetDamegeFlag(j, true);
					}
				}
			}
		}
		shotNo = 0;
		if (nowTime >= 5)
		{
			timeRate = 0;
			nowTime = 0;
			slowValue = 1;
			returnFlag = false;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		//�����蔻��̃{�b�N�X�̈ʒu��
		swordAttackBox[i].maxPosition = XMVectorSet(swordPosition[i].x + 2, swordPosition[i].y, swordPosition[i].z + 2, 1);
		swordAttackBox[i].minPosition = XMVectorSet(swordPosition[i].x - 2, swordPosition[i].y, swordPosition[i].z - 2, 1);

		//���̔�ԕ����ƌ����ւ�
		if (haveSword[i] && holdingFlag[i])
		{
			reverseAngle[i] = 0;
			reverseValue[i] = 0;
			stingCnt[i] = 0;
			swordAngle[i] = Rangle;
			swordPosition[i] = position;
			swordAngleVec[i] = (Rangle.y * 3.14) / -180;
			swordStop[i] = false;
			stingCnt[i] = 0;
		}

		//�U��(���ł���~�܂�܂�)
		if (isSwordAttack[i] == true)
		{
			//�G�Ƃ̓����蔻��
			for (size_t j = 0; j < enemy->GetEnemySize(); j++)
			{
				if (enemy->GetHP(j) > 0) {
					if (Collision::CheckSphere2Box(enemy->GetSphere(j), swordAttackBox[i]) && enemy->GetHP(j) > 0)
					{
						isSwordAttack[i] = false;
						isEnemySting[i][j] = true;
						enemy->DamegeThrowSword(j);
						if (enemyDamegeTime[j] > 0) {
							enemy->SetDamegeFlag(j, true);
						}

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
		for (size_t j = 0; j < enemy->GetEnemySize(); j++)
		{
			if (isSwordAttack[i] == false)
			{
				if (Collision::CheckBox2Box(pBox, swordAttackBox[i]) && holdingFlag[i])
				{
					haveSword[i] = true;
				}
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
			//reverseValue[i] = 180;
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
			}
		}

		if (explosion[i])
		{
			explosionCount[i]++;
			swordPosition[i].x += cos(swordAngleVec[i] + reverseValue[i]) * 2 * slowValue;      // x���W���X�V
			swordPosition[i].z += sin(swordAngleVec[i] + reverseValue[i]) * 2 * slowValue;      // z���W���X�V
			holdingFlag[i] = true;
			if (explosionCount[i] >= 20)
			{
				swordAngle[i].z = -90;
				explosion[i] = false;
			}
		}
	}
}



void   Player::Angle()
{
	float rad = 0.0f;
	//�E��
	if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//�E��
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//����
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}//��
	else if (Input::Instance()->KeybordPush(DIK_UP)) {
		rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//�E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
		rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//��
	else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
		rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);

	}//��
	else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
		rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		sinRad = sinf(rad);
		cosRad = cosf(rad);
	}

	if (Input::Instance()->ControllerPush(LButtonRight) || Input::Instance()->ControllerPush(LButtonLeft) ||
		Input::Instance()->ControllerPush(LButtonUp) || Input::Instance()->ControllerPush(LButtonDown))
	{
		rad = Input::Instance()->GetLeftAngle();
		sinRad = sinf(-rad);
		cosRad = cosf(rad);
	}
}

void Player::SwordAngle()
{
	//�E�R���g���[��
	if (Input::Instance()->ControllerPush(RButtonRight) || Input::Instance()->ControllerPush(RButtonLeft) ||
		Input::Instance()->ControllerPush(RButtonUp) || Input::Instance()->ControllerPush(RButtonDown))
	{
		float rad = 0.0f;
		rad = Input::Instance()->GetRightAngle();
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

void Player::Damage()
{
	//�_���[�W��H�炤
	damageTime = damageTimeMax;
	HP--;
}

void Player::UIDraw()
{
	//HP
	if (HP > 0)
	{
		Sprite::Instance()->Draw(HPGaugeSub, Vec2(50.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
		Sprite::Instance()->Draw(HPGaugeMain, Vec2(50.0f, 35.0f), 380.0f * (HP / HPMAX), 20.0f);
	}
	Sprite::Instance()->Draw(HPGraph, Vec2(0.0f, 30.0f), 500.0f, 30.0f);

	//�\�[�h�Q�[�W
	//Sprite::Instance()->Draw(swordGargeSub, Vec2(), 100.0f, 100.0f);
	//Sprite::Instance()->Draw(swordGargeMain, Vec2(), 100.0f, 100.0f);
	Sprite::Instance()->Draw(swordGraph, Vec2(), 100.0f, 100.0f);

}

void Player::DebugDraw()
{
#if _DEBUG

#endif
}

void Player::EffectDraw()
{
	if (AttackEffect == true)
	{
		switch (direction)
		{

		case Up:
			AttackAngle.y = 90.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z + r + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Down:
			AttackAngle.y = 270.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x, position.y, position.z - r - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Left:
			AttackAngle.y = 0.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - r - AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case Right:
			AttackAngle.y = 180.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + r + AttackEffectSize, position.y, position.z), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);
			break;
		case UpRight:
			AttackAngle.y = 120.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case UpLeft:
			AttackAngle.y = 60.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z + AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case DownRight:
			AttackAngle.y = 240.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x + AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		case DownLeft:
			AttackAngle.y = 300.0f;
			Object::Instance()->Draw(AttackEffectOBJ, Vec3(position.x - AttackEffectSize, position.y, position.z - AttackEffectSize), AttackScale, AttackAngle, color, AttackEffectGraph[effectCount]);

			break;
		}
	}
	//AttackEffect = true;
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
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalLength, position.y, position.z + normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case DownRight:
		normalAttackBox.maxPosition = XMVectorSet(position.x + normalLength, position.y, position.z - normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	case DownLeft:
		normalAttackBox.maxPosition = XMVectorSet(position.x - normalLength, position.y, position.z - normalLength, 1);
		normalAttackBox.minPosition = XMVectorSet(position.x, position.y, position.z, 1);
		break;
	}
}