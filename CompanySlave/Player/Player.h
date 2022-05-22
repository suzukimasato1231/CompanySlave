#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include"Easing.h"
#include"Sprite.h"
#include"Direction.h"
#include <array>
#include<time.h>
#include"Vec.h"
#include "Safe_delete.h"
#include "Audio.h"
class Enemy;
/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
public:
	static const int eNumMax = 100;
	Player();						//コンストラクタ

	~Player();						//ですコンストラクタ

	void Init();					//初期化

	void LoopInit();				//ループでの初期化

	void StageInit(int stageNum);	//ステージごとの初期化

	void Update(class Enemy* enemy);//更新

	void Draw();					//描画
	//プレイヤーの位置を設定
	void SetPosition(Vec3 position) { this->position = position; }
	//剣の位置を設定
	void SetSwordAttack(int i) { this->isSwordAttack[i] = false; }

	void SetSwordReverse(float reverseValue, int i) { this->reverseValue[i] = reverseValue; }

	void SetSwordAngle(Vec3 swordAngle, int i) { this->swordAngle[i] = swordAngle; }

	void SetSwordStop(int i) { this->swordStop[i] = true; }

	int GetHP() { return HP; }
	/// <summary>
	/// ダメージ
	/// </summary>
	void Damage(int damegeNum);
	//プレイヤーUI描画
	void UIDraw();
private:
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();

	//通常攻撃
	void NormalAttack(class Enemy* enemy);

	//剣攻撃
	void SwordAttack(class Enemy* enemy);

	//体力回復
	void LifePortion();
	//攻撃角度決定
	void Angle();
	//剣の向きを決める
	void SwordAngle();

	//プレイヤーの向きを決める
	void PDirection();

	//回避
	void Avoidance();

	/// <summary>
	/// 通常攻撃の範囲を決める
	/// </summary>
	void NormalFieldDirection();

	//無敵時間の更新
	void InvincivleUpdate();

	//エフェクト描画
	void EffectDraw();

	//プレイヤーに一番近い敵を求める
	int  EnemyNeedNumber(Enemy* enemy);

	//剣を全部もってるかどうか
	bool IsSwordALLHave();
	//不発時バツのシェイク
	void ShakeUpdate();

	bool AttackToFlag();
public://取得系
	//座標
	Vec3 GetPosition() { return position; }

	Vec3 GetOldPosition() { return oldPosition; }

	float GetPSize() { return r; }
	//プレイヤーのBox
	Box GetBox() { return pBox; }

	bool GetMoveFlag() { return moveFlag; }
	Vec3 GetAngle() { return angle; }
	//座標
	Vec3 GetSwordPosition(int No) { return swordPosition[No]; }

	Vec3 GetSwordAngle(int i) { return swordAngle[i]; }
	//プレイヤーのBox
	Box GetSwordBox(int No) { return swordAttackBox[No]; }

	float GetSlow() { return slowValue; }
	float GetenemyDamegeTime(int i) { return enemyDamegeTime[i]; }
	int GetDirection() { return direction; }

	int GetInvincivleTime() { return invincivleTime; }
	//カメラの座標を求める
	Vec3 GetCameraPos();
	bool GetSS() { return swordSoundFlag; }
	float SetVolume(float volume) { return this->volume = volume; };

private:
	Object::ObjectData playerSwordWalkObject[4];	//プレイヤー歩きオブジェクト(剣あり)
	Object::ObjectData playerAttackObject[9];
	Object::ObjectData playerDamageObject;          //ダメージ食らったとき//プレイヤー攻撃
	Object::ObjectData playerLifeObject[2];			//プレイヤー回復
	int walkCount = 0;								//描画用カウント
	int walkNo = 0;									//描画するNO
	int attackCount = 0;							//攻撃モーション描画用カウント
	int attackNo = 0;								//攻撃モーションNo
	int portionNo = 0;								//描画するNO
	bool attackMode = false;
	Box pBox;										//プレイヤーの長方形
	Sphere pSphere;									//プレイヤーの球
	Vec3 position{ 10.0f,0.0f,-10.0f };				//座標
	Vec3 oldPosition{};								//1つ前の座標
	Vec3 speed{ 1.5f,1.5f,1.5f };					//プレイヤースピード
	Vec3 scale{ 1.0f,1.0f,1.0f };					//大きさ
	Vec3 angle{ 0.0f,0.0f,0.0f };					//角度
	Vec4 color{ 1.0f,1.0f,1.0f,1.0f };				//色
	float r = 3;									//プレイヤーの半径
	int  direction = 0;								//プレイヤーの向き
	float sinRad = 0;
	float cosRad = 0;
	float RsinRad = 0;
	float RcosRad = 0;
	Vec3 Rangle = { 0.0f ,0.0f,0.0f };
	const float HPMAX = 12;
	float HP = 12;						//プレイヤーHP
	float HPSub = 12;					//描画上の演出用HP
	const int damageTimeMax = 20;
	int damageTime = 0;					//ダメージ食らったかの見た目用

	//回復アイテム
	SpriteData portionSprite[2];
	const int portionMax = 2;			//最大個数
	int portion = 0;
	bool portionFlag = false;
	const int portionTimeMax = 50;		//回復時間
	int portionTime = 0;

	//動いているかどうか
	bool moveFlag = false;

	//回避
	bool avoidanceFlag = false;		//回避中か
	const int avoidanceTimeMax = 10;//回避時間
	int avoiDirection = 0;			//回避向き
	int avoidanceTime = 0;			//今回避時間
	float avoiSpeed = 4.0f;			//回避スピード
	const int avoiCoolTimeMax = 20;	//回避クールタイム
	int avoiCoolTime = 0;			//今回避クールタイム
	float radDir = 0.0f;

	//ダメージ後の無敵
	const int invincibleTimeMax = 60;
	int invincivleTime = 0;

	//通常攻撃
	std::array <bool, 3> normalAttackFlag = { false,false,false };	//通常攻撃可能か
	float normalLength = 20.0f;							//攻撃の半径
	float normalLengthSub = 15.0f;						//向いていない方向の攻撃の半径
	const int normalAttackTimeMax = 35;					//攻撃と攻撃の間の時間
	int normalAttackTime = 0;							//攻撃と攻撃の間
	int normalDirection = 0;							//攻撃の向き
	Box normalAttackBox;								//実際の攻撃の当たり判定
	int normalAttackCount = 0;							//通常攻撃の何回目か
	const int normalGraceTimeMax = 50;					//連続切りまでの猶予
	int normalGraceTime = 0;							//連続切りまでの猶予
	float attackMoveHighSpeed = 0.7f;					//３撃目の進むスピード
	float attackMoveSpeed = 0.2f;						//攻撃方向へ進むスピード

	const float attackMoveSpeed2 = 2.0f;
	const float attackMoveHighSpeed2 = 3.0f;

	//剣攻撃
	Object::ObjectData swordObject;//剣
	Object::ObjectData tornadoObject;//剣
	int cursorGraph;	//カーソル
	Object::ObjectData cursorObject;//カーソル
	SpriteData BlackGraph;//剣回収の白いやつ
	bool BlackFlag = false; //白いやつフラグ

	std::array <Vec3, 7> swordPosition = { Vec3{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f },{ 10.0f, 0.0f, 0.0f } };	//座標
	Object::ObjectData swordEffectObject;//剣
	std::array<Vec3, 7> swordAngle = { Vec3{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f } };		//角度
	float swordSpeed = 6;//スピード
	std::array<float, 7> swordAngleVec = { 0,0,0,0,0,0,0 };//飛ばす方向
	std::array<bool, 7> isSwordAttack = { false,false,false,false,false,false,false };//アタックフラグ
	std::array<int, 7> stingCnt = { 0,0,0,0,0,0,0 };//刺さるまでの時間
	std::array<bool, 7> haveSword = { true,true,true,true,true,true,true };//持ってる剣
	bool isEnemySting[7][eNumMax];			//敵に刺さってるか
	std::array < Box, 7> swordAttackBox;	//剣の当たり判定
	int shotNo = 0;							//どの剣か
	bool returnFlag = false;				//剣が戻る時のフラグ
	float nowTime = 0;						//剣が戻る時のラープ
	float endTime = 3;						//剣が戻る時のラープ
	float timeRate = 0;						//剣が戻る時のラープ
	std::array<float, 7> reverseValue = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f };
	std::array<int, 7>  reverseAngle = { 0,0,0,0,0,0,0 };
	std::array<bool, 7>  swordStop = { false,false,false,false,false,false,false };
	std::array<bool, 7>  holdingFlag = { true,true,true,true,true,true,true };
	std::array<bool, 7>  explosion = { false,false,false,false,false,false,false };
	std::array<int, 7> explosionCount = { 0,0,0,0,0,0,0 };
	std::array<float, 7> explosionAngle = { 0,0,0,0,0,0,0 };
	Vec3 havePosition = { 10.0f, 0.0f, 0.0f };
	Box haveBox;
	float tornadoAngle = 0;
	float tornadoScale = 0.5;
	float slowValue = 1;
	int slowCount = 0;
	bool slowFlag = false;

	bool eslowFlag = false;
	int eslowTime = 120;

	bool homingFlag = true;
	//剣回収クールタイム
	SpriteData swordUI[10];			//クールタイムの時間
	SpriteData skillUI[2];			//クールタイムゲージ
	SpriteData swordNot;			//剣回収不発時
	SpriteData swordRotationGraph;	//回収ゲージが溜まった時の演出
	SpriteData swordPre;
	time_t start_time, end_time;	//時間計測
	const float swordCoolTimeMax = 15.0f;
	float swordCoolTime = swordCoolTimeMax;
	bool swordCoolTimeFlag = false;			//ソードゲージが回復したかどうか
	int swordCoolTimePlas = 0;

	const float swordNotTimeMax = 20;//回収時不発時間
	float swordNotTime = 0;
	const float lifeNotTimeMax = 20;//回復不発時時間
	float lifeNotTime = 0;
	Vec2 shake = {};				//不発時シェイク

	SpriteData lifeNot;				//ライフ満タン時

	//エフェクト関係
	float AttackEffectSize = 5.0f;
	Vec3 AttackScale{ 1.0f,1.0f,1.0f };
	Vec3 AttackAngle{ 90.0f,0.0f,0.0f };

	std::array<float, eNumMax >enemyDamegeTime = {};

	//UI
	Vec3 UIAngle{ 90.0f,0.0f,0.0f };

	//プレイヤーHP
	SpriteData HPGraph;
	SpriteData HPGaugeSub;
	SpriteData HPGaugeMain;

	//ソードゲージ
	SpriteData swordGraph[15];
	SpriteData swordGargeSub;
	SpriteData swordGargeMain;

	//右スティックを入力したか
	bool inputStickFlag = false;

	//音
	Audio* audio = nullptr;
	//音データ
	SoundData sound1;
	SoundData sound2;
	SoundData sound3;
	SoundData sound4;
	SoundData sound5;
	SoundData sound6;
	SoundData sound7;
	SoundData sound8;
	SoundData sound9;
	SoundData sound10;

	bool swordSoundFlag = false;
	bool swordSoundFlag2 = false;
	bool swordSoundCount = 0;
	float volume = 1.0f;

	float collectCount = 0.1f;
#if _DEBUG
	Object::ObjectData attackField;//攻撃範囲可視化
	int redColor;

	Object::ObjectData normalFieldOBJ;//通常攻撃
	Object::ObjectData normalFieldOBJ2;//通常攻撃
	//int normalGraph;

#endif
	Object::ObjectData AttackEffectOBJ;//通常攻撃エフェクト
	bool attackEfectFlag = true;
	int AttackEffectGraph[27];
	int attackEfectNo = 0;
	int attackEffectCount = 0;
	bool AttackEnd = false;
};
