#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
#include"Player.h"
#include"LoadCSV.h"
#include"Direction.h"
enum spawnNo
{
	NOTSPAWN,
	ONESPAWN,
	TWOSPAWN,
};

//敵の状態
enum Status
{
	NORMAL, //通常状態
	MOVE,	//動き
	ATTACK,	//攻撃
	ENEMIES,//敵を見つけたら
};

class Enemy
{
private://構造体
	//敵の情報
	struct EnemyData
	{
		Object::ObjectData enemyObject;		//プレイヤーオブジェクト
		Box eBox;							//敵ボックス
		Sphere eSphere;						//敵球
		Vec3 position{ 50.0f,0.0f,0.0f };	//座標
		Vec3 oldPosition{};					//1個前の座標
		float speed = 0.5f;					//敵スピード
		Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
		Vec3 angle{ 0.0f,0.0f,0.0f };		//角度
		Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
		int HP = 10;						//HP
		float r = 5;						//大きさ
		int Status = NORMAL;				//状態
		int StatusTime = 0;					//状態時間
		int direction = Down;				//向いている向き
		int attackDirection = Down;			//攻撃の向き
		int damegeTime = 0;					//点滅時間
	};
private:
	//敵最大数
	static const int eNumMax = 20;
	//敵配列
	std::vector<EnemyData *>eData;

public:
	Enemy();//コンストラクタ

	~Enemy();//ですコンストラクタ

	void Init();//初期化

	void Update(class Player *player);//更新

	void Draw();//描画

	//TRUEにする
	void WasAttack(int i);
	//敵の位置を設定
	void SetPosition(int i, Vec3 position);
	/// <summary>
	/// プレイヤーの通常攻撃ダメージ
	/// </summary>
	void DamegeNormal(int i);
private:
	//移動
	void Move(int i, Player *player);
	//索敵
	void SearchPlayer(int i, class Player *player);
	//攻撃
	void Attack(int i, class Player *player);
	//プレイヤーを見つけた後
	void Enemies(int i, class Player *player);
	//削除
	void Delete();
	//向きによって索敵範囲の決定
	Box SearchField(int i);
	//向きによって攻撃範囲の決定
	Box AttackField(int i);
	//敵が向いている方向
	int Direction(int i, class Player *player);
public://取得系
	// 座標取得
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//エネミー数取得
	int GetEnemySize() { return eData.size(); }
	//攻撃されたかを取得
	//bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//沸き地のマップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置
private:
	const Vec2 attackEnemies{ 40.0f,40.0f };		//索敵範囲:width,height

	const Vec2 attackField{ 10.0f,10.0f };			//攻撃範囲

	const int moveTime = 40;						//移動時間

	const int attackMotionTime = 60;				//攻撃モーションの時間
	const int attackMotionDamege = 10;				//攻撃モーション中のどの時間でダメージを与えるか

	const float player2EnemyLength = 10.0f;			//プレイヤーと敵の距離

	Object::ObjectData debugField;	//索敵
	Object::ObjectData debugField2;//攻撃範囲
	int redColor;
};