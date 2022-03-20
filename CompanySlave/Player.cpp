#include"Player.h"
#include"Shape.h"
#include"Input.h"
#include "MapStage.h"
#include"Collision.h"
Player::Player()
{}

Player::~Player()
{}

void Player::Init()
{
	playerObject = Object::Instance()->CreateOBJ("player");
	pBox.minPosition = XMVectorSet(0, 2, 0, 1);
	pBox.maxPosition = XMVectorSet(0, 2, 0, 1);
	oldPosition = position;
}

void Player::Update(Enemy *enemy)
{
	if (enemy == nullptr)
	{
		return;
	}
	//ˆÚ“®
	Move();

	//Ža‚è‚És‚­“G‚ÌÀ•W‚ð’T‚·
	PlayerAttack(enemy);

	//UŒ‚‚ðŽ~‚ß‚é
	StopAttack();
}

void Player::Draw()
{
	Object::Instance()->Draw(playerObject, position, scale, angle, color);
}

//ˆÚ“®
void Player::Move()
{
	oldPosition = position;

	//ˆÚ“®
	if (Input::Instance()->KeybordPush(DIK_RIGHT) || Input::Instance()->KeybordPush(DIK_LEFT)
		|| Input::Instance()->KeybordPush(DIK_UP) || Input::Instance()->KeybordPush(DIK_DOWN))
	{
		moveFlag = true;
	}
	else {
		moveFlag = false;
	}

	if (attackFlag == false && nowComboTime <= 0)
	{
		//ˆÚ“®
		if (Input::Instance()->KeybordPush(DIK_RIGHT))
		{
			position.x += speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_LEFT))
		{
			position.x -= speed.x;
		}
		if (Input::Instance()->KeybordPush(DIK_UP))
		{
			position.z += speed.z;
		}
		if (Input::Instance()->KeybordPush(DIK_DOWN))
		{
			position.z -= speed.z;
		}
		//À•W‚ð‡‚í‚¹‚é
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}
}

//ƒvƒŒƒCƒ„[‚ÆƒGƒlƒ~[‚Æ‚ÌÅ¬‹——£‚Ì“G‚ðŒ©‚Â‚¯‚é
void Player::PlayerAttack(Enemy *enemy)
{
	if (enemy == nullptr) { return; }
	//UŒ‚ŠJŽn
	if (((Input::Instance()->KeybordPush(DIK_LEFT) || Input::Instance()->KeybordPush(DIK_UP) ||
		Input::Instance()->KeybordPush(DIK_DOWN) || Input::Instance()->KeybordPush(DIK_RIGHT)))
		&& coolTime <= 0 && attackFlag == false && Input::Instance()->KeybordPush(DIK_C))
	{
		attackAngle = Angle();
		attackFlag = true;
		comboFlag = false;
		attackTime = attackMaxTime;
	}


	//“G‚ÉŒü‚©‚Á‚Ä‚¢‚­ˆ—
	if (attackFlag == true && attackTime > 0)
	{
		//“G‚Ì•ûŒü‚ÉŒü‚©‚Á‚Ä‚¢‚­
		position.x += attackSpeed.x * cosf(attackAngle);
		position.z += attackSpeed.z * sinf(attackAngle);

		//“G‚Æ“–‚½‚Á‚½‚©
		for (int i = 0; i < enemy->GetEnemySize(); i++)
		{
			if (Collision::CheckBox2Box(pBox, enemy->GetBox(i))
				&& enemy->GetWasAttackFlag(i) == false)
			{
				enemy->WasAttack(i);
				comboFlag = true;
				comboNum++;
			}
		}
		//UŒ‚ŽžŠÔŒ¸­
		attackTime--;
		if (attackTime == 0)
		{//“G‚ðŽa‚èI‚í‚Á‚½‚ç
			attackFlag = false;
			//ƒRƒ“ƒ{¬Œ÷Žž
			if (comboFlag == true)
			{
				comboTime = comboMaxTime;
				nowComboTime = comboTime;
			}
			//ƒRƒ“ƒ{Ž¸”sŽž
			else
			{
				coolTime = coolTimeMax;
				nowComboTime = 0;
				comboNum = 0;
			}
		}
		//À•W‚ð‡‚í‚¹‚é
		pBox.minPosition = XMVectorSet(position.x - r, position.y - r, position.z - r, 1);
		pBox.maxPosition = XMVectorSet(position.x + r, position.y + r, position.z + r, 1);
	}


	//ƒRƒ“ƒ{ƒ^ƒCƒ€Œ¸­
	if (comboFlag == true && attackFlag == false)
	{
		nowComboTime--;
		if (nowComboTime == 0)
		{
			comboFlag = false;
			coolTime = coolTimeMax;
			comboNum = 0;
		}
	}
	//ƒN[ƒ‹ƒ^ƒCƒ€Œ»Û
	if (coolTime > 0 && comboFlag == false && attackFlag == false)
	{
		coolTime--;
	}

}

void Player::StopAttack()
{
	if (Input::Instance()->KeybordPush(DIK_SPACE))
	{
		attackFlag = false;
		comboFlag = false;
		coolTime = coolTimeMax;
		nowComboTime = 0;
		comboNum = 0;
	}
}

float  Player::Angle()
{	//‰Eã
	if ((Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_UP))
		|| (Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonUp))) {
		float rad = atan2(position.z + 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//‰E‰º
	else if (Input::Instance()->KeybordPush(DIK_RIGHT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonRight) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x + 10.0f - position.x);
		return rad;
	}//¶‰º
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_DOWN)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonDown)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//¶ã
	else if (Input::Instance()->KeybordPush(DIK_LEFT) && Input::Instance()->KeybordPush(DIK_UP)
		|| Input::Instance()->ControllerPush(LButtonLeft) && Input::Instance()->ControllerPush(LButtonUp)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - 10.0f - position.x);
		return rad;
	}//ã
	else if (Input::Instance()->KeybordPush(DIK_UP)) {
		float rad = atan2(position.z + 10.0f - position.z, position.x - position.x);
		return rad;
	}//‰E
	else if (Input::Instance()->KeybordPush(DIK_RIGHT)) {
		float rad = atan2(position.z - position.z, position.x + 10.0f - position.x);
		return rad;
	}//‰º
	else if (Input::Instance()->KeybordPush(DIK_DOWN)) {
		float rad = atan2(position.z - 10.0f - position.z, position.x - position.x);
		return rad;
	}//¶
	else if (Input::Instance()->KeybordPush(DIK_LEFT)) {
		float rad = atan2(position.z - position.z, position.x - 10.0f - position.x);
		return rad;
	}
	return 0.0f;
}
