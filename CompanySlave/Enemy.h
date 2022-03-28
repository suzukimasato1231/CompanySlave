#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
#include"Player.h"
#include"LoadCSV.h"

enum spawnNo
{
	NOTSPAWN,
	ONESPAWN,
	TWOSPAWN,
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
		bool wasAttackFlag = false;			//攻撃されたかどうか
		int damegeTime = 0;					//点滅時間
	};
	//敵スポーン
	struct EnemySpawner
	{
		Vec3 position;				//位置
		int enemyNum;				//
		float spawnTime = 0;		//今敵のスポーン間隔時間
		float spawnTimemax = 50;	//敵のスポーン間隔時間
		int enemyKind = 0;			//敵の種類
	};
private:
	//敵最大数
	static const int eNumMax = 20;
	//敵配列
	std::vector<EnemyData *>eData;
	//敵スポーン
	std::vector<EnemySpawner *>eSpawner;
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
	//ダメージ処理
	void Damege(int i, class Player *player);
	//生成
	void Generation(class Player *player);
	//削除
	void Delete();
	//エネミーとエネミーの押し戻し
	void Enemy2Enemy(class Player *player);
public://取得系
	// 座標取得
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Vec3 GetOldPosition(int i) { return eData[i]->oldPosition; }

	Box GetBox(int i) { return eData[i]->eBox; }

	Sphere GetSphere(int i) { return eData[i]->eSphere; }
	//エネミー数取得
	int GetEnemySize() { return eData.size(); }
	//攻撃されたかを取得
	bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }

	float GetEnemyR(int i) { return eData[i]->r; }

	int	spawnMap[MAP_HEIGHT][MAP_WIDTH];//沸き地のマップ

	const float mapSize = 10.0f;

	Vec3 basePosition = { 0,0,0 };//マップチップの初期位置
};