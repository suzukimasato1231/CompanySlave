#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
/// <summary>
/// �U���������
/// </summary>
enum AttackDirection
{
	AttackUp,
	AttackDown,
	AttackLeft,
	AttackRight,
};
/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
public:
	Player();//�R���X�g���N�^

	~Player();//�ł��R���X�g���N�^

	void Init();//������

	void Update(class Enemy *enemy);//�X�V

	void Draw();//�`��

	void SetPosition(Vec3 position) { this->position = position; }
private:
	/// <summary>
	/// �v���C���[�ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// �v���C���[�ƃG�l�~�[�Ƃ̍ŏ������̓G��������
	/// </summary>
	/// <param name="enemy">�G�l�~�[�N���X</param>
	void PlayerAttack(class Enemy *enemy);

	/// <summary>
	/// �_���[�W����
	/// </summary>
	void Damege();

	/// <summary>
	/// �w�肵�������ɓG�����邩�ǂ���
	/// </summary>
	/// <param name="enemy">�G�l�~�[�N���X</param>
	/// <param name="enemyNumber">�ǂ̓G</param>
	/// <returns></returns>
	bool AttackDirection(class Enemy *enemy, int enemyNumber);

	/// <summary>
	/// �U�����~�߂�
	/// </summary>
	void StopAttack();
public://�擾�n
	//���W
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//�v���C���[��Box
	Box GetBox() { return pBox; }

	bool GetAttackFlag() { return attackFlag; }
	bool GetMoveFlag() { return moveFlag; }

	int GetComboNum() { return comboNum; }
private:
	Object::ObjectData playerObject;	//�v���C���[�I�u�W�F�N�g

	Box pBox;							//�v���C���[�̒����`

	Vec3 position{ 10.0f,0.0f,0.0f };	//���W
	Vec3 oldPosition{};					//1�O�̍��W
	Vec3 speed{ 2.0f,2.0f,2.0f };		//�v���C���[�X�s�[�h

	Vec3 scale{ 1.0f,1.0f,1.0f };		//�傫��
	Vec3 angle{ 0.0f,0.0f,0.0f };		//�p�x
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//�F
	float r = 3;

	//�v���C���[HP
	int HP = 10;

	//�G��|�����߂̃p�����[�^
	const float comboMaxTime = 30;	//�R���{���ԍŏ�
	float comboTime = 30;			//�R���{����
	float nowComboTime = 0;			//���݂̃R���{����

	const float AttackSpeed = 3;	//�U���X�s�[�h

	bool selectEnemyFlag = false;//�a��ɍs���G�����܂������ǂ���
	int attackDirection = 0;		//�a��ɍs������
	Vec3 enemyPos = {};				//�a��ɍs���G�̍��W
	Vec3 direction = {};			//�a��ɍs���G�̕���
	int enemyNum = 0;				//�ǂ̓G���̏��
	Box eBox;

	//�����Ă��邩�ǂ���
	bool moveFlag = false;

	//�A���؂�̍Œ����ǂ���
	bool attackFlag = false;
	//�R���{��
	int comboNum = 0;

	//�U���͈�
	Vec2 attackPos = {};//�U���J�n�ʒu
	float circleMain = 50.0f;
	float ciecleSub = 50.0f;
#if _DEBUG
	Object::ObjectData circleM;
	Object::ObjectData circleS;
	int white;
	int blue;
	int yellow;
#endif
};