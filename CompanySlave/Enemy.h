#pragma once
#include"Object.h"
#include"CollisionPrimitive.h"
#include<vector>
#include"Player.h"
class Enemy
{
private://構造体
	//敵の情報
	struct EnemyData
	{
		Object::ObjectData enemyObject;		//プレイヤーオブジェクト
		Box eBox;							//敵ボックス
		Vec3 position{ 50.0f,0.0f,0.0f };	//座標
		Vec3 oldPosition{};					//1個前の座標
		Vec3 speed{ 2.0f,2.0f,2.0f };		//プレイヤースピード
		Vec3 scale{ 1.0f,1.0f,1.0f };		//大きさ
		Vec3 angle{ 0.0f,0.0f,0.0f };		//角度
		Vec4 color{ 1.0f,1.0f,1.0f,1.0f };	//色
		int HP = 10;						//HP
		float r = 5;						//大きさ
		bool wasAttackFlag = false;			//攻撃されたかどうか
	};
private:
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
private:
	//移動
	void Move(int i);
	//ダメージ処理
	void Damege(int i, class Player *player);
	//生成
	void Generation(class Player *player);
	//削除
	void Delete();
public://取得系
	// 座標取得
	Vec3 GetPosition(int i) { return eData[i]->position; }

	Box GetBox(int i) { return eData[i]->eBox; }
	//エネミー数取得
	int GetEnemySize() { return eData.size(); }
	//攻撃されたかを取得
	bool GetWasAttackFlag(int i) { return eData[i]->wasAttackFlag; }
};