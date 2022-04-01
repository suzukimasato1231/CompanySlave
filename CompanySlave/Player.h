#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Enemy.h"
#include"Sprite.h"
#include"Direction.h"
/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	Player();//コンストラクタ

	~Player();//ですコンストラクタ

	void Init();//初期化

	void StageInit(int stageNum);//ステージごとの初期化

	void Update(class Enemy *enemy);//更新

	void Draw();//描画

	void SetPosition(Vec3 position) { this->position = position; }

	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage();
	//プレイヤーUI描画
	void UIDraw();
private:
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();

	//通常攻撃
	void NormalAttack(class Enemy *enemy);

	//剣攻撃
	void SwordAttack(class Enemy *enemy);

	//攻撃角度決定
	void Angle();

	//プレイヤーの向きを決める
	void PDirection();

	//回避
	void Avoidance();

	/// <summary>
	/// 通常攻撃の範囲を決める
	/// </summary>
	void NormalFieldDirection();

	//デバック描画
	void DebugDraw();

	//エフェクト描画
	void EffectDraw();
public://取得系
	//座標
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//プレイヤーのBox
	Box GetBox() { return pBox; }

	bool GetMoveFlag() { return moveFlag; }

private:
	Object::ObjectData playerSwordWalkObject[4];	//プレイヤー歩きオブジェクト(剣あり)
	Object::ObjectData playerAttackObject[4];	//プレイヤー攻撃
	int walkCount = 0;	//描画用カウント
	int walkNo = 0;	//描画するNO
	int attackCount = 0;	//描画用カウント
	int attackNo = 0;	//描画するNO
	bool attackMode = false;
	Box pBox;							//プレイヤーの長方形
	Sphere pSphere;						//プレイヤーの球
	Vec3 position{ 10.0f,0.0f,-10.0f };	//座標
	Vec3 oldPosition{};					//1つ前の座標
	Vec3 speed{ 1.5f,1.5f,1.5f };		//プレイヤースピード
	Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
	Vec3 angle{ 0.0f,0.0f,0.0f };		//角度
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
	float r = 3;						//プレイヤーの半径
	int  direction = 0;					//プレイヤーの向き
	float sinRad = 0;
	float cosRad = 0;
	const float HPMAX = 10;
	float HP = 10;						//プレイヤーHP
	const int damageTimeMax = 20;
	int damageTime = 0;					//ダメージ食らったかの見た目用

	//動いているかどうか
	bool moveFlag = false;

	//回避
	bool avoidanceFlag = false;		//回避中か
	const int avoidanceTimeMax = 10;//回避時間
	int avoiDirection = 0;			//回避向き
	int avoidanceTime = 0;			//今回避時間
	float avoiSpeed = 5.0f;			//回避スピード
	const int avoiCoolTimeMax = 20;	//回避クールタイム
	int avoiCoolTime = 0;			//今回避クールタイム
	float radDir = 0.0f;
	bool invincibleFlag = false;	//無敵中か


	//通常攻撃
	bool normalAttackFlag[3] = { false,false,false };	//通常攻撃可能か
	float normalLength = 5.0f;							//攻撃の半径
	const int normalAttackTimeMax = 20;					//攻撃と攻撃の間の時間
	int normalAttackTime = 0;							//攻撃と攻撃の間
	int normalDirection = 0;							//攻撃の向き
	Box normalAttackBox;								//実際の攻撃の当たり判定
	int normalAttackCount = 0;							//通常攻撃の何回目か
	const int normalGraceTimeMax = 50;					//連続切りまでの猶予
	int normalGraceTime = 0;							//連続切りまでの猶予
	float attackMoveSpeed = 0.7f;						//攻撃方向へ進むスピード

	//剣攻撃
	Object::ObjectData swordObject;//剣
	int cursorGraph;	//カーソル
	Object::ObjectData cursorObject;//カーソル

	Vec3 swordPosition[7] = { { 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f } };	//座標
	Vec3 swordAngle[7] = { { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } };		//角度
	float swordSpeed[7] = { 3,3,3,3,3,3,3 };//スピード
	float swordAngleVec[7] = { 0,0,0,0,0,0,0 };//飛ばす方向
	bool isSwordAttack[7] = { false,false,false,false,false,false,false };//アタックフラグ
	int stingCnt[7] = { 0,0,0,0,0,0,0 };//刺さるまでの時間
	bool haveSword[7] = { true,true,true,true,true,true,true };//持ってる剣
	bool isEnemySting[7][4];//敵に刺さってるか
	Box swordAttackBox[7];	//剣の当たり判定
	int shotNo = 0;//どの剣か
	bool returnFlag = false;//剣が戻る時のフラグ
	float nowTime = 0;//剣が戻る時のラープ
	float endTime = 5;//剣が戻る時のラープ
	float timeRate = 0;//剣が戻る時のラープ

	//エフェクト関係
	bool AttackEffect = false;
	float AttackEffectSize = 5.0f;
	Vec3 AttackScale{ 1.0f,1.0f,1.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };
	int effectTime = 10;
	int effectCount = 0;
	

	//UI
	Vec3 UIAngle{ 90.0f,0.0f,0.0f };

	//プレイヤーHP
	Sprite::SpriteData HPGraph;
	Sprite::SpriteData HPGaugeSub;
	Sprite::SpriteData HPGaugeMain;

	//ソードゲージ
	Sprite::SpriteData swordGraph;
	Sprite::SpriteData swordGargeSub;
	Sprite::SpriteData swordGargeMain;

	//Object::ObjectData comboPolygon;
	//Object::ObjectData  comboNumberObj;//１桁目
	//Object::ObjectData black;
	/*int comboGraph;
	int comboNumberGraph[10];*/
	int yellowColor;



#if _DEBUG
	Object::ObjectData attackField;//攻撃範囲可視化
	int redColor;

	Object::ObjectData normalFieldOBJ;//通常攻撃
	int normalGraph;

#endif
	Object::ObjectData AttackEffectOBJ;//通常攻撃エフェクト
	int AttackEffectGraph[9];
};
